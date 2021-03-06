#include "MantidKernel/MandatoryValidator.h"
#include <boost/python/class.hpp>
#include <string>

using Mantid::Kernel::MandatoryValidator;
using Mantid::Kernel::IValidator;
using namespace boost::python;

namespace
{
  /// A macro for generating exports for each type
  #define EXPORT_MANDATORYVALIDATOR(ElementType, prefix) \
    class_<MandatoryValidator<ElementType>, bases<IValidator>, \
           boost::noncopyable>(#prefix"MandatoryValidator") \
    ;
}

// clang-format off
void export_MandatoryValidator()
// clang-format on
{
  EXPORT_MANDATORYVALIDATOR(double, Float);
  EXPORT_MANDATORYVALIDATOR(long, Int);
  EXPORT_MANDATORYVALIDATOR(std::string, String);

  // Array types
  EXPORT_MANDATORYVALIDATOR(std::vector<double>, FloatArray);
  EXPORT_MANDATORYVALIDATOR(std::vector<long>, IntArray);
  EXPORT_MANDATORYVALIDATOR(std::vector<std::string>, StringArray);
}


