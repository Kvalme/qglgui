project(qglgui)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake/modules)

set(CMAKE_C_FLAGS_DEBUG "-fPIC -DPIC -Wall -Werror -Og -g3 -DDEBUG -DPROFILING_FILENAME_PREFIX=\\\"QGLGUI\\\"")
set(CMAKE_C_FLAGS_RELEASE "-fPIC -DPIC -Wall -Werror -O3 -DNDEBUG -mfpmath=sse,387 -msse2 -msse3 -DPROFILING_FILENAME_PREFIX=\\\"QGLGUI\\\"")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -fPIC -DPIC -Wall -Werror -Og -g3 -DDEBUG -DPROFILING_FILENAME_PREFIX=\\\"QGLGUI\\\"")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -fPIC -DPIC -Wall -Werror -O3 -DNDEBUG -mfpmath=sse,387 -msse2 -msse3 -DPROFILING_FILENAME_PREFIX=\\\"QGLGUI\\\"")

find_package(OpenGL)

if (NOT OPENGL_FOUND)
	message(FATAL_ERROR "OpenGL library was not found")
endif (NOT OPENGL_FOUND)

option(BUILD_SAMPLES "Build sample applications" ON)
option(ENABLE_PROFILING "Enable profiling" OFF)

if (ENABLE_PROFILING)
	add_subdirectory(libcppprofiler)
endif (ENABLE_PROFILING)

add_subdirectory(qglgui)

if (BUILD_SAMPLES)
	
	find_package(glfw3)
	
	if (NOT GLFW3_FOUND)
		message(WARNING "GLFW3 not found. Samples will not be build.")
	else ()
		add_subdirectory(samples)
	endif (NOT GLFW3_FOUND)
endif (BUILD_SAMPLES)



