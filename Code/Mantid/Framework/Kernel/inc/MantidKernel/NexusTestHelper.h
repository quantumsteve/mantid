#ifndef MANTID_KERNEL_NEXUSTESTHELPER_H_
#define MANTID_KERNEL_NEXUSTESTHELPER_H_
    
#include "MantidKernel/System.h"
#include "nexus/NeXusFile.hpp"


namespace Mantid
{
namespace Kernel
{

  /** A Helper class for easily writing nexus saving/loading tests.
    
    @author Janik Zikovsky
    @date 2011-09-07

    Copyright &copy; 2011 ISIS Rutherford Appleton Laboratory & NScD Oak Ridge National Laboratory

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
  class DLLExport NexusTestHelper 
  {
  public:
    NexusTestHelper(bool deleteFile = true);
    virtual ~NexusTestHelper();

    void createFile(std::string barefilename);
    void reopenFile();

    /// Nexus file handle
    ::NeXus::File * file;

    /// Created filename (full path)
    std::string filename;

    /// Do you delete when finished?
    bool deleteFile;
  };


} // namespace Kernel
} // namespace Mantid

#endif  /* MANTID_KERNEL_NEXUSTESTHELPER_H_ */
