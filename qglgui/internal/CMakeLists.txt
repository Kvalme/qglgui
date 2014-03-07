include_directories(${GLFW3_INCLUDEDIR} ${PROJECT_SOURCE_DIR})

add_subdirectory(qtplugin)

set(QGLGUI_INTERNAL_SOURCES glguimultithread.cpp glguiinternalbase.cpp glguisinglethread.cpp)

add_library(qglgui_internal OBJECT ${QGLGUI_INTERNAL_SOURCES})
#target_link_libraries(qglgui_internal qglgui_qtplugin)
