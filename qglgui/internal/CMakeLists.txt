include_directories(${GLFW3_INCLUDEDIR} ${PROJECT_SOURCE_DIR})

add_subdirectory(qtplugin)

set(QGLGUI_INTERNAL_SOURCES gl1guirenderer.cpp glguimultithread.cpp glguiinternalbase.cpp glguisinglethread.cpp glguiwindowdecoratoraurorae.cpp)

add_library(qglgui_internal OBJECT ${QGLGUI_INTERNAL_SOURCES})
set_property(TARGET qglgui_internal APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Core_INCLUDE_DIRS}")
set_property(TARGET qglgui_internal APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Gui_INCLUDE_DIRS}")
set_property(TARGET qglgui_internal APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Widgets_INCLUDE_DIRS}")
set_property(TARGET qglgui_internal APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Core_PRIVATE_INCLUDE_DIRS}")
set_property(TARGET qglgui_internal APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Gui_PRIVATE_INCLUDE_DIRS}")
