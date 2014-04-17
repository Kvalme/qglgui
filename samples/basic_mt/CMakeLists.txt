set(SAMPLE_APPLICATION_SOURCE basic.cpp )

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(basic_multithread ${SAMPLE_APPLICATION_SOURCE})
target_link_libraries(basic_multithread sample_qt samples_shared qglgui-static Qt5PlatformSupport ${GLFW3_LIBRARY} ${OPENGL_gl_LIBRARY} ${FREETYPE_LIBRARIES} glib-2.0 ${PROFILER_LIB})
qt5_use_modules(basic_multithread Core Gui Widgets Svg)

