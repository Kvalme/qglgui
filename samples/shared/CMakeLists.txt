include_directories()

set(SHARED_SOURCE glfwsampleapplication.cpp)

add_library(samples_shared STATIC ${SHARED_SOURCE})
