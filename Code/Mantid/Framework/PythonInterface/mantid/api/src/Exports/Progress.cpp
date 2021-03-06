#include "MantidAPI/Algorithm.h"
#include "MantidAPI/Progress.h"

#include <boost/python/class.hpp>
#include <boost/python/init.hpp>

using Mantid::API::Progress;
using Mantid::API::Algorithm;
using Mantid::Kernel::ProgressBase;
using namespace boost::python;

// clang-format off
void export_Progress()
// clang-format on
{
  class_<Progress,
         bases<ProgressBase>,
         boost::noncopyable>("Progress",
                             "Make a Progress object that is attached to the given algorithm, "
                             "with progress between fractions [start,end] notifying a total of nreports times",
                             init<Algorithm*,double,double,size_t>((arg("alg"),arg("start"),arg("end"),arg("nreports")))
                            )
    ;
}

