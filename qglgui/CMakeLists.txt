include_directories(${GLFW3_INCLUDEDIR} ${PROJECT_SOURCE_DIR})

add_subdirectory(internal)

set(QGLGUI_SOURCES glgui.cpp glguirenderer.cpp)
set(QGLGUI_HEADERS glgui.h glguirenderer.h)

add_library(qglgui SHARED ${QGLGUI_SOURCES})
add_library(qglgui-static STATIC ${QGLGUI_SOURCES})

target_link_libraries(qglgui qglgui_internal)
target_link_libraries(qglgui-static qglgui_internal)

set_target_properties(qglgui-static PROPERTIES OUTPUT_NAME "qglgui")
set_target_properties(qglgui-static PROPERTIES PREFIX "lib")

set_target_properties(qglgui PROPERTIES PUBLIC_HEADER "${QGLGUI_HEADERS}")

install( TARGETS qglgui qglgui-static LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include/qglgui)
