#include "MantidVatesAPI/vtkMD0DFactory.h"
#include "MantidAPI/IMDWorkspace.h"
#include "MantidVatesAPI/vtkNullUnstructuredGrid.h"
#include "MantidVatesAPI/ProgressAction.h"
#include "MantidKernel/Logger.h"

using namespace Mantid::API;

namespace
{
  Mantid::Kernel::Logger g_log("vtkMD0DFactory");
}

namespace Mantid
{
  namespace VATES
  {
    /**
    Constructor
    @param thresholdRange : Thresholding range functor
    @param scalarName : Name to give to signal
    */
    vtkMD0DFactory::vtkMD0DFactory(ThresholdRange_scptr thresholdRange, const std::string& scalarName) : m_thresholdRange(thresholdRange), m_scalarName(scalarName)
    {
    }

    /// Destructor
    vtkMD0DFactory::~vtkMD0DFactory()
    {
    }

    /**
    Create the vtkStructuredGrid from the provided workspace
    @param progressUpdating: Reporting object to pass progress information up the stack.
    @return fully constructed vtkDataSet.
    */
    vtkDataSet* vtkMD0DFactory::create(ProgressAction& progressUpdating) const
    {
      g_log.warning() << "Factory " << this->getFactoryTypeName() << " is being used. You are viewing data with less than three dimensions in the VSI. \n";
      (void) progressUpdating;
      vtkNullUnstructuredGrid nullGrid;
      vtkUnstructuredGrid *visualDataSet = nullGrid.createNullData();
      return visualDataSet;
    }

    /// Initalize with a target workspace.
    void vtkMD0DFactory::initialize(Mantid::API::Workspace_sptr ws)
    {
    }

   /// Validate the workspace 
   void vtkMD0DFactory::validate() const
   {
   }
  }
}
