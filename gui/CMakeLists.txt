find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include_directories(../console)
include_directories(${DRAW_PATH})
include_directories(/usr/include//python3.6/)

add_library(lab03_lib STATIC
        Lab04_View.cpp
        Presenter.cpp
        )

# todo not working!
target_compile_definitions(lab03_lib PUBLIC
        CURRENT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../models"
        )

target_link_libraries(lab03_lib
        xyz_lib
        draw_lib
        console_lib
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )
