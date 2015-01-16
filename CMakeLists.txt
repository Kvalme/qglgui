project(qglgui)

INCLUDE(CPack)

cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake/modules)

set(CMAKE_C_FLAGS_DEBUG "-fPIC -DPIC -Wall -Werror -Og -g3 -DDEBUG -DPROFILING_FILENAME_PREFIX=\\\"QGLGUI\\\" -DQT_STATICPLUGIN")
set(CMAKE_C_FLAGS_RELEASE "-fPIC -DPIC -Wall -Werror -O3 -DNDEBUG -mfpmath=sse,387 -msse2 -msse3 -DPROFILING_FILENAME_PREFIX=\\\"QGLGUI\\\" -DQT_STATICPLUGIN")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -fPIC -DPIC -Wall -Werror -Og -g3 -DDEBUG -DPROFILING_FILENAME_PREFIX=\\\"QGLGUI\\\" -DQT_STATICPLUGIN")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -fPIC -DPIC -Wall -Werror -O3 -DNDEBUG -mfpmath=sse,387 -msse2 -msse3 -DPROFILING_FILENAME_PREFIX=\\\"QGLGUI\\\" -DQT_STATICPLUGIN")

find_package(OpenGL REQUIRED)
find_package(Qt5Core 5.2 REQUIRED)
find_package(Qt5Gui 5.2 REQUIRED)
find_package(Qt5Widgets 5.2 REQUIRED)
find_package(Qt5Svg 5.2 REQUIRED)
find_package(Qt5Qml 5.2 REQUIRED)
find_package(Qt5Quick 5.2 REQUIRED)
find_package(Freetype REQUIRED)


option(BUILD_SAMPLES "Build sample applications" ON)
option(ENABLE_PROFILING "Enable profiling" OFF)

option(ENABLE_GL1_RENDERER "Enable basic GL1 renderer (glBegin/End)" OFF)
option(ENABLE_GL2_RENDERER "Enable basic GL2 renderer (VBO + fixed pipeline)" OFF)
option(ENABLE_GL3_RENDERER "Enable basic GL3 renderer (VBO + built-in shaders)" OFF)

if (ENABLE_PROFILING)
	add_definitions(-DENABLE_PROFILING)
	add_subdirectory(libcppprofiler)
	set(PROFILER_LIB cppprofiler-static)
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



