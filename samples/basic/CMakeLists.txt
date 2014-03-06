set(SAMPLE_APPLICATION_SOURCE basic.cpp basicqtapp.cpp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Freetype REQUIRED)

qt5_wrap_ui(ui_basicqtapp.h basicqtapp.ui)

add_executable(basic ${SAMPLE_APPLICATION_SOURCE} ui_basicqtapp.h)
target_link_libraries(basic samples_shared qglgui-static ${GLFW3_LIBRARY} ${OPENGL_gl_LIBRARY})
qt5_use_modules(basic Core Gui Widgets)

