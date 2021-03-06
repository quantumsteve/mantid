set ( SRC_FILES
	src/background.cpp
  src/DataComparison.cpp
	src/deltaECalc.cpp
	src/DirectConvertToEnergy.cpp
	src/Homer.cpp
	src/Indirect/AbsorptionCorrections.cpp
	src/Indirect/ApplyCorr.cpp
	src/Indirect/CalcCorr.cpp
	src/Indirect/ConvFit.cpp
  src/Indirect/DensityOfStates.cpp
	src/Indirect/Elwin.cpp
	src/Indirect/IDATab.cpp
  src/Indirect/ILLCalibration.cpp
  src/Indirect/ILLEnergyTransfer.cpp
  src/Indirect/IndirectBayes.cpp
	src/Indirect/IndirectBayesTab.cpp
	src/Indirect/IndirectDataAnalysis.cpp
	src/Indirect/IndirectDataReduction.cpp
	src/Indirect/IndirectDataReductionTab.cpp
	src/Indirect/IndirectDiffractionReduction.cpp
  src/Indirect/IndirectLoadILL.cpp
	src/Indirect/IndirectMolDyn.cpp
	src/Indirect/IndirectMoments.cpp
	src/Indirect/IndirectSassena.cpp
	src/Indirect/IndirectSimulation.cpp
	src/Indirect/IndirectSimulationTab.cpp
  src/Indirect/IndirectSqw.cpp
	src/Indirect/IndirectSymmetrise.cpp
	src/Indirect/IndirectTab.cpp
	src/Indirect/IndirectTools.cpp
	src/Indirect/IndirectToolsTab.cpp
  src/Indirect/IndirectTransmission.cpp
  src/Indirect/IndirectTransmissionCalc.cpp
  src/Indirect/ISISCalibration.cpp
  src/Indirect/ISISDiagnostics.cpp
  src/Indirect/ISISEnergyTransfer.cpp
	src/Indirect/Iqt.cpp
	src/Indirect/IqtFit.cpp
	src/Indirect/JumpFit.cpp
	src/Indirect/MSDFit.cpp
	src/Indirect/Quasi.cpp
	src/Indirect/ResNorm.cpp
	src/Indirect/Stretch.cpp
	src/MantidEV.cpp
	src/MantidEVWorker.cpp
    src/MultiDatasetFit/MultiDatasetFit.cpp
    src/MultiDatasetFit/MDFAddWorkspaceDialog.cpp
    src/MultiDatasetFit/MDFDataController.cpp
    src/MultiDatasetFit/MDFPlotController.cpp
    src/MultiDatasetFit/MDFDatasetPlotData.cpp
    src/MultiDatasetFit/MDFLocalParameterEditor.cpp
    src/MultiDatasetFit/MDFLocalParameterItemDelegate.cpp
    src/MultiDatasetFit/MDFEditLocalParameterDialog.cpp
	src/Muon/ALCBaselineModellingModel.cpp
	src/Muon/ALCBaselineModellingPresenter.cpp
	src/Muon/ALCBaselineModellingView.cpp
	src/Muon/ALCDataLoadingPresenter.cpp
	src/Muon/ALCDataLoadingView.cpp
	src/Muon/ALCHelper.cpp
	src/Muon/ALCInterface.cpp
	src/Muon/ALCPeakFittingModel.cpp
	src/Muon/ALCPeakFittingPresenter.cpp
	src/Muon/ALCPeakFittingView.cpp
	src/Muon/IO_MuonGrouping.cpp
	src/Muon/MuonAnalysis.cpp
	src/Muon/MuonAnalysisFitDataTab.cpp
	src/Muon/MuonAnalysisHelper.cpp
	src/Muon/MuonAnalysisOptionTab.cpp
	src/Muon/MuonAnalysisResultTableTab.cpp
	src/QReflTableModel.cpp
	src/QtReflMainView.cpp
	src/QtReflOptionsDialog.cpp
	src/ReflCatalogSearcher.cpp
	src/ReflLegacyTransferStrategy.cpp
	src/ReflMainViewPresenter.cpp
	src/ReflSearchModel.cpp
  src/SampleTransmission.cpp
	src/SANSAddFiles.cpp
	src/SANSDiagnostics.cpp
	src/SANSEventSlicing.cpp
	src/SANSPlotSpecial.cpp
	src/SANSRunWindow.cpp
	src/StepScan.cpp
	src/TomoReconstruction/SavuConfigDialog.cpp
	src/TomoReconstruction/TomoReconstruction.cpp
	src/TomoReconstruction/TomoToolConfigDialog.cpp
	src/TomoReconstruction/ToolSettings.cpp
	src/UserInputValidator.cpp
)

