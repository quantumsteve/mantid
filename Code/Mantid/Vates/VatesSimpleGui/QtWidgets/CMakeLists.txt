project( MantidVatesSimpleGuiQtWidgets )

# These are the C++ files to be compiled.
set( INCLUDE_FILES
  inc/MantidVatesSimpleGuiQtWidgets/AxisInformation.h
  inc/MantidVatesSimpleGuiQtWidgets/GeometryParser.h
  inc/MantidVatesSimpleGuiQtWidgets/ModeControlWidget.h
  inc/MantidVatesSimpleGuiQtWidgets/RotationPointDialog.h
  inc/MantidVatesSimpleGuiQtWidgets/WidgetDllOption.h
)

set( SOURCE_FILES
  src/AxisInformation.cpp
  src/GeometryParser.cpp
  src/ModeControlWidget.cpp
  src/RotationPointDialog.cpp
)

# These are the headers to be preprocessed using Qt's moc preprocessor.
qt4_wrap_cpp( MOC_SOURCES
  inc/MantidVatesSimpleGuiQtWidgets/ModeControlWidget.h
  inc/MantidVatesSimpleGuiQtWidgets/RotationPointDialog.h
)

# Add the QtWidgets icons
qt4_add_resources( RES_FILES icons/QtWidgetsIcons.qrc )

# These are the ui files to be processed using
# Qt's ui file processor.
qt4_wrap_ui( UI_BUILT_SOURCES
  inc/MantidVatesSimpleGuiQtWidgets/ModeControlWidget.ui
  inc/MantidVatesSimpleGuiQtWidgets/RotationPointDialog.ui
)

# Setup the include locations.
include_directories(
  inc
  ${CMAKE_CURRENT_BINARY_DIR}
  ${QWT_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/MantidQt/API/inc
)

set( ALL_FILES
${INCLUDE_FILES}
${SOURCE_FILES}
${MOC_SOURCES}
${UI_BUILT_SOURCES}
${RES_FILES}
)

# For Windows
add_definitions( -DIN_MANTID_VATES_SIMPLEGUI_QTWIDGETS )
# Add the target for this directory
add_library( VatesSimpleGuiQtWidgets ${ALL_FILES} )
# Set the name of the generated library
set_target_properties( VatesSimpleGuiQtWidgets PROPERTIES OUTPUT_NAME MantidVatesSimpleGuiQtWidgets )
# Add to the 'VatesSimpleGui' group in VS
set_property( TARGET VatesSimpleGuiQtWidgets PROPERTY FOLDER MantidVatesSimpleGui )

target_link_libraries( VatesSimpleGuiQtWidgets  LINK_PRIVATE 
${POCO_LIBRARIES}
${QT_LIBRARIES}
${QWT_LIBRARIES}
${MANTID_SUBPROJECT_LIBS}
MantidQtAPI
)

if (OSX_VERSION VERSION_GREATER 10.8)
  set_target_properties ( VatesSimpleGuiQtWidgets PROPERTIES INSTALL_RPATH "@loader_path/../Contents/Libraries;@loader_path/../Contents/MacOS")
endif ()

install( TARGETS VatesSimpleGuiQtWidgets ${SYSTEM_PACKAGE_TARGET} DESTINATION ${PVPLUGINS_DIR} )
