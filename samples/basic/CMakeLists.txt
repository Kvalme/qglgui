set(SAMPLE_APPLICATION_SOURCE basic.cpp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(basic ${SAMPLE_APPLICATION_SOURCE})
target_link_libraries(basic sample_qt samples_shared qglgui-static Qt5PlatformSupport ${GLFW3_LIBRARY} ${OPENGL_gl_LIBRARY} ${FREETYPE_LIBRARIES} glib-2.0 ${PROFILER_LIB})
qt5_use_modules(basic Core Gui Widgets)