# Include files aren't required, but this makes them appear in Visual Studio
set ( INC_FILES
	inc/MantidQtCustomInterfaces/Background.h
  inc/MantidQtCustomInterfaces/DataComparison.h
	inc/MantidQtCustomInterfaces/deltaECalc.h
	inc/MantidQtCustomInterfaces/DirectConvertToEnergy.h
	inc/MantidQtCustomInterfaces/DllConfig.h
	inc/MantidQtCustomInterfaces/Homer.h
	inc/MantidQtCustomInterfaces/Indirect/IDATab.h
	inc/MantidQtCustomInterfaces/Indirect/AbsorptionCorrections.h
	inc/MantidQtCustomInterfaces/Indirect/ApplyCorr.h
	inc/MantidQtCustomInterfaces/Indirect/CalcCorr.h
	inc/MantidQtCustomInterfaces/Indirect/ConvFit.h
  inc/MantidQtCustomInterfaces/Indirect/DensityOfStates.h
	inc/MantidQtCustomInterfaces/Indirect/Elwin.h
  inc/MantidQtCustomInterfaces/Indirect/ILLCalibration.h
  inc/MantidQtCustomInterfaces/Indirect/ILLEnergyTransfer.h
	inc/MantidQtCustomInterfaces/Indirect/IndirectBayes.h
	inc/MantidQtCustomInterfaces/Indirect/IndirectBayesTab.h
	inc/MantidQtCustomInterfaces/Indirect/IndirectDataAnalysis.h
	inc/MantidQtCustomInterfaces/Indirect/IndirectDataReduction.h
 	inc/MantidQtCustomInterfaces/Indirect/IndirectDataReductionTab.h
	inc/MantidQtCustomInterfaces/Indirect/IndirectDiffractionReduction.h
  inc/MantidQtCustomInterfaces/Indirect/IndirectLoadILL.h
	inc/MantidQtCustomInterfaces/Indirect/IndirectMolDyn.h
	inc/MantidQtCustomInterfaces/Indirect/IndirectMoments.h
	inc/MantidQtCustomInterfaces/Indirect/IndirectSassena.h
	inc/MantidQtCustomInterfaces/Indirect/IndirectSimulation.h
	inc/MantidQtCustomInterfaces/Indirect/IndirectSimulationTab.h
	inc/MantidQtCustomInterfaces/Indirect/IndirectSqw.h
	inc/MantidQtCustomInterfaces/Indirect/IndirectSymmetrise.h
  inc/MantidQtCustomInterfaces/Indirect/IndirectTransmission.h
  inc/MantidQtCustomInterfaces/Indirect/IndirectTransmissionCalc.h
	inc/MantidQtCustomInterfaces/Indirect/IndirectTools.h
	inc/MantidQtCustomInterfaces/Indirect/IndirectToolsTab.h
  inc/MantidQtCustomInterfaces/Indirect/IndirectTab.h
	inc/MantidQtCustomInterfaces/Indirect/JumpFit.h
	inc/MantidQtCustomInterfaces/Indirect/MSDFit.h
	inc/MantidQtCustomInterfaces/Indirect/Quasi.h
	inc/MantidQtCustomInterfaces/Indirect/ResNorm.h
	inc/MantidQtCustomInterfaces/Indirect/Stretch.h
  inc/MantidQtCustomInterfaces/Indirect/ISISCalibration.h
  inc/MantidQtCustomInterfaces/Indirect/ISISDiagnostics.h
  inc/MantidQtCustomInterfaces/Indirect/ISISEnergyTransfer.h
	inc/MantidQtCustomInterfaces/Indirect/Iqt.h
	inc/MantidQtCustomInterfaces/Indirect/IqtFit.h
	inc/MantidQtCustomInterfaces/IReflPresenter.h
	inc/MantidQtCustomInterfaces/IReflSearcher.h
	inc/MantidQtCustomInterfaces/MantidEV.h
	inc/MantidQtCustomInterfaces/MantidEVWorker.h
	inc/MantidQtCustomInterfaces/MultiDatasetFit/MultiDatasetFit.h
	inc/MantidQtCustomInterfaces/MultiDatasetFit/MDFAddWorkspaceDialog.h
	inc/MantidQtCustomInterfaces/MultiDatasetFit/MDFDataController.h
	inc/MantidQtCustomInterfaces/MultiDatasetFit/MDFPlotController.h
	inc/MantidQtCustomInterfaces/MultiDatasetFit/MDFDatasetPlotData.h
    inc/MantidQtCustomInterfaces/MultiDatasetFit/MDFLocalParameterEditor.h
    inc/MantidQtCustomInterfaces/MultiDatasetFit/MDFLocalParameterItemDelegate.h
    inc/MantidQtCustomInterfaces/MultiDatasetFit/MDFEditLocalParameterDialog.h
	inc/MantidQtCustomInterfaces/Muon/ALCBaselineModellingModel.h
	inc/MantidQtCustomInterfaces/Muon/ALCBaselineModellingPresenter.h
	inc/MantidQtCustomInterfaces/Muon/ALCBaselineModellingView.h
	inc/MantidQtCustomInterfaces/Muon/ALCDataLoadingPresenter.h
	inc/MantidQtCustomInterfaces/Muon/ALCDataLoadingView.h
	inc/MantidQtCustomInterfaces/Muon/ALCHelper.h
	inc/MantidQtCustomInterfaces/Muon/ALCInterface.h
	inc/MantidQtCustomInterfaces/Muon/ALCPeakFittingModel.h
	inc/MantidQtCustomInterfaces/Muon/ALCPeakFittingPresenter.h
	inc/MantidQtCustomInterfaces/Muon/ALCPeakFittingView.h
	inc/MantidQtCustomInterfaces/Muon/IALCBaselineModellingModel.h
	inc/MantidQtCustomInterfaces/Muon/IALCBaselineModellingView.h
	inc/MantidQtCustomInterfaces/Muon/IALCDataLoadingView.h
	inc/MantidQtCustomInterfaces/Muon/IALCPeakFittingModel.h
	inc/MantidQtCustomInterfaces/Muon/IALCPeakFittingView.h
	inc/MantidQtCustomInterfaces/Muon/IO_MuonGrouping.h
	inc/MantidQtCustomInterfaces/Muon/MuonAnalysis.h
	inc/MantidQtCustomInterfaces/Muon/MuonAnalysisFitDataTab.h
	inc/MantidQtCustomInterfaces/Muon/MuonAnalysisHelper.h
	inc/MantidQtCustomInterfaces/Muon/MuonAnalysisOptionTab.h
	inc/MantidQtCustomInterfaces/Muon/MuonAnalysisResultTableTab.h
	inc/MantidQtCustomInterfaces/ReflCatalogSearcher.h
	inc/MantidQtCustomInterfaces/ReflLegacyTransferStrategy.h
	inc/MantidQtCustomInterfaces/ReflMainView.h
	inc/MantidQtCustomInterfaces/ReflMainViewPresenter.h
	inc/MantidQtCustomInterfaces/ReflSearchModel.h
	inc/MantidQtCustomInterfaces/ReflTransferStrategy.h
	inc/MantidQtCustomInterfaces/QReflTableModel.h
	inc/MantidQtCustomInterfaces/QtReflMainView.h
	inc/MantidQtCustomInterfaces/QtReflOptionsDialog.h
        inc/MantidQtCustomInterfaces/SampleTransmission.h
	inc/MantidQtCustomInterfaces/SANSAddFiles.h
	inc/MantidQtCustomInterfaces/SANSDiagnostics.h
	inc/MantidQtCustomInterfaces/SANSEventSlicing.h
	inc/MantidQtCustomInterfaces/SANSPlotSpecial.h
	inc/MantidQtCustomInterfaces/SANSRunWindow.h
	inc/MantidQtCustomInterfaces/StepScan.h
	inc/MantidQtCustomInterfaces/TomoReconstruction/TomoReconstruction.h
	inc/MantidQtCustomInterfaces/TomoReconstruction/TomoToolConfigDialog.h
	inc/MantidQtCustomInterfaces/TomoReconstruction/ToolSettings.h
	inc/MantidQtCustomInterfaces/Updateable.h
	inc/MantidQtCustomInterfaces/UserInputValidator.h
)

