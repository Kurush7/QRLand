find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

add_library(widgets_lib STATIC
        ColorEdit.cpp
        QRInput.cpp
        CustomDoubleValidator.cpp
        LineSpacer.cpp
        QRadioField.cpp
        MultiRadioField.cpp
        Themes.cpp
        layoutManager/QRLayoutManager.cpp
        layoutManager/QRLayoutNode.cpp)

target_link_libraries(widgets_lib
        Qt5::Widgets
        )