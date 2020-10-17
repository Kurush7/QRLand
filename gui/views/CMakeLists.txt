find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

add_library(views_lib STATIC QRMenu.cpp QRMenu.h QRViews.h)

target_link_libraries(views_lib
        Qt5::Widgets
        )