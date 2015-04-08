#include "MantidSINQ/PoldiFitPeaks2D.h"

#include "MantidDataObjects/Workspace2D.h"
#include "MantidDataObjects/TableWorkspace.h"
#include "MantidKernel/ListValidator.h"
#include "MantidAPI/TableRow.h"
#include "MantidAPI/FunctionFactory.h"
#include "MantidAPI/MultiDomainFunction.h"
#include "MantidSINQ/PoldiUtilities/PoldiSpectrumDomainFunction.h"
#include "MantidSINQ/PoldiUtilities/PoldiSpectrumLinearBackground.h"
#include "MantidSINQ/PoldiUtilities/PoldiSpectrumPawleyFunction.h"
#include "MantidAPI/FunctionDomain1D.h"

#include "MantidSINQ/PoldiUtilities/IPoldiFunction1D.h"
#include "MantidSINQ/PoldiUtilities/PoldiPeakCollection.h"
#include "MantidSINQ/PoldiUtilities/PoldiInstrumentAdapter.h"
#include "MantidSINQ/PoldiUtilities/PoldiDeadWireDecorator.h"
#include "MantidAPI/IPeakFunction.h"
#include "MantidAPI/IPawleyFunction.h"

#include "MantidSINQ/PoldiUtilities/Poldi2DFunction.h"

#include "boost/make_shared.hpp"
#include "MantidSINQ/PoldiUtilities/PoldiDGrid.h"

