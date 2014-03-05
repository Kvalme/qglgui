# Find EGL
#
# GLFW3_INCLUDE_DIR
# GLFW3_LIBRARY
# GLFW3_FOUND

find_path(GLFW3_INCLUDE_DIR NAMES GLFW/glfw3.h)

set(GLFW3_NAMES ${GLFW3_NAMES} glfw glfw3)
find_library(GLFW3_LIBRARY NAMES ${GLFW3_NAMES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW3 DEFAULT_MSG GLFW3_LIBRARY GLFW3_INCLUDE_DIR)

mark_as_advanced(GLFW3_INCLUDE_DIR GLFW3_LIBRARY)