set ( SRC_UNITY_IGNORE_FILES )

set ( MOC_FILES inc/MantidQtCustomInterfaces/Background.h
                inc/MantidQtCustomInterfaces/DataComparison.h
                inc/MantidQtCustomInterfaces/DirectConvertToEnergy.h
                inc/MantidQtCustomInterfaces/Homer.h
                inc/MantidQtCustomInterfaces/Indirect/IDATab.h
                inc/MantidQtCustomInterfaces/Indirect/AbsorptionCorrections.h
                inc/MantidQtCustomInterfaces/Indirect/ApplyCorr.h
                inc/MantidQtCustomInterfaces/Indirect/CalcCorr.h
                inc/MantidQtCustomInterfaces/Indirect/ConvFit.h
                inc/MantidQtCustomInterfaces/Indirect/DensityOfStates.h
                inc/MantidQtCustomInterfaces/Indirect/Elwin.h
                inc/MantidQtCustomInterfaces/Indirect/ILLCalibration.h
                inc/MantidQtCustomInterfaces/Indirect/ILLEnergyTransfer.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectBayes.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectBayesTab.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectDataAnalysis.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectDataReduction.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectDataReductionTab.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectDiffractionReduction.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectLoadILL.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectMolDyn.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectMoments.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectSassena.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectSimulation.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectSimulationTab.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectSqw.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectSymmetrise.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectTransmission.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectTransmissionCalc.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectTools.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectToolsTab.h
                inc/MantidQtCustomInterfaces/Indirect/IndirectTab.h
                inc/MantidQtCustomInterfaces/Indirect/Iqt.h
                inc/MantidQtCustomInterfaces/Indirect/IqtFit.h
                inc/MantidQtCustomInterfaces/Indirect/JumpFit.h
                inc/MantidQtCustomInterfaces/Indirect/MSDFit.h
                inc/MantidQtCustomInterfaces/Indirect/Quasi.h
                inc/MantidQtCustomInterfaces/Indirect/ResNorm.h
                inc/MantidQtCustomInterfaces/Indirect/Stretch.h
                inc/MantidQtCustomInterfaces/Indirect/ISISCalibration.h
                inc/MantidQtCustomInterfaces/Indirect/ISISDiagnostics.h
                inc/MantidQtCustomInterfaces/Indirect/ISISEnergyTransfer.h
                inc/MantidQtCustomInterfaces/MantidEV.h
                inc/MantidQtCustomInterfaces/MultiDatasetFit/MultiDatasetFit.h
		inc/MantidQtCustomInterfaces/MultiDatasetFit/MDFAddWorkspaceDialog.h
		inc/MantidQtCustomInterfaces/MultiDatasetFit/MDFDataController.h
		inc/MantidQtCustomInterfaces/MultiDatasetFit/MDFPlotController.h
		inc/MantidQtCustomInterfaces/MultiDatasetFit/MDFLocalParameterEditor.h
		inc/MantidQtCustomInterfaces/MultiDatasetFit/MDFLocalParameterItemDelegate.h
		inc/MantidQtCustomInterfaces/MultiDatasetFit/MDFEditLocalParameterDialog.h
                inc/MantidQtCustomInterfaces/Muon/ALCBaselineModellingPresenter.h
                inc/MantidQtCustomInterfaces/Muon/ALCBaselineModellingView.h
                inc/MantidQtCustomInterfaces/Muon/ALCDataLoadingPresenter.h
                inc/MantidQtCustomInterfaces/Muon/ALCInterface.h
                inc/MantidQtCustomInterfaces/Muon/ALCPeakFittingPresenter.h
                inc/MantidQtCustomInterfaces/Muon/IALCBaselineModellingView.h
                inc/MantidQtCustomInterfaces/Muon/IALCBaselineModellingModel.h
                inc/MantidQtCustomInterfaces/Muon/IALCDataLoadingView.h
                inc/MantidQtCustomInterfaces/Muon/IALCPeakFittingView.h
                inc/MantidQtCustomInterfaces/Muon/IALCPeakFittingModel.h
                inc/MantidQtCustomInterfaces/Muon/MuonAnalysis.h
                inc/MantidQtCustomInterfaces/Muon/MuonAnalysisFitDataTab.h
                inc/MantidQtCustomInterfaces/Muon/MuonAnalysisHelper.h
                inc/MantidQtCustomInterfaces/Muon/MuonAnalysisOptionTab.h
                inc/MantidQtCustomInterfaces/Muon/MuonAnalysisResultTableTab.h
                inc/MantidQtCustomInterfaces/ReflSearchModel.h
                inc/MantidQtCustomInterfaces/QReflTableModel.h
                inc/MantidQtCustomInterfaces/QtReflMainView.h
                inc/MantidQtCustomInterfaces/QtReflOptionsDialog.h
                inc/MantidQtCustomInterfaces/SampleTransmission.h
                inc/MantidQtCustomInterfaces/SANSAddFiles.h
                inc/MantidQtCustomInterfaces/SANSPlotSpecial.h
                inc/MantidQtCustomInterfaces/SANSRunWindow.h
                inc/MantidQtCustomInterfaces/SANSEventSlicing.h              
                inc/MantidQtCustomInterfaces/SANSDiagnostics.h
                inc/MantidQtCustomInterfaces/StepScan.h
                inc/MantidQtCustomInterfaces/TomoReconstruction/TomoReconstruction.h
                inc/MantidQtCustomInterfaces/TomoReconstruction/TomoToolConfigDialog.h
)

set ( UI_FILES inc/MantidQtCustomInterfaces/DataComparison.ui
               inc/MantidQtCustomInterfaces/DirectConvertToEnergy.ui
               inc/MantidQtCustomInterfaces/Indirect/AbsorptionCorrections.ui
               inc/MantidQtCustomInterfaces/Indirect/ApplyCorr.ui
               inc/MantidQtCustomInterfaces/Indirect/CalcCorr.ui
               inc/MantidQtCustomInterfaces/Indirect/ConvFit.ui
               inc/MantidQtCustomInterfaces/Indirect/DensityOfStates.ui
               inc/MantidQtCustomInterfaces/Indirect/Elwin.ui
               inc/MantidQtCustomInterfaces/Indirect/ILLCalibration.ui
               inc/MantidQtCustomInterfaces/Indirect/ILLEnergyTransfer.ui
               inc/MantidQtCustomInterfaces/Indirect/IndirectBayes.ui
               inc/MantidQtCustomInterfaces/Indirect/IndirectDataAnalysis.ui
               inc/MantidQtCustomInterfaces/Indirect/IndirectDataReduction.ui
               inc/MantidQtCustomInterfaces/Indirect/IndirectDiffractionReduction.ui
               inc/MantidQtCustomInterfaces/Indirect/IndirectLoadILL.ui
               inc/MantidQtCustomInterfaces/Indirect/IndirectMolDyn.ui
               inc/MantidQtCustomInterfaces/Indirect/IndirectMoments.ui
               inc/MantidQtCustomInterfaces/Indirect/IndirectSassena.ui
               inc/MantidQtCustomInterfaces/Indirect/IndirectSimulation.ui
               inc/MantidQtCustomInterfaces/Indirect/IndirectSqw.ui
               inc/MantidQtCustomInterfaces/Indirect/IndirectSymmetrise.ui
               inc/MantidQtCustomInterfaces/Indirect/IndirectTools.ui
               inc/MantidQtCustomInterfaces/Indirect/IndirectTransmission.ui
               inc/MantidQtCustomInterfaces/Indirect/IndirectTransmissionCalc.ui
               inc/MantidQtCustomInterfaces/Indirect/Iqt.ui
               inc/MantidQtCustomInterfaces/Indirect/IqtFit.ui
               inc/MantidQtCustomInterfaces/Indirect/JumpFit.ui
               inc/MantidQtCustomInterfaces/Indirect/MSDFit.ui
               inc/MantidQtCustomInterfaces/Indirect/Quasi.ui
               inc/MantidQtCustomInterfaces/Indirect/ResNorm.ui
               inc/MantidQtCustomInterfaces/Indirect/Stretch.ui
               inc/MantidQtCustomInterfaces/Indirect/ISISCalibration.ui
               inc/MantidQtCustomInterfaces/Indirect/ISISDiagnostics.ui
               inc/MantidQtCustomInterfaces/Indirect/ISISEnergyTransfer.ui
               inc/MantidQtCustomInterfaces/MantidEV.ui
               inc/MantidQtCustomInterfaces/MultiDatasetFit/MultiDatasetFit.ui
	       inc/MantidQtCustomInterfaces/MultiDatasetFit/MDFAddWorkspaceDialog.ui
	       inc/MantidQtCustomInterfaces/MultiDatasetFit/EditLocalParameterDialog.ui
               inc/MantidQtCustomInterfaces/Muon/ALCBaselineModellingView.ui
               inc/MantidQtCustomInterfaces/Muon/ALCDataLoadingView.ui
               inc/MantidQtCustomInterfaces/Muon/ALCInterface.ui
               inc/MantidQtCustomInterfaces/Muon/ALCPeakFittingView.ui
               inc/MantidQtCustomInterfaces/Muon/MuonAnalysis.ui
               inc/MantidQtCustomInterfaces/ReflMainWidget.ui
               inc/MantidQtCustomInterfaces/ReflOptionsDialog.ui
               inc/MantidQtCustomInterfaces/ReflWindow.ui
               inc/MantidQtCustomInterfaces/SampleTransmission.ui
               inc/MantidQtCustomInterfaces/SANSPlotSpecial.ui
               inc/MantidQtCustomInterfaces/SANSRunWindow.ui  
               inc/MantidQtCustomInterfaces/SANSEventSlicing.ui
               inc/MantidQtCustomInterfaces/StepScan.ui
               inc/MantidQtCustomInterfaces/TomoReconstruction/TomoReconstruction.ui
               inc/MantidQtCustomInterfaces/TomoReconstruction/TomoToolConfigAstra.ui
               inc/MantidQtCustomInterfaces/TomoReconstruction/TomoToolConfigCustom.ui
               inc/MantidQtCustomInterfaces/TomoReconstruction/TomoToolConfigSavu.ui
               inc/MantidQtCustomInterfaces/TomoReconstruction/TomoToolConfigTomoPy.ui
)

