include_directories(${GLFW3_INCLUDEDIR} ${PROJECT_SOURCE_DIR} ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Core_PRIVATE_INCLUDE_DIRS} ${Qt5Gui_PRIVATE_INCLUDE_DIRS})

add_subdirectory(internal)

set(QGLGUI_SOURCES glfwhelper.cpp glgui.cpp glguirenderer.cpp)
set(QGLGUI_HEADERS glgui.h glguirenderer.h)

add_library(qglgui SHARED ${QGLGUI_SOURCES} $<TARGET_OBJECTS:qglgui_internal> $<TARGET_OBJECTS:qglgui_qtplugin>)
add_library(qglgui-static STATIC ${QGLGUI_SOURCES} $<TARGET_OBJECTS:qglgui_internal> $<TARGET_OBJECTS:qglgui_qtplugin>)

target_link_libraries(qglgui Qt5PlatformSupport ${FREETYPE_LIBRARIES} glib-2.0)
qt5_use_modules(qglgui Core Gui Widgets)

set_target_properties(qglgui-static PROPERTIES OUTPUT_NAME "qglgui")
set_target_properties(qglgui-static PROPERTIES PREFIX "lib")

set_target_properties(qglgui PROPERTIES PUBLIC_HEADER "${QGLGUI_HEADERS}")

install( TARGETS qglgui qglgui-static LIBRARY DESTINATION lib ARCHIVE DESTINATION lib PUBLIC_HEADER DESTINATION include/qglgui)
