#ifndef MANTID_VATES_NULL_PEAKS_PRESENTER
#define MANTID_VATES_NULL_PEAKS_PRESENTER

#include "MantidKernel/System.h"
#include "MantidVatesAPI/PeaksPresenterVsi.h"
#include "MantidAPI/IPeaksWorkspace.h"
#include "MantidVatesAPI/ViewFrustum.h"
#include <vector>

namespace Mantid
{
namespace VATES
{
  class DLLExport NullPeaksPresenterVsi : public PeaksPresenterVsi
  {
    public:
    NullPeaksPresenterVsi(){}
    virtual ~NullPeaksPresenterVsi(){}
    virtual Mantid::API::IPeaksWorkspace_sptr getPeaksWorkspace(){throw std::runtime_error("NullPeaksPresenterVsi does not implement this method. Misused");}
    virtual std::vector<bool> getViewablePeaks() {throw std::runtime_error("NullPeaksPresenterVsi does not implement this method. Misused");}
    virtual void updateViewFrustum(ViewFrustum ) {}
    virtual std::string getFrame(){throw std::runtime_error("NullPeaksPresenterVsi does not implement this method. Misused");}
    virtual std::string getPeaksWorkspaceName(){throw std::runtime_error("NullPeaksPresenterVsi does not implement this method. Misused");}
    virtual void getPeaksInfo(Mantid::API::IPeaksWorkspace_sptr , int ,
                              Mantid::Kernel::V3D& , double& ){throw std::runtime_error("NullPeaksPresenterVsi does not implement this method. Misused");}
  };
}
}

#endif