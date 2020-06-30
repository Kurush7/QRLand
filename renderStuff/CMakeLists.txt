find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)


add_library(render_lib
        QRender.cpp
        QRPolyRectCutter.cpp
        QRasterizeZBuffer.cpp
        )

target_link_libraries(render_lib
        Qt5::Widgets
        )