set ( TEST_FILES
  ALCBaselineModellingModelTest.h
  ALCBaselineModellingPresenterTest.h
  ALCDataLoadingPresenterTest.h
  ALCPeakFittingModelTest.h
  ALCPeakFittingPresenterTest.h
  IO_MuonGroupingTest.h
  MuonAnalysisHelperTest.h
  ReflLegacyTransferStrategyTest.h
  ReflMainViewPresenterTest.h
  UserInputValidatorTest.h
)

include_directories ( inc )
include_directories ( ../../QtPropertyBrowser/src )

# Ugly hack to get around issue in Boost version 1.48.0 
# in conjunction with Qt 4.7.4 or greater
set ( qt_version ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH} )
if ( ${Boost_VERSION} GREATER 104799 AND ${qt_version} VERSION_GREATER 4.7.3 )
	set( extra_options "-DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED")
else ( ${Boost_VERSION} GREATER 104799 AND ${qt_version} VERSION_GREATER 4.7.3 )
	set ( extra_options "" )
endif ( ${Boost_VERSION} GREATER 104799 AND ${qt_version} VERSION_GREATER 4.7.3 )

qt4_wrap_cpp ( MOCCED_FILES ${MOC_FILES} OPTIONS ${extra_options} )
qt4_add_resources ( RES_FILES icons/CustomInterfacesIcons.qrc )

