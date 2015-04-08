#ifndef MANTID_VATES_CONCRETE_PEAKS_PRESENTER_VSI_H
#define MANTID_VATES_CONCRETE_PEAKS_PRESENTER_VSI_H

#include "MantidKernel/System.h"
#include "MantidVatesAPI/PeaksPresenterVsi.h"
#include "MantidAPI/PeakTransform.h"
#include "MantidAPI/IPeaksWorkspace.h"
#include "MantidVatesAPI/ViewFrustum.h"
#include "MantidGeometry/Crystal/PeakShape.h"
#include <vector>


namespace Mantid
{
namespace VATES
{
class DLLExport ConcretePeaksPresenterVsi : public PeaksPresenterVsi
{
  public:
    ConcretePeaksPresenterVsi(Mantid::API::IPeaksWorkspace_sptr peaksWorkspace, ViewFrustum frustum, std::string wsFrame);
    ~ConcretePeaksPresenterVsi();
    virtual Mantid::API::IPeaksWorkspace_sptr getPeaksWorkspace();
    virtual std::vector<bool> getViewablePeaks();
    virtual void updateViewFrustum(ViewFrustum frustum);
    virtual std::string getFrame();
    virtual std::string getPeaksWorkspaceName();
    virtual void getPeaksInfo(Mantid::API::IPeaksWorkspace_sptr peaksWorkspace, int row, Mantid::Kernel::V3D& position, double& radius);
  private:
    /// Get the max radius.
    double getMaxRadius(Mantid::Geometry::PeakShape_sptr shape);
    /// Viewable Peaks
    std::vector<bool> m_viewablePeaks;
    /// The viewable region
    ViewFrustum m_viewableRegion;
    /// The peaks workspace
    Mantid::API::IPeaksWorkspace_sptr m_peaksWorkspace;
    /// The frame
    std::string m_frame;
};
}
}
#endif