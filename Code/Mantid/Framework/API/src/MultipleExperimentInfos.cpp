#include "MantidAPI/ExperimentInfo.h"
#include "MantidAPI/MultipleExperimentInfos.h"
#include "MantidKernel/System.h"

using namespace Mantid::Kernel;
using namespace Mantid::API;

namespace Mantid
{
namespace API
{


  //----------------------------------------------------------------------------------------------
  /** Constructor
   */
  MultipleExperimentInfos::MultipleExperimentInfos()
  {
  }
    
  //----------------------------------------------------------------------------------------------
  /** Copy constructor
   *
   * @param other :: other workspace to copy    */
  MultipleExperimentInfos::MultipleExperimentInfos(const MultipleExperimentInfos & other)
  {
    m_expInfos.clear();
    // Do a deep copy of ExperimentInfo's
    for (size_t i=0; i<other.m_expInfos.size(); i++)
    {
      ExperimentInfo_sptr copy(new ExperimentInfo(*other.m_expInfos[i]));
      m_expInfos.push_back(copy);
    }
  }

  //----------------------------------------------------------------------------------------------
  /** Destructor
   */
  MultipleExperimentInfos::~MultipleExperimentInfos()
  {
  }
  


  //-----------------------------------------------------------------------------------------------
  /** Get the ExperimentInfo for the given run Index
   *
   * @param runIndex :: 0-based index of the run to get.
   * @return shared ptr to the ExperimentInfo class
   */
  ExperimentInfo_sptr MultipleExperimentInfos::getExperimentInfo(const uint16_t runIndex)
  {
    if (size_t(runIndex) >= m_expInfos.size())
      throw std::invalid_argument("MDEventWorkspace::getExperimentInfo(): runIndex is out of range.");
    return m_expInfos[runIndex];
  }

  //-----------------------------------------------------------------------------------------------
  /** Get the ExperimentInfo for the given run Index
   *
   * @param runIndex :: 0-based index of the run to get.
   * @return shared ptr to the ExperimentInfo class
   */
  ExperimentInfo_const_sptr MultipleExperimentInfos::getExperimentInfo(const uint16_t runIndex) const
  {
    if (size_t(runIndex) >= m_expInfos.size())
      throw std::invalid_argument("MDEventWorkspace::getExperimentInfo(): runIndex is out of range.");
    return m_expInfos[runIndex];
  }

  //-----------------------------------------------------------------------------------------------
  /** Add a new ExperimentInfo to this MDEventWorkspace
   *
   * @param ei :: shared ptr to the ExperimentInfo class to add
   * @return the runIndex at which it was added
   */
  uint16_t MultipleExperimentInfos::addExperimentInfo(ExperimentInfo_sptr ei)
  {
    m_expInfos.push_back(ei);
    return uint16_t(m_expInfos.size()-1);
  }

  //-----------------------------------------------------------------------------------------------
  /** Replace the ExperimentInfo entry at a given place
   *
   * @param runIndex :: 0-based index of the run to replace
   * @param ei :: shared ptr to the ExperimentInfo class to add
   */
  void MultipleExperimentInfos::setExperimentInfo(const uint16_t runIndex, ExperimentInfo_sptr ei)
  {
    if (size_t(runIndex) >= m_expInfos.size())
      throw std::invalid_argument("MDEventWorkspace::setExperimentInfo(): runIndex is out of range.");
    m_expInfos[runIndex] = ei;
  }

  //-----------------------------------------------------------------------------------------------
  /// @return the number of ExperimentInfo's in this workspace
  uint16_t MultipleExperimentInfos::getNumExperimentInfo() const
  {
    return uint16_t(m_expInfos.size());
  }





} // namespace Mantid
} // namespace API
