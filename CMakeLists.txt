cmake_minimum_required(VERSION 3.10)
project(big3DFluffy)
set(CMAKE_CXX_STANDARD 14)

set(VERBOSE=1)
set(CMAKE_AUTOGEN_VERBOSE on)
set(CMAKE_VERBOSE_MAKEFILE on)
set(DRAW_PATH ${CMAKE_CURRENT_SOURCE_DIR}/2DomainOld)

# QT includes
find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(QT_INCLUDES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets)

include_directories(3Domain)
add_subdirectory(3Domain)


include_directories(2DomainOld)
add_subdirectory(2DomainOld)

include_directories(2Domain)
add_subdirectory(2Domain)

add_subdirectory(tests)

add_library(gui_lib
        gui/MainWindow.cpp
        gui/Presenter.cpp
        gui/QTPainter.cpp
        )
target_link_libraries(gui_lib
        ${QT_INCLUDES}
        )

target_compile_definitions(gui_lib PUBLIC
        CURRENT_DIR="~/../models"
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