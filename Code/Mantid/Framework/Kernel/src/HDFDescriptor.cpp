#include "MantidKernel/HDFDescriptor.h"
#include "MantidKernel/Exception.h"

#include <nexus/NeXusFile.hpp>
#include <nexus/NeXusException.hpp>

#include <Poco/File.h>
#include <Poco/Path.h>

#include <cstring>

namespace Mantid
{
  namespace Kernel
  {
    //---------------------------------------------------------------------------------------------------------------------------
    // static HDFDescriptor constants
    //---------------------------------------------------------------------------------------------------------------------------
    /// Size of HDF magic number
    const size_t HDFDescriptor::HDFMagicSize = 4;
    /// HDF cookie that is stored in the first 4 bytes of the file.
    const unsigned char HDFDescriptor::HDFMagic[4] = {'\016','\003','\023','\001'}; // From HDF4::hfile.h

    /// Size of HDF5 signature
    size_t HDFDescriptor::HDF5SignatureSize = 8;
    /// signature identifying a HDF5 file.
    const unsigned char HDFDescriptor::HDF5Signature[8] = { 137, 'H', 'D', 'F', '\r', '\n', '\032', '\n' };

    namespace
    {
      //---------------------------------------------------------------------------------------------------------------------------
      // Anonymous helper methods to use isHDF methods to use an open file handle
      //---------------------------------------------------------------------------------------------------------------------------

      /**
       * Currently simply checks for the HDF signatures and returns true if one of them is found
       * @param fileHandle A file handled opened and pointing at the start of the file. On return the
       * fileHandle is left at the start of the file
       * @param version One of the HDFDescriptor::Version enumerations specifying the required version
       * @return True if the file is considered hierarchical, false otherwise
       */
      bool isHDFHandle(FILE *fileHandle, HDFDescriptor::Version version)
      {
        if(!fileHandle) throw std::invalid_argument("HierarchicalFileDescriptor::isHierarchical - Invalid file handle");

        bool result(false);

        // HDF4 check requires 4 bytes,  HDF5 check requires 8 bytes
        // Use same buffer and waste a few bytes if only checking HDF4
        unsigned char buffer[8] = {'0','0','0','0','0','0','0','0'};
        std::fread(static_cast<void*>(&buffer), sizeof(unsigned char), HDFDescriptor::HDF5SignatureSize, fileHandle);
        // Number of bytes read doesn't matter as if it is not enough then the memory simply won't match
        // as the buffer has been "zeroed"
        if(version == HDFDescriptor::Version5 || version == HDFDescriptor::AnyVersion )
        {
          result = (std::memcmp(&buffer, &HDFDescriptor::HDF5Signature, HDFDescriptor::HDF5SignatureSize) == 0);
        }
        if(!result && (version == HDFDescriptor::Version4 || version == HDFDescriptor::AnyVersion) )
        {
          result = (std::memcmp(&buffer, &HDFDescriptor::HDFMagic, HDFDescriptor::HDFMagicSize) == 0);
        }

        // Return file stream to start of file
        std::rewind(fileHandle);
        return result;
      }
    }

    //---------------------------------------------------------------------------------------------------------------------------
    // static HDFDescriptor methods
    //---------------------------------------------------------------------------------------------------------------------------

    /**
     * Checks for the HDF signatures and returns true if one of them is found
     * @param filename A string filename to check
     * @param version One of the HDFDescriptor::Version enumerations specifying the required version
     * @return True if the file is considered hierarchical, false otherwise
     */
    bool HDFDescriptor::isHDF(const std::string & filename, const Version version)
    {
      FILE *fd = fopen(filename.c_str(), "rb");
      if(!fd)
      {
        throw std::invalid_argument("HierarchicalFileDescriptor::isHierarchical - Unable to open file '" + filename + "'");
      }
      const bool result = isHDFHandle(fd, version); // use anonymous helper
      fclose(fd);
      return result;
    }

    //---------------------------------------------------------------------------------------------------------------------------
    // HDFDescriptor public methods
    //---------------------------------------------------------------------------------------------------------------------------
    /**
     * Constructs the wrapper
     * @param filename A string pointing to an existing file
     * @throws std::invalid_argument if the file is not identified to be hierarchical. This currently
     * involves simply checking for the signature if a HDF file at the start of the file
     */
    HDFDescriptor::HDFDescriptor(const std::string & filename)
      : m_filename(), m_extension(), m_firstEntryNameType(),
        m_rootAttrs(), m_pathsToTypes(), m_file(NULL)
    {
      if(filename.empty())
      {
        throw std::invalid_argument("HDFDescriptor() - Empty filename '" + filename + "'");
      }
      if(!Poco::File(filename).exists())
      {
        throw std::invalid_argument("HDFDescriptor() - File '" + filename + "' does not exist");
      }
      try
      {
        initialize(filename);
      }
      catch(::NeXus::Exception &)
      {
        throw std::invalid_argument("HDFDescriptor::initialize - File '" + filename + "' does not look like a HDF file.");
      }
    }

