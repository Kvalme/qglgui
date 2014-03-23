set(SAMPLE_APPLICATION_SOURCE basic.cpp basicqtapp.cpp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(styles)

qt5_wrap_ui(ui_basicqtapp.h basicqtapp.ui)

add_executable(basic ${SAMPLE_APPLICATION_SOURCE} ui_basicqtapp.h)
target_link_libraries(basic samples_shared qglgui-static sample_styles Qt5PlatformSupport ${GLFW3_LIBRARY} ${OPENGL_gl_LIBRARY} ${FREETYPE_LIBRARIES} glib-2.0 ${PROFILER_LIB})
qt5_use_modules(basic Core Gui Widgets)

