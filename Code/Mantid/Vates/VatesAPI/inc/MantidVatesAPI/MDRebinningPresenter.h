#ifndef MANTID_VATES_MD_REBINNING_PRESENTER
#define MANTID_VATES_MD_REBINNING_PRESENTER

#include "MantidKernel/System.h"
#include <vector>

class vtkUnstructuredGrid;
namespace Mantid
{
  namespace VATES
  {
    class ProgressAction;
    class vtkDataSetFactory;
     /** 
    @class MDRebinningPresenter, Abstract presenters for multi-dimensional rebinning of various types.
    @author Owen Arnold, Tessella plc
    @date 03/06/2011

    Copyright &copy; 2010 ISIS Rutherford Appleton Laboratory & NScD Oak Ridge National Laboratory

    This file is part of Mantid.

    Mantid is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Mantid is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    File change history is stored at: <https://svn.mantidproject.org/mantid/trunk/Code/Mantid>
    Code Documentation is available at: <http://doxygen.mantidproject.org>
    */
    class DLLExport MDRebinningPresenter
    {
    public:
      virtual void updateModel() = 0;
      virtual vtkUnstructuredGrid* execute(vtkDataSetFactory* factory, ProgressAction& eventHandler) = 0;
      virtual const std::string& getAppliedGeometryXML() const = 0;
      virtual bool hasTDimensionAvailable() const = 0;
      virtual std::vector<double> getTimeStepValues() const = 0;
      virtual ~MDRebinningPresenter(){}
    };

  }
}

#endif