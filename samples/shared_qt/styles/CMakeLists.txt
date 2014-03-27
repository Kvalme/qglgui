set(SAMPLE_APPLICATION_SOURCE basic.cpp basicqtapp.cpp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_add_resources(styles_RESOURCES_SRC styles.qrc)

add_library(sample_styles STATIC norwegianwoodstyle.cpp widgetgallery.cpp ${styles_RESOURCES_SRC})

qt5_use_modules(sample_styles Core Gui Widgets)

