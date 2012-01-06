/*WIKI* 
 The fluctuation dissipation theorem [1,2] relates the dynamic susceptibility to the scattering function

 <math>\left(1-e^{-\frac{E}{k_B T}}\right) S(\mathbf{q}, E) = \frac{1}{\pi} \chi'' (\mathbf{q}, E) </math>

 where <math>E</math> is the energy transfer to the system. The algorithm assumes that the y axis of the
 input workspace contains the scattering function <math>S</math>. The y axis of the output workspace will
 contain the dynamic susceptibility. The temperature is extracted from a log attached to the workspace, as the mean
 value. Alternatively, the temperature can be directly specified. The algorithm will fail if neither option is
 valid.

 [1] S. W. Lovesey - Theory of Neutron Scattering from Condensed Matter, vol 1
 [2] I. A. Zaliznyak and S. H. Lee - Magnetic Neutron Scattering in "Modern techniques for characterizing magnetic materials"

*WIKI*/
#include "MantidAlgorithms/ApplyDetailedBalance.h"
#include "MantidKernel/System.h"
#include "MantidKernel/TimeSeriesProperty.h"
#include "MantidKernel/PropertyWithValue.h"
#include "boost/lexical_cast.hpp"
#include <iostream>
#include <cmath>
#include "MantidAPI/WorkspaceValidators.h"
#include "MantidKernel/PhysicalConstants.h"

using std::string;
using namespace Mantid::Kernel;
using namespace Mantid::API;

namespace Mantid
{
namespace Algorithms
{

  // Register the algorithm into the AlgorithmFactory
  DECLARE_ALGORITHM(ApplyDetailedBalance)
  


  //----------------------------------------------------------------------------------------------
  /** Constructor
   */
  ApplyDetailedBalance::ApplyDetailedBalance()
  {
  }
    
  //----------------------------------------------------------------------------------------------
  /** Destructor
   */
  ApplyDetailedBalance::~ApplyDetailedBalance()
  {
  }
  

  //----------------------------------------------------------------------------------------------
  /// Sets documentation strings for this algorithm
  void ApplyDetailedBalance::initDocs()
  {
    this->setWikiSummary("Transform scattering intensity to dynamic susceptibility.");
    this->setOptionalMessage("Transform scattering intensity to dynamic susceptibility.");
  }

  //----------------------------------------------------------------------------------------------
  /** Initialize the algorithm's properties.
   */
  void ApplyDetailedBalance::init()
  {
    CompositeWorkspaceValidator<> *wsValidator = new CompositeWorkspaceValidator<>;
    wsValidator->add(new WorkspaceUnitValidator<>("DeltaE"));
    declareProperty(new WorkspaceProperty<>("InputWorkspace","",Direction::Input,wsValidator), "An input workspace.");
    declareProperty(new WorkspaceProperty<>("OutputWorkspace","",Direction::Output), "An output workspace.");
    declareProperty(new PropertyWithValue<string>("Temperature","",Direction::Input),"SampleLog variable name that contains the temperature, or a number");
  }

  //----------------------------------------------------------------------------------------------
  /** Execute the algorithm.
   */
  void ApplyDetailedBalance::exec()
  {
     MatrixWorkspace_sptr inputWS = getProperty("InputWorkspace");
     MatrixWorkspace_sptr outputWS = getProperty("OutputWorkspace");
     // If input and output workspaces are not the same, create a new workspace for the output
     if (outputWS != inputWS)
     {
       outputWS = API::WorkspaceFactory::Instance().create(inputWS);
     }

     std::string Tstring=getProperty("Temperature");
     double Temp;
     try
     {
        if (inputWS->run().hasProperty(Tstring))
          Temp=(dynamic_cast<Kernel::TimeSeriesProperty<double> *>(inputWS->run().getProperty(Tstring)))->getStatistics().mean;
        else
          Temp=boost::lexical_cast<double>(Tstring);
     }
     catch (...)
     {
       Tstring+= " is not a valid log, nor is it a number";
       throw std::invalid_argument(Tstring);
     }

     double oneOverT=PhysicalConstants::meVtoKelvin/Temp;
     // Run the exponential correction algorithm explicitly to enable progress reporting
     IAlgorithm_sptr expcor = createSubAlgorithm("OneMinusExponentialCor",0.0,1.0);
     expcor->setProperty<MatrixWorkspace_sptr>("InputWorkspace", inputWS);
     expcor->setProperty<MatrixWorkspace_sptr>("OutputWorkspace", outputWS);
     expcor->setProperty<double>("C1", M_PI);
     expcor->setProperty<double>("C", oneOverT);
     expcor->setPropertyValue("Operation","Multiply");
     expcor->executeAsSubAlg();
     // Get back the result
     outputWS = expcor->getProperty("OutputWorkspace");

     setProperty("OutputWorkspace",outputWS);
  }



} // namespace Mantid
} // namespace Algorithms

