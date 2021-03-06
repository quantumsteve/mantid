##
## Tests for PythonAlgorithms
##

set ( TEST_PY_FILES
  AddSampleLogMultipleTest.py
  ApplyPaalmanPingsCorrectionTest.py
  CalculateSampleTransmissionTest.py
  CheckForSampleLogsTest.py
  ConjoinSpectraTest.py
  CorrectLogTimesTest.py
  CreateLeBailFitInputTest.py
  CreateMDTest.py
  IndirectCalibrationTest.py
  CreateWorkspaceTest.py
  CylinderPaalmanPingsCorrectionTest.py
  DakotaChiSquaredTest.py
  DensityOfStatesTest.py
  DSFinterpTest.py
  EnginXCalibrateFullTest.py
  EnginXCalibrateTest.py
  EnginXFitPeaksTest.py
  EnginXFocusTest.py
  FilterLogByTimeTest.py
  FindReflectometryLinesTest.py
  FlatPlatePaalmanPingsCorrectionTest.py
  GetEiT0atSNSTest.py
  IndirectAnnulusAbsorptionTest.py
  IndirectCylinderAbsorptionTest.py
  IndirectFlatPlateAbsorptionTest.py
  IndirectILLReductionTest.py
  IndirectTransmissionTest.py
  IndirectTransmissionMonitorTest.py
  ILLIN16BCalibrationTest.py
  ISISIndirectEnergyTransferTest.py
  LoadDNSLegacyTest.py
  LoadFullprofFileTest.py
  LoadLiveDataTest.py
  LoadLogPropertyTableTest.py
  LoadMultipleGSSTest.py
  MaskAngleTest.py
  MaskBTPTest.py
  MaskWorkspaceToCalFileTest.py
  MeanTest.py
  MergeCalFilesTest.py
  MolDynTest.py
  MSDFitTest.py
  PDDetermineCharacterizationsTest.py
  RetrieveRunInfoTest.py
  SANSWideAngleCorrectionTest.py
  SavePlot1DTest.py
  SaveVulcanGSSTest.py
  SortByQVectorsTest.py
  SofQWMomentsTest.py
  SortDetectorsTest.py
  SortXAxisTest.py
  Stitch1DManyTest.py
  SuggestTibCNCSTest.py
  SuggestTibHYSPECTest.py
  SymmetriseTest.py
  UpdatePeakParameterTableValueTest.py
  SANSSubtractTest.py
  TimeSliceTest.py
  TransformToIqtTest.py
  ExportSampleLogsToCSVFileTest.py
  ExportExperimentLogTest.py
  PoldiMergeTest.py
  VesuvioResolutionTest.py
  PoldiCreatePeaksFromFileTest.py
)

check_tests_valid ( ${CMAKE_CURRENT_SOURCE_DIR} ${TEST_PY_FILES} )

# Prefix for test name=PythonAlgorithms
pyunittest_add_test ( ${CMAKE_CURRENT_SOURCE_DIR} PythonAlgorithms ${TEST_PY_FILES} )
