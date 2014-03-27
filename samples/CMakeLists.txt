include_directories(./shared ./shared_qt ${PROJECT_SOURCE_DIR} ${GLFW3_INCLUDE_DIR})

add_subdirectory(shared)
add_subdirectory(shared_qt)
add_subdirectory(basic)
add_subdirectory(basic_mt)
