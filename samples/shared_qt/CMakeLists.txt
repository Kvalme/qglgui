add_subdirectory(styles)

set(SAMPLE_QT_LIBRARY_SOURCE basicqtapp.cpp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_wrap_ui(ui_basicqtapp.h basicqtapp.ui)

add_library(sample_qt STATIC ${SAMPLE_QT_LIBRARY_SOURCE} ui_basicqtapp.h)

qt5_use_modules(sample_qt Core Gui Widgets)

