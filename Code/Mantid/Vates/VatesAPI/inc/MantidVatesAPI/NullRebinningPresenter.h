#ifndef MANTID_VATES_NULL_REBINNING_PRESENTER
#define MANTID_VATES_NULL_REBINNING_PRESENTER
#include "MantidVatesAPI/MDRebinningPresenter.h"
namespace Mantid
{
  namespace VATES
  {
    class DLLExport NullRebinningPresenter : public MDRebinningPresenter
    {
    public:

      NullRebinningPresenter();

      virtual void updateModel();

      virtual vtkUnstructuredGrid* execute(vtkDataSetFactory*, ProgressAction&);

      virtual const std::string& getAppliedGeometryXML() const;

      virtual std::vector<double> getTimeStepValues() const;

      virtual bool hasTDimensionAvailable() const;

      virtual ~NullRebinningPresenter();

    private:

      NullRebinningPresenter(const NullRebinningPresenter&);

      NullRebinningPresenter& operator=(const NullRebinningPresenter&);

    };
  }
}

#endif