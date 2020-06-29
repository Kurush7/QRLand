find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# todo make one-lib build
add_library(2domain_lib STATIC
        QRCanvas.cpp
        QRCanvasViewer.cpp
        ImageQT.cpp
        )

target_link_libraries(2domain_lib
        Qt5::Widgets
        )