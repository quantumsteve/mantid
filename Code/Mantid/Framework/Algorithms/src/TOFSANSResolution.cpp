//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "MantidAlgorithms/TOFSANSResolution.h"
#include "MantidAPI/WorkspaceValidators.h"
#include "MantidDataObjects/EventWorkspace.h"
#include "MantidDataObjects/EventList.h"
#include "MantidKernel/RebinParamsValidator.h"
#include "MantidKernel/ArrayProperty.h"
#include "MantidKernel/VectorHelper.h"

namespace Mantid
{
namespace Algorithms
{

// Register the algorithm into the AlgorithmFactory
DECLARE_ALGORITHM(TOFSANSResolution)

/// Sets documentation strings for this algorithm
void TOFSANSResolution::initDocs()
{
  this->setWikiSummary("Calculate the Q resolution for TOF SANS data.");
  this->setOptionalMessage("Calculate the Q resolution for TOF SANS data.");
}


using namespace Kernel;
using namespace API;
using namespace Geometry;
using namespace DataObjects;

void TOFSANSResolution::init()
{
  CompositeValidator<> *iqValidator = new CompositeValidator<>;
  iqValidator->add(new WorkspaceUnitValidator<>("MomentumTransfer"));
  iqValidator->add(new EventWorkspaceValidator<>(false));

  declareProperty(new WorkspaceProperty<>("InputWorkspace","",Direction::InOut, iqValidator),
      "Name the workspace to calculate the resolution for");

  CompositeValidator<> *wsValidator = new CompositeValidator<>;
  wsValidator->add(new WorkspaceUnitValidator<>("Wavelength"));
  declareProperty(new WorkspaceProperty<>("ReducedWorkspace","",Direction::Input, wsValidator),
      "I(Q) workspace");
  declareProperty(new ArrayProperty<double>("OutputBinning", new RebinParamsValidator));

  declareProperty("MinWavelength", EMPTY_DBL(), "Minimum wavelength to use.");
  declareProperty("MaxWavelength", EMPTY_DBL(), "Maximum wavelength to use.");

  BoundedValidator<double> *positiveDouble = new BoundedValidator<double>();
  positiveDouble->setLower(0);
  declareProperty("PixelSizeX", 5.15, positiveDouble,
      "Pixel size in the X direction (mm).");
  declareProperty("PixelSizeY", 5.15, positiveDouble->clone(),
      "Pixel size in the Y direction (mm).");
  declareProperty("SampleApertureRadius", 10.0, positiveDouble->clone(),
      "Sample aperture radius (mm).");
  declareProperty("SourceApertureRadius", 20.0, positiveDouble->clone(),
      "Source aperture radius (mm).");
  declareProperty("DeltaT", 250.0, positiveDouble->clone(),
      "TOF spread (microsec).");
}

void TOFSANSResolution::exec()
{
  MatrixWorkspace_sptr iqWS = getProperty("InputWorkspace");
  MatrixWorkspace_sptr reducedWS = getProperty("ReducedWorkspace");
  EventWorkspace_sptr reducedEventWS = boost::dynamic_pointer_cast<EventWorkspace>(reducedWS);
  const double min_wl = getProperty("MinWavelength");
  const double max_wl = getProperty("MaxWavelength");
  double pixel_size_x = getProperty("PixelSizeX");
  double pixel_size_y = getProperty("PixelSizeY");
  double R1 = getProperty("SourceApertureRadius");
  double R2 = getProperty("SampleApertureRadius");
  // Convert to meters
  pixel_size_x /= 1000.0;
  pixel_size_y /= 1000.0;
  R1 /= 1000.0;
  R2 /= 1000.0;
  double deltaT = getProperty("DeltaT");

  if (!reducedEventWS)
  {
    g_log.warning() << "An Event Workspace is needed to compute dQ. Calculation skipped." << std::endl;
    return;
  }

  // Calculate the output binning
  const std::vector<double> binParams = getProperty("OutputBinning");

  // Count histogram for normalization
  const int xLength = static_cast<int>(iqWS->readX(0).size());
  std::vector<double> XNorm(xLength-1, 0.0);

  // Create workspaces with each component of the resolution for debugging purposes
  MatrixWorkspace_sptr thetaWS = WorkspaceFactory::Instance().create(iqWS);
  declareProperty(new WorkspaceProperty<>("ThetaError","",Direction::Output));
  setPropertyValue("ThetaError","__"+iqWS->getName()+"_theta_error");
  setProperty("ThetaError",thetaWS);
  thetaWS->setX(0,iqWS->readX(0));
  MantidVec& ThetaY = thetaWS->dataY(0);

  MatrixWorkspace_sptr tofWS = WorkspaceFactory::Instance().create(iqWS);
  declareProperty(new WorkspaceProperty<>("TOFError","",Direction::Output));
  setPropertyValue("TOFError","__"+iqWS->getName()+"_tof_error");
  setProperty("TOFError",tofWS);
  tofWS->setX(0,iqWS->readX(0));
  MantidVec& TOFY = tofWS->dataY(0);

  // Initialize Dq
  MantidVec& DxOut = iqWS->dataDx(0);
  for ( int i = 0; i<xLength-1; i++ ) DxOut[i] = 0.0;

  const V3D samplePos = reducedWS->getInstrument()->getSample()->getPos();
  const V3D sourcePos = reducedWS->getInstrument()->getSource()->getPos();
  const V3D SSD = samplePos - sourcePos;
  const double L1 = SSD.norm();

  const int numberOfSpectra = static_cast<int>(reducedWS->getNumberHistograms());
  Progress progress(this,0.0,1.0,numberOfSpectra);

  PARALLEL_FOR2(reducedEventWS, iqWS)
  for (int i = 0; i < numberOfSpectra; i++)
  {
    PARALLEL_START_INTERUPT_REGION
    IDetector_const_sptr det;
    try {
      det = reducedEventWS->getDetector(i);
    } catch (Exception::NotFoundError&) {
      g_log.warning() << "Spectrum index " << i << " has no detector assigned to it - discarding" << std::endl;
      // Catch if no detector. Next line tests whether this happened - test placed
      // outside here because Mac Intel compiler doesn't like 'continue' in a catch
      // in an openmp block.
    }
    // If no detector found or if it's masked or a monitor, skip onto the next spectrum
    if ( !det || det->isMonitor() || det->isMasked() ) continue;

    // Get the flight path from the sample to the detector pixel
    const V3D scattered_flight_path = det->getPos() - samplePos;

    // Multiplicative factor to go from lambda to Q
    // Don't get fooled by the function name...
    const double theta = reducedEventWS->detectorTwoTheta(det);
    const double factor = 4.0 * M_PI * sin( theta/2.0 );

    EventList& el = reducedEventWS->getEventList(i);
    el.switchTo(WEIGHTED);

    std::vector<WeightedEvent>::iterator itev;
    std::vector<WeightedEvent>::iterator itev_end = el.getWeightedEvents().end();

    for (itev = el.getWeightedEvents().begin(); itev != itev_end; itev++)
    {
      if ( itev->m_weight != itev->m_weight ) continue;
      if (std::abs(itev->m_weight) == std::numeric_limits<double>::infinity()) continue;
      if ( !isEmpty(min_wl) && itev->m_tof < min_wl ) continue;
      if ( !isEmpty(max_wl) && itev->m_tof > max_wl ) continue;

      const double q = factor/itev->m_tof;
      int iq = 0;

      // Bin assignment depends on whether we have log or linear bins
      if(binParams[1]>0.0)
      {
        iq = (int)floor( (q-binParams[0])/ binParams[1] );
      } else {
        iq = (int)floor(log(q/binParams[0])/log(1.0-binParams[1]));
      }

      const double L2 = scattered_flight_path.norm();
      const double src_to_pixel = L1+L2;
      const double dTheta2 = ( 3.0*R1*R1/(L1*L1) + 3.0*R2*R2*src_to_pixel*src_to_pixel/(L1*L1*L2*L2)
            + 2.0*(pixel_size_x*pixel_size_x+pixel_size_y*pixel_size_y)/(L2*L2) )/12.0;

      const double dwl_over_wl = 3.9560*deltaT/(1000.0*(L1+L2)*itev->m_tof);
      const double dq_over_q = std::sqrt(dTheta2/(theta*theta)+dwl_over_wl*dwl_over_wl);

      PARALLEL_CRITICAL(iq)    /* Write to shared memory - must protect */
      if (iq>=0 && iq < xLength-1 && !dq_over_q!=dq_over_q && dq_over_q>0)
      {
        DxOut[iq] += q*dq_over_q*itev->m_weight;
        XNorm[iq] += itev->m_weight;
        TOFY[iq] += q*std::fabs(dwl_over_wl)*itev->m_weight;
        ThetaY[iq] += q*std::sqrt(dTheta2)/theta*itev->m_weight;
      }
    }

    progress.report("Computing Q resolution");
    PARALLEL_END_INTERUPT_REGION
  }
  PARALLEL_CHECK_INTERUPT_REGION
  // Normalize according to the chosen weighting scheme
  for ( int i = 0; i<xLength-1; i++ )
  {
    if (XNorm[i]>0)
    {
      DxOut[i] /= XNorm[i];
      TOFY[i] /= XNorm[i];
      ThetaY[i] /= XNorm[i];
    }
  }
}
} // namespace Algorithms
} // namespace Mantid

