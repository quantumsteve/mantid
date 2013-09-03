#include "MantidRemoteAlgorithms/QueryRemoteJob.h"
#include "MantidKernel/MandatoryValidator.h"
#include "MantidKernel/NullValidator.h"
#include "MantidKernel/FacilityInfo.h"
#include "MantidKernel/ListValidator.h"
#include "MantidRemote/SimpleJSON.h"
#include "MantidKernel/RemoteJobManager.h"

#include "boost/make_shared.hpp"

namespace Mantid
{
namespace RemoteAlgorithms
{
    
// Register the algorithm into the AlgorithmFactory
DECLARE_ALGORITHM(QueryRemoteJob)

using namespace Mantid::Kernel;
using namespace Mantid::API;
using namespace Mantid::Geometry;

// A reference to the logger is provided by the base class, it is called g_log.
// It is used to print out information, warning and error messages

void QueryRemoteJob::init()
{
  // Unlike most algorithms, this one doesn't deal with workspaces....

  auto requireValue = boost::make_shared<MandatoryValidator<std::string> >();
  auto nullValidator = boost::make_shared<NullValidator>();

  // Compute Resources
  std::vector<std::string> computes = Mantid::Kernel::ConfigService::Instance().getFacility().computeResources();
  declareProperty( "ComputeResource", "", boost::make_shared<StringListValidator>(computes), "", Direction::Input);

  // The ID of the job we want to query
  declareProperty( "JobID", "", requireValue, "", Direction::Input);

  // Name given to the job
  declareProperty( "JobName", "", nullValidator, "",  Direction::Output);

  // Name of the python script that was (or will be) run
  declareProperty( "ScriptName", "", nullValidator, "",  Direction::Output);

  // A human readable description of the job's status
  declareProperty( "JobStatusString", "", nullValidator, "",  Direction::Output);

  // Transaction ID this job is associated with
  declareProperty( "TransID", "", nullValidator, "",  Direction::Output);

}

void QueryRemoteJob::exec()
{
  boost::shared_ptr<RemoteJobManager> jobManager = Mantid::Kernel::ConfigService::Instance().getFacility().getRemoteJobManager( getPropertyValue("ComputeResource"));

  // jobManager is a boost::shared_ptr...
  if (! jobManager)
  {
    // Requested compute resource doesn't exist
    // TODO: should we create our own exception class for this??
    throw( std::runtime_error( std::string("Unable to create a compute resource named " + getPropertyValue("ComputeResource"))));
  }

  std::istream &respStream = jobManager->httpGet("/query", std::string("JobID=") + getPropertyValue("JobID"));
  JSONObject resp;
  initFromStream( resp, respStream);
  if (jobManager->lastStatus() == Poco::Net::HTTPResponse::HTTP_OK)
  {
    setProperty( "JobStatusString", resp["JobStatus"]);
    setProperty( "JobName", resp["JobName"]);
    setProperty( "ScriptName", resp["ScriptName"]);
    setProperty( "TransID", resp["TransID"]);
  }
  else
  {
    std::string errMsg;
    resp["Err_Msg"].getValue( errMsg);
    throw( std::runtime_error( errMsg));
  }
}


} // end namespace RemoteAlgorithms
} // end namespace Mantid