    /**
     */
    HDFDescriptor::~HDFDescriptor()
    {
      delete m_file;
    }

    /// Returns the name & type of the first entry in the file
    const std::pair<std::string,std::string> & HDFDescriptor::firstEntryNameType() const
    {
      return m_firstEntryNameType;
    }

    /**
     * @param name The name of an attribute
     * @return True if the attribute exists, false otherwise
     */
    bool HDFDescriptor::hasRootAttr(const std::string &name) const
    {
      return (m_rootAttrs.count(name) == 1); 
    }

    /**
     * @param path A string giving a path using UNIX-style path separators (/), e.g. /raw_data_1, /entry/bank1
     * @return True if the path exists in the file, false otherwise
     */
    bool HDFDescriptor::pathExists(const std::string& path) const
    {
      return (m_pathsToTypes.find(path) != m_pathsToTypes.end());
    }

    /**
     * @param path A string giving a path using UNIX-style path separators (/), e.g. /raw_data_1, /entry/bank1
     * @param type A string specifying the required type
     * @return True if the path exists in the file, false otherwise
     */
    bool HDFDescriptor::pathOfTypeExists(const std::string& path, const std::string & type) const
    {
      auto it = m_pathsToTypes.find(path);
      if(it != m_pathsToTypes.end())
      {
        return (it->second == type);
      }
      else return false;
    }

    /**
     * @param type A string specifying the required type
     * @return path A string giving a path using UNIX-style path separators (/), e.g. /raw_data_1, /entry/bank1
     */
	std::string HDFDescriptor::pathOfType(const std::string & type) const {
		auto iend = m_pathsToTypes.end();
		for (auto it = m_pathsToTypes.begin(); it != iend; ++it) {
			if (type == it->second)
				return  it->first;
		}
		return  "";
	}


    /**
     * @param classType A string name giving a class type
     * @return True if the type exists in the file, false otherwise
     */
    bool HDFDescriptor::classTypeExists(const std::string & classType) const
    {
      auto iend = m_pathsToTypes.end();
      for(auto it = m_pathsToTypes.begin(); it != iend; ++it)
      {
        if(classType == it->second) return true;
      }
      return false;
    }

    //---------------------------------------------------------------------------------------------------------------------------
    // HDFDescriptor private methods
    //---------------------------------------------------------------------------------------------------------------------------

    /**
     * Creates the internal cached structure of the file as a tree of nodes
     */
    void HDFDescriptor::initialize(const std::string& filename)
    {
      m_filename = filename;
      m_extension = "." + Poco::Path(filename).getExtension();

      m_file = new ::NeXus::File(this->filename());

      m_file->openPath("/");
      m_rootAttrs.clear();
      m_pathsToTypes.clear();
      walkFile(*m_file, "", "", m_pathsToTypes,0);
    }

    /**
     * Cache the structure in the given maps
     * @param file An open NeXus File object
     * @param rootPath The current path that is open in the file
     * @param className The class of the current open path
     * @param tmap [Out] An output map filled with mappings of type->path
     * @param pmap [Out] An output map filled with mappings of path->type
     * @param level An integer defining the current level in the file
     */
    void HDFDescriptor::walkFile(::NeXus::File & file,const std::string & rootPath, const std::string & className,
                                 std::map<std::string, std::string> & pmap, int level)
    {
      if (!rootPath.empty())
      {
        pmap.insert(std::make_pair(rootPath, className));
      }
      if(level == 0)
      {
        auto attrInfos = file.getAttrInfos();
        for(size_t i = 0; i < attrInfos.size(); ++i)
        {
          m_rootAttrs.insert(attrInfos[i].name);
        }
      }

      auto dirents = file.getEntries();
      auto itend = dirents.end();
      for (auto it = dirents.begin(); it != itend; ++it)
      {
        const std::string & entryName = it->first;
        const std::string & entryClass = it->second;
        const std::string entryPath = rootPath + "/" + entryName;
        if(entryClass == "SDS")
        {
          //tmap.insert(std::make_pair(entryClass, entryPath));
          pmap.insert(std::make_pair(entryPath, entryClass));
        }
        else if(entryClass == "CDF0.0")
        {
          // Do nothing with this
        }
        else
        {
          if(level == 0) m_firstEntryNameType = (*it); //copy first entry name & type
          file.openGroup(entryName, entryClass);
          walkFile(file, entryPath, entryClass, pmap,level+1);
        }
      }
      file.closeGroup();
    }



  } // namespace Kernel
} // namespace Mantid