set ( ALL_SRC ${SRC_FILES} ${MOCCED_FILES} )#${RES_FILES})

qt4_wrap_ui ( UI_HDRS ${UI_FILES} ) 
include_directories ( ${CMAKE_CURRENT_BINARY_DIR} )

if(UNITY_BUILD)
  include(UnityBuild)
  enable_unity_build(CustomInterfaces SRC_FILES SRC_UNITY_IGNORE_FILES 10)
endif(UNITY_BUILD)

# Use a precompiled header where they are supported
enable_precompiled_headers( inc/MantidQtCustomInterfaces/PrecompiledHeader.h  ALL_SRC )
add_library ( CustomInterfaces ${ALL_SRC} ${INC_FILES} ${UI_HDRS} )

# Set the name of the generated library
set_target_properties ( CustomInterfaces PROPERTIES OUTPUT_NAME MantidQtCustomInterfaces
                                                    COMPILE_DEFINITIONS IN_MANTIDQT_CUSTOMINTERFACES )

if (OSX_VERSION VERSION_GREATER 10.8)
  set_target_properties ( CustomInterfaces PROPERTIES INSTALL_RPATH "@loader_path/../../../Contents/MacOS")
endif () 

find_Package(Threads)

target_link_libraries ( CustomInterfaces LINK_PRIVATE 
            MantidQtAPI
            MantidWidgets
            QtPropertyBrowser
            ${CORE_MANTIDLIBS}
            ${QT_LIBRARIES}
            ${QWT_LIBRARIES}
            ${POCO_LIBRARIES}
            ${Boost_LIBRARIES}
            ${JSONCPP_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT})

###########################################################################
# Testing
###########################################################################

add_subdirectory ( test )

###########################################################################
# Installation settings
###########################################################################

install ( TARGETS CustomInterfaces ${SYSTEM_PACKAGE_TARGET} DESTINATION ${PLUGINS_DIR}/qtplugins/mantid )
