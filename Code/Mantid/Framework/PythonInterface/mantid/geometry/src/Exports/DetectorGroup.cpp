#include "MantidGeometry/Instrument/DetectorGroup.h"
#include <boost/python/class.hpp>

using Mantid::Geometry::DetectorGroup;
using Mantid::Geometry::IDetector;
using namespace boost::python;

// clang-format off
void export_DetectorGroup()
// clang-format on
{
  class_<DetectorGroup, bases<IDetector>, boost::noncopyable>("DetectorGroup",
                                                              no_init)
      .def("getDetectorIDs", &DetectorGroup::getDetectorIDs,
           "Returns the list of detector IDs within this group")
      .def("getNameSeparator", &DetectorGroup::getNameSeparator,
           "Returns separator for list of names of detectors");
}
