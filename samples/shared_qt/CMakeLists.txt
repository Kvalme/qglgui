add_subdirectory(styles)
add_subdirectory(tree)
add_subdirectory(QtQuick2)

set(SAMPLE_QT_LIBRARY_SOURCE basicqtapp.cpp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_wrap_ui(ui_basicqtapp.h basicqtapp.ui)

add_library(sample_qt STATIC ${SAMPLE_QT_LIBRARY_SOURCE} ui_basicqtapp.h $<TARGET_OBJECTS:sample_styles> $<TARGET_OBJECTS:sample_tree> $<TARGET_OBJECTS:sample_QtQuick2>)

qt5_use_modules(sample_qt Core Gui Widgets Qml Quick)

