set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${PROJECT_SOURCE_DIR})
add_definitions(-DQT_STATICPLUGIN)

set(qtplugin_SOURCES uiopenglcontext.cpp uiwindow.cpp uibackingstore.cpp uiscreen.cpp uiintegration.cpp uiworker.cpp)

add_library(qglgui_qtplugin OBJECT ${qtplugin_SOURCES})
set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Core_INCLUDE_DIRS}")
set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Gui_INCLUDE_DIRS}")
set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Widgets_INCLUDE_DIRS}")
set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Quick_INCLUDE_DIRS}")
set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Core_PRIVATE_INCLUDE_DIRS}")
set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Gui_PRIVATE_INCLUDE_DIRS}")
set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Qml_PRIVATE_INCLUDE_DIRS}}")
set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Quick_PRIVATE_INCLUDE_DIRS}")
set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${_qt5Gui_install_prefix}/include/qt5/QtPlatformSupport/")
set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${_qt5Gui_install_prefix}/include/qt5/QtPlatformSupport/${Qt5Gui_VERSION_STRING}")
set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${_qt5Gui_install_prefix}/include/qt5/QtPlatformSupport/${Qt5Gui_VERSION_STRING}/QtPlatformSupport")

