#ifndef MANTID_ALGORITHMS_BINARYOPERATION_H_
#define MANTID_ALGORITHMS_BINARYOPERATION_H_

//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "MantidAPI/PairedGroupAlgorithm.h"
#include "MantidAPI/Workspace.h"
#include "MantidAPI/WorkspaceGroup.h"
#include "MantidAPI/Sample.h"

namespace Mantid
{
  namespace Algorithms
  {
    /** 
    BinaryOperation supports the implementation of a binary operation on two input workspaces.
    It inherits from the Algorithm class, and overrides the init() & exec() methods.

    Required Properties:
    <UL>
    <LI> InputWorkspace1 - The name of the workspace forming the left hand operand</LI>
    <LI> InputWorkspace2 - The name of the workspace forming the right hand operand </LI>
    <LI> OutputWorkspace - The name of the workspace in which to store the result </LI>
    </UL>

    @author Nick Draper
    @date 14/12/2007

    Copyright &copy; 2007-2010 STFC Rutherford Appleton Laboratory

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
    */
    class DLLExport BinaryOperation : public API::PairedGroupAlgorithm
    {
    public:
      /// Default constructor
      BinaryOperation();
      /// Destructor
      virtual ~BinaryOperation();

      /// Algorithm's category for identification overriding a virtual method
      virtual const std::string category() const { return "Arithmetic";}

    //protected:
    private:
      // Overridden Algorithm methods
      void init();
      void exec();

    protected:
      /// The name of the first input workspace property
      virtual std::string inputPropName1() const { return "LHSWorkspace";}
      /// The name of the second input workspace property
      virtual std::string inputPropName2() const { return "RHSWorkspace";}
      /// The name of the output workspace property
      virtual std::string outputPropName() const { return "OutputWorkspace";}

      /// Checks the compatibility of the two workspaces
      virtual bool checkCompatibility(const API::MatrixWorkspace_const_sptr lhs,const API::MatrixWorkspace_const_sptr rhs) const;
      /// Checks the overall size compatibility of two workspaces
      virtual bool checkSizeCompatibility(const API::MatrixWorkspace_const_sptr lhs,const API::MatrixWorkspace_const_sptr rhs) const;

      /** Carries out the binary operation on a single spectrum.
       *  @param lhsX The X values, made available if required.
       *  @param lhsY The vector of lhs data values
       *  @param lhsE The vector of lhs error values
       *  @param rhsY The vector of rhs data values
       *  @param rhsE The vector of rhs error values
       *  @param YOut The vector to hold the data values resulting from the operation
       *  @param EOut The vector to hold the error values resulting from the operation
       */
      virtual void performBinaryOperation(const MantidVec& lhsX, const MantidVec& lhsY, const MantidVec& lhsE,
                                          const MantidVec& rhsY, const MantidVec& rhsE, MantidVec& YOut, MantidVec& EOut) = 0;

      /** Carries out the binary operation when the right hand operand is a single number.
       *  @param lhsX The X values, made available if required.
       *  @param lhsY The vector of lhs data values
       *  @param lhsE The vector of lhs error values
       *  @param rhsY The rhs data value
       *  @param rhsE The rhs error value
       *  @param YOut The vector to hold the data values resulting from the operation
       *  @param EOut The vector to hold the error values resulting from the operation
       */
      virtual void performBinaryOperation(const MantidVec& lhsX, const MantidVec& lhsY, const MantidVec& lhsE,
                                          const double& rhsY, const double& rhsE, MantidVec& YOut, MantidVec& EOut) = 0;

      /** Should be overridden by operations that need to manipulate the units of the output workspace.
       *  Does nothing by default.
       *  @param lhs The first input workspace
       *  @param rhs The second input workspace
       *  @param out The output workspace
       */
      virtual void setOutputUnits(const API::MatrixWorkspace_const_sptr lhs,const API::MatrixWorkspace_const_sptr rhs,API::MatrixWorkspace_sptr out) {}

      /** Only overridden by operations that affect the properties of the sample (e.g. Plus
       *  where the proton currents (charges) are added). Otherwise it does nothing.
       *  @param lhs one of the workspaces to operate on
       *  @param rhs the other workspace
       *  @param ans the output workspace
       */
      virtual void operateOnSample(const API::Sample& lhs, const API::Sample& rhs, API::Sample& ans) const {};

    private:
      void doSingleValue(const API::MatrixWorkspace_const_sptr lhs,const API::MatrixWorkspace_const_sptr rhs,API::MatrixWorkspace_sptr out);
      void doSingleSpectrum(const API::MatrixWorkspace_const_sptr lhs,const API::MatrixWorkspace_const_sptr rhs,API::MatrixWorkspace_sptr out);
      void doSingleColumn(const API::MatrixWorkspace_const_sptr lhs,const API::MatrixWorkspace_const_sptr rhs,API::MatrixWorkspace_sptr out);
      void do2D(const API::MatrixWorkspace_const_sptr lhs,const API::MatrixWorkspace_const_sptr rhs,API::MatrixWorkspace_sptr out);

      void propagateMasking(const API::MatrixWorkspace_const_sptr rhs, API::MatrixWorkspace_sptr out);

      API::Progress* m_progress;  ///< Progress reporting
    };

  } // namespace Algorithms
} // namespace Mantid

#endif /*MANTID_ALGORITHM_BINARYOPERATION_H_*/
