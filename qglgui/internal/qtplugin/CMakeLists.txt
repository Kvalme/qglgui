set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Freetype REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})

set(qtplugin_SOURCES uiwindow.cpp uibackingstore.cpp uiscreen.cpp uiintegration.cpp uiworker.cpp)

add_library(qglgui_qtplugin STATIC ${qtplugin_SOURCES})
target_link_libraries(qglgui_qtplugin Qt5PlatformSupport ${FREETYPE_LIBRARIES})

set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Core_PRIVATE_INCLUDE_DIRS}")
set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Gui_PRIVATE_INCLUDE_DIRS}")
set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${_qt5Gui_install_prefix}/include/qt5/QtPlatformSupport/")
set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${_qt5Gui_install_prefix}/include/qt5/QtPlatformSupport/${Qt5Gui_VERSION_STRING}")
set_property(TARGET qglgui_qtplugin APPEND PROPERTY INCLUDE_DIRECTORIES "${_qt5Gui_install_prefix}/include/qt5/QtPlatformSupport/${Qt5Gui_VERSION_STRING}/QtPlatformSupport")

qt5_use_modules(qglgui_qtplugin Core Gui Widgets)

