set(SAMPLE_APPLICATION_SOURCE basic.cpp basicqtapp.cpp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_add_resources(styles_RESOURCES_SRC styles.qrc)

add_library(sample_styles OBJECT norwegianwoodstyle.cpp widgetgallery.cpp ${styles_RESOURCES_SRC})

set_property(TARGET sample_styles APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Core_INCLUDE_DIRS}")
set_property(TARGET sample_styles APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Gui_INCLUDE_DIRS}")
set_property(TARGET sample_styles APPEND PROPERTY INCLUDE_DIRECTORIES "${Qt5Widgets_INCLUDE_DIRS}")

