add_subdirectory(qtplugin)

set(QGLGUI_SOURCES glgui.cpp)
set(QGLGUI_HEADERS glgui.h)

add_library(qglgui SHARED ${QGLGUI_SOURCES})
add_library(qglgui-static STATIC ${QGLGUI_SOURCES})

set_target_properties(qglgui-static PROPERTIES OUTPUT_NAME "qglgui")
set_target_properties(qglgui-static PROPERTIES PREFIX "lib")

set_target_properties(qglgui PROPERTIES PUBLIC_HEADER ${QGLGUI_HEADERS})

install( TARGETS qglgui qglgui-static LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include/qglgui)