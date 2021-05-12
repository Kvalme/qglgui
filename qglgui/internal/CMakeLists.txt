include_directories(${GLFW3_INCLUDEDIR} ${PROJECT_SOURCE_DIR})

add_subdirectory(qtplugin)

set(QGLGUI_INTERNAL_SOURCES gluiwindowdecoratorauroraebuttons.cpp glguimultithread.cpp glguiinternalbase.cpp glguisinglethread.cpp glguiwindowdecoratoraurorae.cpp)

if (QGL_ENABLE_GL1_RENDERER)
    list(APPEND QGLGUI_INTERNAL_SOURCES gl1guirenderer.cpp)
endif (QGL_ENABLE_GL1_RENDERER)

add_library(qglgui_internal OBJECT ${QGLGUI_INTERNAL_SOURCES})
target_include_directories(qglgui_internal
    PUBLIC "${Qt5Core_INCLUDE_DIRS}"
    PUBLIC "${Qt5Gui_INCLUDE_DIRS}"
    PUBLIC "${Qt5Widgets_INCLUDE_DIRS}"
    PUBLIC "${Qt5Core_PRIVATE_INCLUDE_DIRS}"
    PUBLIC "${Qt5Gui_PRIVATE_INCLUDE_DIRS}"
    PUBLIC "${Qt5Svg_INCLUDE_DIRS}"
    PUBLIC "${_qt5Gui_install_prefix}/include/qt5/QtPlatformSupport/"
    PUBLIC "${Qt5FontDatabaseSupport_PRIVATE_INCLUDE_DIRS}"
    PUBLIC "${Qt5EventDispatcherSupport_PRIVATE_INCLUDE_DIRS}"
)
target_compile_definitions(qglgui_internal PUBLIC QT_NO_DEPRECATED_WARNINGS)
