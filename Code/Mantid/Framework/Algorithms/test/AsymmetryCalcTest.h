#ifndef ASYMMETRYCALCTEST_H_
#define ASYMMETRYCALCTEST_H_

#include <cxxtest/TestSuite.h>

#include "MantidDataHandling/LoadMuonNexus2.h"
#include "MantidDataHandling/LoadInstrument.h"
#include "MantidDataHandling/GroupDetectors.h"
#include "MantidAPI/IAlgorithm.h"
#include "MantidAlgorithms/AsymmetryCalc.h"
#include "MantidAPI/Workspace.h"
#include "MantidDataObjects/Workspace2D.h"
#include "MantidAPI/AnalysisDataService.h"
#include <stdexcept>

using namespace Mantid::Algorithms;
using namespace Mantid::API;

class AsymmetryCalcTest : public CxxTest::TestSuite
{
public:

  void testName()
  {
    TS_ASSERT_EQUALS( asymCalc.name(), "AsymmetryCalc" )
  }

  void testCategory()
  {
    TS_ASSERT_EQUALS( asymCalc.category(), "Muon" )
  }

  void testInit()
  {
    asymCalc.initialize();
    TS_ASSERT( asymCalc.isInitialized() )
  }

  void testLoadNexusAndSetProperties()
  {
    //Load the muon nexus file
    loader.initialize();
    loader.setPropertyValue("Filename", "emu00006473.nxs");
    loader.setPropertyValue("OutputWorkspace", "EMU6473");
    TS_ASSERT_THROWS_NOTHING( loader.execute() );
    TS_ASSERT_EQUALS(loader.isExecuted(),true);

    //Create the forward and backward groups
    group1.initialize();
    group1.setPropertyValue("Workspace","EMU6473");
    group1.setPropertyValue("SpectraList", "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16");
    TS_ASSERT_THROWS_NOTHING( group1.execute() );
    TS_ASSERT_EQUALS(group1.isExecuted(),true);
	  
    group2.initialize();
    group2.setPropertyValue("SpectraList", "17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32");
    group2.setPropertyValue("Workspace","EMU6473");
    TS_ASSERT_THROWS_NOTHING( group2.execute() );
    TS_ASSERT_EQUALS(group2.isExecuted(),true);

    asymCalc.setPropertyValue("InputWorkspace", "EMU6473");
    asymCalc.setPropertyValue("OutputWorkspace", "Result");
    asymCalc.setPropertyValue("Alpha", "1.0");
    asymCalc.setPropertyValue("ForwardSpectra", "0");
    asymCalc.setPropertyValue("BackwardSpectra", "16");
  }

  void testProperties()
  {
    //this test is badly written and requires the previous test to have succeeded
    TS_ASSERT_EQUALS( asymCalc.getPropertyValue("Alpha"), "1");
  }

  void testExecute()
  {
    //this test is badly written and requires the previous test to have succeeded
    try 
    {
      TS_ASSERT_EQUALS(asymCalc.execute(),true);
    }
    catch(std::runtime_error & e)
    {
      TS_FAIL(e.what());
    }

    MatrixWorkspace_const_sptr outputWS = AnalysisDataService::Instance().retrieveWS<MatrixWorkspace>("Result");

    //Use a range as cxxtest seems to complain about the accuracy
    TS_ASSERT_DELTA(outputWS->dataY(0)[100],0.2965,0.005);
  }

private:
  AsymmetryCalc asymCalc;
  Mantid::DataHandling::LoadMuonNexus2 loader;
  Mantid::DataHandling::GroupDetectors group1;
  Mantid::DataHandling::GroupDetectors group2;

};

#endif /*ASYMMETRYCALCTEST_H_*/