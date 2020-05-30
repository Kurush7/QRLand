cmake_minimum_required(VERSION 3.10)
project(kg)
set(CMAKE_CXX_STANDARD 14)

set(VERBOSE=1)
set(CMAKE_AUTOGEN_VERBOSE on)
set(CMAKE_VERBOSE_MAKEFILE on)
set(DRAW_PATH ${CMAKE_CURRENT_SOURCE_DIR}/2Domain)

# QT includes
find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(QT_INCLUDES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets)


add_subdirectory(tests)

include_directories(2Domain)
add_subdirectory(2Domain)
link_directories(${CMAKE_SOURCE_DIR}/2D_draw_module)

include_directories(3Domain)
add_subdirectory(3Domain)
link_directories(${CMAKE_SOURCE_DIR}/3D_draw_module)

add_library(gui_lib
        gui/MainWindow.cpp
        gui/Presenter.cpp
        )
target_link_libraries(gui_lib
        ${QT_INCLUDES}
        )

add_executable(main.exe
        main.cpp)
#target_compile_definitions(main.exe PUBLIC
#        LAB_NUM=10)

target_link_libraries(main.exe
        gui_lib
        draw_lib
        3domain_lib)

add_custom_command(
        TARGET main.exe POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/gui/style.css
        ${CMAKE_CURRENT_BINARY_DIR}/gui/style.css)