namespace Mantid {
namespace Poldi {
// Register the algorithm into the AlgorithmFactory
DECLARE_ALGORITHM(PoldiFitPeaks2D)

using namespace API;
using namespace Kernel;
using namespace DataObjects;

/** Constructor
 */
PoldiFitPeaks2D::PoldiFitPeaks2D()
    : Algorithm(), m_poldiInstrument(), m_timeTransformer(), m_deltaT(0.0) {}

/** Destructor
 */
PoldiFitPeaks2D::~PoldiFitPeaks2D() {}

/// Algorithm's name for identification. @see Algorithm::name
const std::string PoldiFitPeaks2D::name() const { return "PoldiFitPeaks2D"; }

/// Algorithm's version for identification. @see Algorithm::version
int PoldiFitPeaks2D::version() const { return 1; }

/// Algorithm's category for identification. @see Algorithm::category
const std::string PoldiFitPeaks2D::category() const { return "SINQ\\Poldi"; }

/// Very short algorithm summary. @see Algorith::summary
const std::string PoldiFitPeaks2D::summary() const {
  return "Calculates a POLDI 2D-spectrum.";
}

/// Validate inputs for algorithm in case PawleyFit is used.
std::map<std::string, std::string> PoldiFitPeaks2D::validateInputs() {
  std::map<std::string, std::string> errorMap;

  bool isPawleyFit = getProperty("PawleyFit");

  if (isPawleyFit) {
    Property *cellProperty = getPointerToProperty("InitialCell");
    if (cellProperty->isDefault()) {
      errorMap["InitialCell"] = "Initial cell must be given for PawleyFit.";
    }

    Property *refinedCellParameters =
        getPointerToProperty("RefinedCellParameters");
    if (refinedCellParameters->isDefault()) {
      errorMap["RefinedCellParameters"] = "Workspace name for refined cell "
                                          "parameters must be supplied for "
                                          "PawleyFit.";
    }
  }

  return errorMap;
}

/// Initialization of algorithm properties.
void PoldiFitPeaks2D::init() {
  declareProperty(new WorkspaceProperty<MatrixWorkspace>("InputWorkspace", "",
                                                         Direction::Input),
                  "Measured POLDI 2D-spectrum.");
  declareProperty(new WorkspaceProperty<TableWorkspace>("PoldiPeakWorkspace",
                                                        "", Direction::Input),
                  "Table workspace with peak information.");

  auto peakFunctionValidator = boost::make_shared<StringListValidator>(
      FunctionFactory::Instance().getFunctionNames<IPeakFunction>());
  declareProperty("PeakProfileFunction", "Gaussian", peakFunctionValidator,
                  "Profile function to use for integrating the peak profiles "
                  "before calculating the spectrum.");

  declareProperty("PawleyFit", false, "Instead of refining individual peaks, "
                                      "refine a unit cell. Peaks must be "
                                      "indexed, an initial cell must be "
                                      "provided, as well as a crystal system.");
  declareProperty("InitialCell", "", "Initial unit cell parameters as 6 "
                                     "space-separated numbers. Only used when "
                                     "PawleyFit is checked.");

  std::vector<std::string> crystalSystems;
  crystalSystems.push_back("Cubic");
  crystalSystems.push_back("Tetragonal");
  crystalSystems.push_back("Hexagonal");
  crystalSystems.push_back("Trigonal");
  crystalSystems.push_back("Orthorhombic");
  crystalSystems.push_back("Monoclinic");
  crystalSystems.push_back("Triclinic");

  auto crystalSystemValidator =
      boost::make_shared<StringListValidator>(crystalSystems);

  declareProperty("CrystalSystem", "Cubic", crystalSystemValidator,
                  "Crystal system to use for constraining "
                  "unit cell parameters. Only used when "
                  "PawleyFit is checked.");

  declareProperty("FitConstantBackground", true,
                  "Add a constant background term to the fit.");
  declareProperty("ConstantBackgroundParameter", 0.0,
                  "Initial value of constant background.");

  declareProperty("FitLinearBackground", true,
                  "Add a background term linear in 2theta to the fit.");
  declareProperty("LinearBackgroundParameter", 0.0,
                  "Initial value of linear background.");

  declareProperty("MaximumIterations", 0, "Maximum number of iterations for "
                                          "the fit. Use 0 to calculate "
                                          "2D-spectrum without fitting.");

  declareProperty(new WorkspaceProperty<MatrixWorkspace>("OutputWorkspace", "",
                                                         Direction::Output),
                  "Calculated POLDI 2D-spectrum");
  declareProperty(new WorkspaceProperty<MatrixWorkspace>("Calculated1DSpectrum",
                                                         "", Direction::Output),
                  "Calculated POLDI 1D-spectrum.");

  declareProperty("LambdaMin", 1.1,
                  "Minimum wavelength for 1D spectrum calculation");
  declareProperty("LambdaMax", 5.0,
                  "Minimum wavelength for 1D spectrum calculation");

  declareProperty(new WorkspaceProperty<TableWorkspace>(
                      "RefinedPoldiPeakWorkspace", "", Direction::Output),
                  "Table workspace with fitted peaks.");

  declareProperty(new WorkspaceProperty<ITableWorkspace>(
      "RefinedCellParameters", "", Direction::Output, PropertyMode::Optional));
}

/// Creates a PoldiPeak from the given profile function/hkl pair.
PoldiPeak_sptr
PoldiFitPeaks2D::getPeakFromPeakFunction(IPeakFunction_sptr profileFunction,
                                         const V3D &hkl) {

  IAlgorithm_sptr errorAlg = createChildAlgorithm("EstimatePeakErrors");
  errorAlg->setProperty(
      "Function", boost::dynamic_pointer_cast<IFunction>(profileFunction));
  errorAlg->setPropertyValue("OutputWorkspace", "Errors");
  errorAlg->execute();

  double centre = profileFunction->centre();
  double height = profileFunction->height();
  double fwhmValue = profileFunction->fwhm();

  ITableWorkspace_sptr errorTable = errorAlg->getProperty("OutputWorkspace");

  double centreError = errorTable->cell<double>(0, 2);
  double heightError = errorTable->cell<double>(1, 2);
  double fwhmError = errorTable->cell<double>(2, 2);

  UncertainValue d(centre, centreError);
  UncertainValue intensity(height, heightError);
  UncertainValue fwhm(fwhmValue, fwhmError);

  PoldiPeak_sptr peak =
      PoldiPeak::create(MillerIndices(hkl), d, intensity, UncertainValue(1.0));
  peak->setFwhm(fwhm, PoldiPeak::FwhmRelation::AbsoluteD);

  return peak;
}

/// Returns a TableWorkspace with refined cell parameters and error.
ITableWorkspace_sptr PoldiFitPeaks2D::getRefinedCellParameters(
    const IFunction_sptr &fitFunction) const {
  Poldi2DFunction_sptr poldi2DFunction =
      boost::dynamic_pointer_cast<Poldi2DFunction>(fitFunction);

  if (!poldi2DFunction || poldi2DFunction->nFunctions() < 1) {
    throw std::invalid_argument(
        "Cannot process function that is not a Poldi2DFunction.");
  }

  ITableWorkspace_sptr latticeParameterTable =
      WorkspaceFactory::Instance().createTable();

  latticeParameterTable->addColumn("str", "Parameter");
  latticeParameterTable->addColumn("double", "Value");
  latticeParameterTable->addColumn("double", "Error");

  // The first function should be PoldiSpectrumPawleyFunction
  boost::shared_ptr<PoldiSpectrumPawleyFunction> poldiPawleyFunction =
      boost::dynamic_pointer_cast<PoldiSpectrumPawleyFunction>(
          poldi2DFunction->getFunction(0));

  if (!poldiPawleyFunction) {
    throw std::invalid_argument("First function in Poldi2DFunction is not "
                                "PoldiSpectrumPawleyFunction.");
  }

  IPawleyFunction_sptr pawleyFunction =
      boost::dynamic_pointer_cast<IPawleyFunction>(
          poldiPawleyFunction->getDecoratedFunction());

  if (pawleyFunction) {
    CompositeFunction_sptr pawleyParts =
        boost::dynamic_pointer_cast<CompositeFunction>(
            pawleyFunction->getDecoratedFunction());

    IFunction_sptr pawleyParameters = pawleyParts->getFunction(0);

    for (size_t i = 0; i < pawleyParameters->nParams(); ++i) {
      TableRow newRow = latticeParameterTable->appendRow();
      newRow << pawleyParameters->parameterName(i)
             << pawleyParameters->getParameter(i)
             << pawleyParameters->getError(i);
    }
  }

  return latticeParameterTable;
}

/**
 * Extracts the covariance matrix for the supplied function
 *
 * This method extracts the covariance matrix for a sub-function from
 * the global covariance matrix. If no matrix is given, a zero-matrix is
 * returned.
 *
 * @param covarianceMatrix :: Global covariance matrix.
 * @param parameterOffset :: Offset for the parameters of profileFunction.
 * @param nParams :: Number of parameters of the local function.
 * @return
 */
boost::shared_ptr<DblMatrix> PoldiFitPeaks2D::getLocalCovarianceMatrix(
    const boost::shared_ptr<const Kernel::DblMatrix> &covarianceMatrix,
    size_t parameterOffset, size_t nParams) const {
  if (covarianceMatrix) {
    boost::shared_ptr<Kernel::DblMatrix> localCov =
        boost::make_shared<Kernel::DblMatrix>(nParams, nParams, false);
    for (size_t j = 0; j < nParams; ++j) {
      for (size_t k = 0; k < nParams; ++k) {
        (*localCov)[j][k] =
            (*covarianceMatrix)[parameterOffset + j][parameterOffset + k];
      }
    }

    return localCov;
  }

  return boost::make_shared<Kernel::DblMatrix>(nParams, nParams, false);
}

/**
 * Construct a PoldiPeakCollection from a Poldi2DFunction
 *
 * This method performs the opposite operation of
 *getFunctionFromPeakCollection.
 * It takes a function, checks if it's of the proper type and turns the
 *information
 * into a PoldiPeakCollection.
 *
 * @param  Poldi2DFunction with one PoldiSpectrumDomainFunction per peak
 * @return PoldiPeakCollection containing peaks with normalized intensities
 */
PoldiPeakCollection_sptr PoldiFitPeaks2D::getPeakCollectionFromFunction(
    const IFunction_sptr &fitFunction) {
  Poldi2DFunction_sptr poldi2DFunction =
      boost::dynamic_pointer_cast<Poldi2DFunction>(fitFunction);

  if (!poldi2DFunction) {
    throw std::invalid_argument(
        "Cannot process function that is not a Poldi2DFunction.");
  }

  PoldiPeakCollection_sptr normalizedPeaks =
      boost::make_shared<PoldiPeakCollection>(PoldiPeakCollection::Integral);

  boost::shared_ptr<const Kernel::DblMatrix> covarianceMatrix =
      poldi2DFunction->getCovarianceMatrix();

  size_t offset = 0;

  for (size_t i = 0; i < poldi2DFunction->nFunctions(); ++i) {
    boost::shared_ptr<PoldiSpectrumPawleyFunction> poldiPawleyFunction =
        boost::dynamic_pointer_cast<PoldiSpectrumPawleyFunction>(
            poldi2DFunction->getFunction(i));

    if (poldiPawleyFunction) {
      IPawleyFunction_sptr pawleyFunction =
          poldiPawleyFunction->getPawleyFunction();

      if (pawleyFunction) {
        CompositeFunction_sptr decoratedFunction =
            boost::dynamic_pointer_cast<CompositeFunction>(
                pawleyFunction->getDecoratedFunction());

        offset = decoratedFunction->getFunction(0)->nParams();

        for (size_t j = 0; j < pawleyFunction->getPeakCount(); ++j) {
          IPeakFunction_sptr profileFunction =
              pawleyFunction->getPeakFunction(j);

          size_t nLocalParams = profileFunction->nParams();
          boost::shared_ptr<Kernel::DblMatrix> localCov =
              getLocalCovarianceMatrix(covarianceMatrix, offset, nLocalParams);
          profileFunction->setCovarianceMatrix(localCov);

          // Increment offset for next function
          offset += nLocalParams;

          V3D peakHKL = pawleyFunction->getPeakHKL(j);

          PoldiPeak_sptr peak =
              getPeakFromPeakFunction(profileFunction, peakHKL);

          normalizedPeaks->addPeak(peak);
        }
      }
      break;
    }

    boost::shared_ptr<PoldiSpectrumDomainFunction> peakFunction =
        boost::dynamic_pointer_cast<PoldiSpectrumDomainFunction>(
            poldi2DFunction->getFunction(i));

    if (peakFunction) {
      IPeakFunction_sptr profileFunction =
          boost::dynamic_pointer_cast<IPeakFunction>(
              peakFunction->getProfileFunction());

      // Get local covariance matrix
      size_t nLocalParams = profileFunction->nParams();
      boost::shared_ptr<Kernel::DblMatrix> localCov =
          getLocalCovarianceMatrix(covarianceMatrix, offset, nLocalParams);
      profileFunction->setCovarianceMatrix(localCov);

      // Increment offset for next function
      offset += nLocalParams;

      PoldiPeak_sptr peak =
          getPeakFromPeakFunction(profileFunction, V3D(0, 0, 0));

      normalizedPeaks->addPeak(peak);
    }
  }

  return normalizedPeaks;
}

/**
 * Returns a Poldi2DFunction that encapsulates individual peaks
 *
 * This function takes all peaks from the supplied peak collection and
 *generates
 * an IPeakFunction of the type given in the name parameter, wraps them
 * in a Poldi2DFunction and returns it.
 *
 * @param profileFunctionName :: Profile function name.
 * @param peakCollection :: Peak collection with peaks to be used in the fit.
 * @return :: A Poldi2DFunction with peak profile functions.
 */
Poldi2DFunction_sptr PoldiFitPeaks2D::getFunctionIndividualPeaks(
    std::string profileFunctionName,
    const PoldiPeakCollection_sptr &peakCollection) const {
  Poldi2DFunction_sptr mdFunction(new Poldi2DFunction);

  for (size_t i = 0; i < peakCollection->peakCount(); ++i) {
    PoldiPeak_sptr peak = peakCollection->peak(i);

    boost::shared_ptr<PoldiSpectrumDomainFunction> peakFunction =
        boost::dynamic_pointer_cast<PoldiSpectrumDomainFunction>(
            FunctionFactory::Instance().createFunction(
                "PoldiSpectrumDomainFunction"));

    if (!peakFunction) {
      throw std::invalid_argument(
          "Cannot process null pointer poldi function.");
    }

    peakFunction->setDecoratedFunction(profileFunctionName);

    IPeakFunction_sptr wrappedProfile =
        boost::dynamic_pointer_cast<IPeakFunction>(
            peakFunction->getProfileFunction());

    if (wrappedProfile) {
      wrappedProfile->setCentre(peak->d());
      wrappedProfile->setFwhm(peak->fwhm(PoldiPeak::AbsoluteD));
      wrappedProfile->setIntensity(peak->intensity());
    }

    mdFunction->addFunction(peakFunction);
  }

  return mdFunction;
}

/**
 * Returns a Poldi2DFunction that encapsulates a PawleyFunction
 *
 * This function creates a PawleyFunction using the supplied profile function
 * name and the crystal system as well as initial cell from the input
 *properties
 * of the algorithm and wraps it in a Poldi2DFunction.
 *
 * Because the peak intensities are integral at this step but PawleyFunction
 * expects peak heights, a profile function is created and
 *setIntensity/height-
 * methods are used to convert.
 *
 * @param profileFunctionName :: Profile function name for PawleyFunction.
 * @param peakCollection :: Peak collection with peaks to be used in the fit.
 * @return :: A Poldi2DFunction with a PawleyFunction.
 */
Poldi2DFunction_sptr PoldiFitPeaks2D::getFunctionPawley(
    std::string profileFunctionName,
    const PoldiPeakCollection_sptr &peakCollection) const {
  Poldi2DFunction_sptr mdFunction(new Poldi2DFunction);

  boost::shared_ptr<PoldiSpectrumPawleyFunction> poldiPawleyFunction =
      boost::dynamic_pointer_cast<PoldiSpectrumPawleyFunction>(
          FunctionFactory::Instance().createFunction(
              "PoldiSpectrumPawleyFunction"));

  if (!poldiPawleyFunction) {
    throw std::invalid_argument("Could not create pawley function.");
  }

  poldiPawleyFunction->setDecoratedFunction("PawleyFunction");

  IPawleyFunction_sptr pawleyFunction =
      poldiPawleyFunction->getPawleyFunction();
  pawleyFunction->setProfileFunction(profileFunctionName);

  std::string crystalSystem = getProperty("CrystalSystem");
  pawleyFunction->setCrystalSystem(crystalSystem);

  std::string initialCell = getProperty("InitialCell");
  pawleyFunction->setUnitCell(initialCell);

  IPeakFunction_sptr pFun = boost::dynamic_pointer_cast<IPeakFunction>(
      FunctionFactory::Instance().createFunction(profileFunctionName));

  for (size_t i = 0; i < peakCollection->peakCount(); ++i) {
    PoldiPeak_sptr peak = peakCollection->peak(i);

    pFun->setCentre(peak->d());
    pFun->setFwhm(peak->fwhm(PoldiPeak::AbsoluteD));
    pFun->setIntensity(peak->intensity());

    pawleyFunction->addPeak(peak->hkl().asV3D(),
                            peak->fwhm(PoldiPeak::AbsoluteD), pFun->height());
  }

  pawleyFunction->fix(pawleyFunction->parameterIndex("f0.ZeroShift"));
  mdFunction->addFunction(poldiPawleyFunction);

  return mdFunction;
}

/**
 * Constructs a proper function from a peak collection
 *
 * This method constructs a Poldi2DFunction depending on whether or not a
 * Pawley fit is performed each peak contained in the peak collection.
 *
 * @param peakCollection :: PoldiPeakCollection containing peaks with integral
 *intensities
 * @return Poldi2DFunction with one PoldiSpectrumDomainFunction per peak
 */
Poldi2DFunction_sptr PoldiFitPeaks2D::getFunctionFromPeakCollection(
    const PoldiPeakCollection_sptr &peakCollection) const {
  std::string profileFunctionName = getProperty("PeakProfileFunction");

  bool pawleyFit = getProperty("PawleyFit");
  if (pawleyFit) {
    return getFunctionPawley(profileFunctionName, peakCollection);
  }

  return getFunctionIndividualPeaks(profileFunctionName, peakCollection);
}

/// Executes the algorithm
void PoldiFitPeaks2D::exec() {
  TableWorkspace_sptr peakTable = getProperty("PoldiPeakWorkspace");
  if (!peakTable) {
    throw std::runtime_error("Cannot proceed without peak workspace.");
  }

  MatrixWorkspace_sptr ws = getProperty("InputWorkspace");
  setDeltaTFromWorkspace(ws);

  setPoldiInstrument(boost::make_shared<PoldiInstrumentAdapter>(ws));
  setTimeTransformerFromInstrument(m_poldiInstrument);

  PoldiPeakCollection_sptr peakCollection = getPeakCollection(peakTable);

  Property *profileFunctionProperty =
      getPointerToProperty("PeakProfileFunction");
  if (!profileFunctionProperty->isDefault()) {
    peakCollection->setProfileFunctionName(profileFunctionProperty->value());
  }

  IAlgorithm_sptr fitAlgorithm = calculateSpectrum(peakCollection, ws);

  IFunction_sptr fitFunction = getFunction(fitAlgorithm);

  MatrixWorkspace_sptr outWs1D = get1DSpectrum(fitFunction, ws);

  PoldiPeakCollection_sptr normalizedPeaks =
      getPeakCollectionFromFunction(fitFunction);
  PoldiPeakCollection_sptr integralPeaks =
      getCountPeakCollection(normalizedPeaks);

  assignMillerIndices(peakCollection, integralPeaks);

  setProperty("OutputWorkspace", getWorkspace(fitAlgorithm));
  setProperty("RefinedPoldiPeakWorkspace", integralPeaks->asTableWorkspace());
  setProperty("Calculated1DSpectrum", outWs1D);

  bool isPawleyFit = getProperty("PawleyFit");
  if (isPawleyFit) {
    ITableWorkspace_sptr cellParameters = getRefinedCellParameters(fitFunction);

    if (cellParameters->rowCount() > 0) {
      setProperty("RefinedCellParameters",
                  getRefinedCellParameters(fitFunction));
    } else {
      g_log.warning() << "Warning: Cell parameter table is empty.";
    }
  }
}

/**
 * Adds background functions for the background if applicable
 *
 * If specified by the user via the corresponding algorithm parameters,
 * this function adds a constant and a linear background term to the
 * supplied Poldi2DFunction.
 *
 * @param poldi2DFunction :: Poldi2DFunction to which the background is added.
 */
void PoldiFitPeaks2D::addBackgroundTerms(Poldi2DFunction_sptr poldi2DFunction)
    const {
  bool addConstantBackground = getProperty("FitConstantBackground");
  if (addConstantBackground) {
    IFunction_sptr constantBackground =
        FunctionFactory::Instance().createFunction(
            "PoldiSpectrumConstantBackground");
    constantBackground->setParameter(
        0, getProperty("ConstantBackgroundParameter"));
    poldi2DFunction->addFunction(constantBackground);
  }

  bool addLinearBackground = getProperty("FitLinearBackground");
  if (addLinearBackground) {
    IFunction_sptr linearBackground =
        FunctionFactory::Instance().createFunction(
            "PoldiSpectrumLinearBackground");
    linearBackground->setParameter(0, getProperty("LinearBackgroundParameter"));
    poldi2DFunction->addFunction(linearBackground);
  }
}

/**
 * Performs the fit and returns the fit algorithm
 *
 * In this method the actual function fit/calculation is performed
 * using the Fit algorithm. After execution the algorithm is returned for
 * further processing.
 *
 * @param peakCollection :: PoldiPeakCollection
 * @param matrixWorkspace :: MatrixWorkspace with POLDI instrument and correct
 *dimensions
 * @return Instance of Fit-algorithm, after execution
 */
IAlgorithm_sptr PoldiFitPeaks2D::calculateSpectrum(
    const PoldiPeakCollection_sptr &peakCollection,
    const MatrixWorkspace_sptr &matrixWorkspace) {
  PoldiPeakCollection_sptr integratedPeaks =
      getIntegratedPeakCollection(peakCollection);
  PoldiPeakCollection_sptr normalizedPeakCollection =
      getNormalizedPeakCollection(integratedPeaks);

  Poldi2DFunction_sptr mdFunction =
      getFunctionFromPeakCollection(normalizedPeakCollection);

  addBackgroundTerms(mdFunction);

  IAlgorithm_sptr fit = createChildAlgorithm("Fit", -1, -1, true);

  if (!fit) {
    throw std::runtime_error("Could not initialize 'Fit'-algorithm.");
  }

  fit->setProperty("Function",
                   boost::dynamic_pointer_cast<IFunction>(mdFunction));
  fit->setProperty("InputWorkspace", matrixWorkspace);
  fit->setProperty("CreateOutput", true);

  int maxIterations = getProperty("MaximumIterations");
  fit->setProperty("MaxIterations", maxIterations);

  fit->setProperty("Minimizer", "Levenberg-MarquardtMD");

  // Setting the level to Notice to avoid problems with Levenberg-MarquardtMD.
  int oldLogLevel = g_log.getLevel();
  g_log.setLevelForAll(Poco::Message::PRIO_NOTICE);
  fit->execute();
  g_log.setLevelForAll(oldLogLevel);

  return fit;
}

/// Returns the output workspace stored in the Fit algorithm.
MatrixWorkspace_sptr
PoldiFitPeaks2D::getWorkspace(const IAlgorithm_sptr &fitAlgorithm) const {
  if (!fitAlgorithm) {
    throw std::invalid_argument(
        "Cannot extract workspace from null-algorithm.");
  }

  MatrixWorkspace_sptr outputWorkspace =
      fitAlgorithm->getProperty("OutputWorkspace");
  return outputWorkspace;
}

/// Extracts the fit function from the fit algorithm
IFunction_sptr
PoldiFitPeaks2D::getFunction(const IAlgorithm_sptr &fitAlgorithm) const {
  if (!fitAlgorithm) {
    throw std::invalid_argument("Cannot extract function from null-algorithm.");
  }

  IFunction_sptr fitFunction = fitAlgorithm->getProperty("Function");
  return fitFunction;
}

/**
 * Calculates the 1D diffractogram based on the supplied function
 *
 * This method takes a fit function and checks whether it implements the
 * IPoldiFunction1D interface. If that's the case, it calculates the
 * diffractogram based on the function.
 *
 * @param fitFunction :: IFunction that also implements IPoldiFunction1D.
 * @param workspace :: Workspace with POLDI raw data.
 * @return :: Q-based diffractogram.
 */
MatrixWorkspace_sptr PoldiFitPeaks2D::get1DSpectrum(
    const IFunction_sptr &fitFunction,
    const API::MatrixWorkspace_sptr &workspace) const {

  // Check whether the function is of correct type
  boost::shared_ptr<IPoldiFunction1D> poldiFunction =
      boost::dynamic_pointer_cast<IPoldiFunction1D>(fitFunction);

  if (!poldiFunction) {
    throw std::invalid_argument("Can only process Poldi2DFunctions.");
  }

  // And that we have an instrument available
  if (!m_poldiInstrument) {
    throw std::runtime_error("No POLDI instrument available.");
  }

  PoldiAbstractDetector_sptr detector(new PoldiDeadWireDecorator(
      workspace->getInstrument(), m_poldiInstrument->detector()));
  std::vector<int> indices = detector->availableElements();

  // Create the grid for the diffractogram and corresponding domain/values
  double lambdaMin = getProperty("LambdaMin");
  double lambdaMax = getProperty("LambdaMax");

  PoldiDGrid grid(detector, m_poldiInstrument->chopper(), m_deltaT,
                  std::make_pair(lambdaMin, lambdaMax));

  FunctionDomain1DVector domain(grid.grid());
  FunctionValues values(domain);

  // Calculate 1D function
  poldiFunction->poldiFunction1D(indices, domain, values);

  // Create and return Q-based workspace with spectrum
  return getQSpectrum(domain, values);
}

/// Takes a d-based domain and creates a Q-based MatrixWorkspace.
MatrixWorkspace_sptr
PoldiFitPeaks2D::getQSpectrum(const FunctionDomain1D &domain,
                              const FunctionValues &values) const {
  // Put result into workspace, based on Q
  MatrixWorkspace_sptr ws1D = WorkspaceFactory::Instance().create(
      "Workspace2D", 1, domain.size(), values.size());

  MantidVec &xData = ws1D->dataX(0);
  MantidVec &yData = ws1D->dataY(0);
  size_t offset = values.size() - 1;
  for (size_t i = 0; i < values.size(); ++i) {
    xData[offset - i] = Conversions::dToQ(domain[i]);
    yData[offset - i] = values[i];
  }

  ws1D->getAxis(0)->setUnit("MomentumTransfer");
  return ws1D;
}

void PoldiFitPeaks2D::setPoldiInstrument(
    const PoldiInstrumentAdapter_sptr &instrument) {
  m_poldiInstrument = instrument;
}

/**
 * Constructs a PoldiTimeTransformer from given instrument and calls
 *setTimeTransformer.
 *
 * @param poldiInstrument :: PoldiInstrumentAdapter with valid components
 */
void PoldiFitPeaks2D::setTimeTransformerFromInstrument(
    const PoldiInstrumentAdapter_sptr &poldiInstrument) {
  setTimeTransformer(boost::make_shared<PoldiTimeTransformer>(poldiInstrument));
}

/**
 * Sets the time transformer object that is used for all calculations.
 *
 * @param poldiTimeTransformer
 */
void PoldiFitPeaks2D::setTimeTransformer(
    const PoldiTimeTransformer_sptr &poldiTimeTransformer) {
  m_timeTransformer = poldiTimeTransformer;
}

/**
 * Extracts time bin width from workspace parameter
 *
 * The method uses the difference between first and second x-value of the
 *first
 *spectrum as
 * time bin width. If the workspace does not contain proper data (0 spectra or
 *less than
 * 2 x-values), the method throws an std::invalid_argument-exception.
 *Otherwise
 *it calls setDeltaT.
 *
 * @param matrixWorkspace :: MatrixWorkspace with at least one spectrum with
 *at
 *least two x-values.
 */
void PoldiFitPeaks2D::setDeltaTFromWorkspace(
    const MatrixWorkspace_sptr &matrixWorkspace) {
  if (matrixWorkspace->getNumberHistograms() < 1) {
    throw std::invalid_argument("MatrixWorkspace does not contain any data.");
  }

  MantidVec xData = matrixWorkspace->readX(0);

  if (xData.size() < 2) {
    throw std::invalid_argument(
        "Cannot process MatrixWorkspace with less than 2 x-values.");
  }

  // difference between first and second x-value is assumed to be the bin
  // width.
  setDeltaT(matrixWorkspace->readX(0)[1] - matrixWorkspace->readX(0)[0]);
}

/**
 * Assigns delta t, throws std::invalid_argument on invalid value (determined
 *by
 *isValidDeltaT).
 *
 * @param newDeltaT :: Value to be used as delta t for calculations.
 */
void PoldiFitPeaks2D::setDeltaT(double newDeltaT) {
  if (!isValidDeltaT(newDeltaT)) {
    throw std::invalid_argument("Time bin size must be larger than 0.");
  }

  m_deltaT = newDeltaT;
}

/**
 * Checks whether delta t is larger than 0.
 *
 * @param deltaT :: Value to be checked for validity as a time difference.
 * @return True if delta t is larger than 0, otherwise false.
 */
bool PoldiFitPeaks2D::isValidDeltaT(double deltaT) const {
  return deltaT > 0.0;
}

/**
 * Tries to construct a PoldiPeakCollection from the supplied table.
 *
 * @param peakTable :: TableWorkspace with POLDI peak data.
 * @return PoldiPeakCollection with the data from the table workspace.
 */
PoldiPeakCollection_sptr
PoldiFitPeaks2D::getPeakCollection(const TableWorkspace_sptr &peakTable) const {
  try {
    return boost::make_shared<PoldiPeakCollection>(peakTable);
  }
  catch (...) {
    throw std::runtime_error("Could not initialize peak collection.");
  }
}

/**
 * Return peak collection with integrated peaks
 *
 * This method takes a PoldiPeakCollection where the intensity is represented
 *by
 *the maximum. Then
 * it takes the profile function stored in the peak collection, which must be
 *the name of a registered
 * IPeakFunction-implementation. The parameters height and fwhm are assigned,
 *centre is set to 0 to
 * avoid problems with the parameter transformation for the integration from
 *-inf to inf. The profiles are
 * integrated using a PeakFunctionIntegrator to the precision of 1e-10.
 *
 * The original peak collection is not modified, a new instance is created.
 *
 * @param rawPeakCollection :: PoldiPeakCollection
 * @return PoldiPeakCollection with integrated intensities
 */
PoldiPeakCollection_sptr PoldiFitPeaks2D::getIntegratedPeakCollection(
    const PoldiPeakCollection_sptr &rawPeakCollection) const {
  if (!rawPeakCollection) {
    throw std::invalid_argument(
        "Cannot proceed with invalid PoldiPeakCollection.");
  }

  if (!isValidDeltaT(m_deltaT)) {
    throw std::invalid_argument("Cannot proceed with invalid time bin size.");
  }

  if (!m_timeTransformer) {
    throw std::invalid_argument(
        "Cannot proceed with invalid PoldiTimeTransformer.");
  }

  if (rawPeakCollection->intensityType() == PoldiPeakCollection::Integral) {
    /* Intensities are integral already - don't need to do anything,
     * except cloning the collection, to make behavior consistent, since
     * integrating also results in a new peak collection.
     */
    return rawPeakCollection->clone();
  }

  /* If no profile function is specified, it's not possible to get integrated
   * intensities at all and we try to use the one specified by the user
   * instead.
   */
  std::string profileFunctionName = rawPeakCollection->getProfileFunctionName();

  if (!rawPeakCollection->hasProfileFunctionName()) {
    profileFunctionName = getPropertyValue("PeakProfileFunction");
  }

  std::vector<std::string> allowedProfiles =
      FunctionFactory::Instance().getFunctionNames<IPeakFunction>();

  if (std::find(allowedProfiles.begin(), allowedProfiles.end(),
                profileFunctionName) == allowedProfiles.end()) {
    throw std::runtime_error(
        "Cannot integrate peak profiles with invalid profile function.");
  }

  PoldiPeakCollection_sptr integratedPeakCollection =
      boost::make_shared<PoldiPeakCollection>(PoldiPeakCollection::Integral);
  integratedPeakCollection->setProfileFunctionName(profileFunctionName);

  for (size_t i = 0; i < rawPeakCollection->peakCount(); ++i) {
    PoldiPeak_sptr peak = rawPeakCollection->peak(i);

    IPeakFunction_sptr profileFunction =
        boost::dynamic_pointer_cast<IPeakFunction>(
            FunctionFactory::Instance().createFunction(profileFunctionName));

    profileFunction->setHeight(peak->intensity());
    profileFunction->setFwhm(peak->fwhm(PoldiPeak::AbsoluteD));

    PoldiPeak_sptr integratedPeak = peak->clone();
    integratedPeak->setIntensity(UncertainValue(profileFunction->intensity()));
    integratedPeakCollection->addPeak(integratedPeak);
  }

  return integratedPeakCollection;
}

/**
 * Normalized the intensities of the given integrated peaks
 *
 * This function normalizes the peak intensities according to the source
 *spectrum, the number of
 * chopper slits and the number of detector elements.
 *
 * @param peakCollection :: PoldiPeakCollection with integrated intensities
 * @return PoldiPeakCollection with normalized intensities
 */
PoldiPeakCollection_sptr PoldiFitPeaks2D::getNormalizedPeakCollection(
    const PoldiPeakCollection_sptr &peakCollection) const {
  if (!peakCollection) {
    throw std::invalid_argument(
        "Cannot proceed with invalid PoldiPeakCollection.");
  }

  if (!m_timeTransformer) {
    throw std::invalid_argument("Cannot proceed without PoldiTimeTransformer.");
  }

  PoldiPeakCollection_sptr normalizedPeakCollection =
      boost::make_shared<PoldiPeakCollection>(PoldiPeakCollection::Integral);
  normalizedPeakCollection->setProfileFunctionName(
      peakCollection->getProfileFunctionName());

  for (size_t i = 0; i < peakCollection->peakCount(); ++i) {
    PoldiPeak_sptr peak = peakCollection->peak(i);
    double calculatedIntensity =
        m_timeTransformer->calculatedTotalIntensity(peak->d());

    PoldiPeak_sptr normalizedPeak = peak->clone();
    normalizedPeak->setIntensity(peak->intensity() / calculatedIntensity);
    normalizedPeakCollection->addPeak(normalizedPeak);
  }

  return normalizedPeakCollection;
}

/**
 * Converts normalized peak intensities to count based integral intensities
 *
 * This operation is the opposite of getNormalizedPeakCollection and is used
 *to
 *convert
 * the intensities back to integral intensities.
 *
 * @param peakCollection :: PoldiPeakCollection with normalized intensities
 * @return PoldiPeakCollection with integral intensities
 */
PoldiPeakCollection_sptr PoldiFitPeaks2D::getCountPeakCollection(
    const PoldiPeakCollection_sptr &peakCollection) const {
  if (!peakCollection) {
    throw std::invalid_argument(
        "Cannot proceed with invalid PoldiPeakCollection.");
  }

  if (!m_timeTransformer) {
    throw std::invalid_argument("Cannot proceed without PoldiTimeTransformer.");
  }

  PoldiPeakCollection_sptr countPeakCollection =
      boost::make_shared<PoldiPeakCollection>(PoldiPeakCollection::Integral);
  countPeakCollection->setProfileFunctionName(
      peakCollection->getProfileFunctionName());

  for (size_t i = 0; i < peakCollection->peakCount(); ++i) {
    PoldiPeak_sptr peak = peakCollection->peak(i);
    double calculatedIntensity =
        m_timeTransformer->calculatedTotalIntensity(peak->d());

    PoldiPeak_sptr countPeak = peak->clone();
    countPeak->setIntensity(peak->intensity() * calculatedIntensity);

    countPeakCollection->addPeak(countPeak);
  }

  return countPeakCollection;
}

/// Assign Miller indices from one peak collection to another.
void PoldiFitPeaks2D::assignMillerIndices(const PoldiPeakCollection_sptr &from,
                                          PoldiPeakCollection_sptr &to) const {
  if (!from || !to) {
    throw std::invalid_argument("Cannot process invalid peak collections.");
  }

  if (from->peakCount() != to->peakCount()) {
    throw std::runtime_error(
        "Cannot assign indices if number of peaks does not match.");
  }

  for (size_t i = 0; i < from->peakCount(); ++i) {
    PoldiPeak_sptr fromPeak = from->peak(i);
    PoldiPeak_sptr toPeak = to->peak(i);

    toPeak->setHKL(fromPeak->hkl());
  }
}

} // namespace Poldi
} // namespace Mantid
