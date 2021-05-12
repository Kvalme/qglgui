set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${PROJECT_SOURCE_DIR})
add_definitions(-DQT_STATICPLUGIN)

set(qtplugin_SOURCES uifontdatabase.cpp uiwindow.cpp uibackingstore.cpp uiscreen.cpp uiintegration.cpp uiworker.cpp)

add_library(qglgui_qtplugin OBJECT ${qtplugin_SOURCES})
target_include_directories(qglgui_qtplugin
    PUBLIC "${Qt5Core_INCLUDE_DIRS}"
    PUBLIC "${Qt5Gui_INCLUDE_DIRS}"
    PUBLIC "${Qt5Widgets_INCLUDE_DIRS}"
    PUBLIC "${Qt5Core_PRIVATE_INCLUDE_DIRS}"
    PUBLIC "${Qt5Gui_PRIVATE_INCLUDE_DIRS}"
    PUBLIC "${_qt5Gui_install_prefix}/include/qt5/QtPlatformSupport/"
    PUBLIC "${Qt5FontDatabaseSupport_PRIVATE_INCLUDE_DIRS}"
    PUBLIC "${Qt5EventDispatcherSupport_PRIVATE_INCLUDE_DIRS}"
)

string(REPLACE "QtGui" "QtPlatformSupport" QT5PlatformPrivate "${Qt5Gui_PRIVATE_INCLUDE_DIRS}")

set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${QT5PlatformPrivate}") #"${_qt5Gui_install_prefix}/include/qt5/QtPlatformSupport/${Qt5Gui_VERSION_STRING}")
#set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES #"${_qt5Gui_install_prefix}/include/qt5/QtPlatformSupport/${Qt5Gui_VERSION_STRING}/QtPlatformSupport")
#message(FATAL_ERROR "${QT5PlatformPrivate}")
