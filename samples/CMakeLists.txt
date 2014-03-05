set(SAMPLE_APPLICATION_SOURCE glfwsampleapplication.cpp)

include_directories(${GLFW3_INCLUDEDIR} . ../lib)
link_directories(${GLFW3_LIBDIR})

add_executable(basic basic.cpp ${SAMPLE_APPLICATION_SOURCE})
target_link_libraries(basic ${GLFW3_LIBRARIES} GL)
