cmake_minimum_required(VERSION 2.2)

project(libqgui)

set(CMAKE_C_FLAGS_DEBUG "-fPIC -DPIC -Wall -Werror -Og -g3 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-fPIC -DPIC -Wall -Werror -O3 -DNDEBUG -mfpmath=sse,387 -msse2 -msse3")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -fPIC -DPIC -Wall -Werror -Og -g3 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -fPIC -DPIC -Wall -Werror -O3 -DNDEBUG -mfpmath=sse,387 -msse2 -msse3")

INCLUDE(FindPkgConfig)

option(BUILD_SAMPLES "Build sample applications" ON)

add_subdirectory(lib)

if (BUILD_SAMPLES)
	PKG_CHECK_MODULES(GLFW3 glfw3)

	if (NOT GLFW3_FOUND)
		message(WARNING "GLFW3 not found. Samples will not be build.")
	else ()
		add_subdirectory(samples)
	endif (NOT GLFW3_FOUND)
endif (BUILD_SAMPLES)



