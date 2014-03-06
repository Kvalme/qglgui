include_directories(${GLFW3_INCLUDE_DIR} ${PROJECT_SOURCE_DIR})

set(SAMPLE_APPLICATION_SOURCE glfwsampleapplication.cpp)

add_executable(basic basic.cpp ${SAMPLE_APPLICATION_SOURCE})
target_link_libraries(basic qglgui-static ${GLFW3_LIBRARY} ${OPENGL_gl_LIBRARY})
