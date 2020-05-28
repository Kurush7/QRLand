find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(layout_manager_lib STATIC
        QRLayoutManager.cpp
        QRLayoutNode.cpp
        Themes.cpp
        ThreadFunctionCall.cpp
        )

target_link_libraries(layout_manager_lib
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )