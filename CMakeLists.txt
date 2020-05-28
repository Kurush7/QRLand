cmake_minimum_required(VERSION 3.10)
project(kg)

set(CMAKE_CXX_STANDARD 14)
set(DRAW_PATH ${CMAKE_CURRENT_SOURCE_DIR}/draw_module)

find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)


include_directories(kg3)
include_directories(kg4)
include_directories(console)
include_directories(draw_module)
include_directories(widgets)
include_directories(common)

add_subdirectory(console)
add_subdirectory(draw_module)
add_subdirectory(widgets)
add_subdirectory(common)


add_library(kg3_lib
        kg3/Lab03_View.cpp
        kg3/TaskManager.cpp
        kg3/MeasureTime.cpp
        kg3/measureSteps.cpp
        )
target_link_libraries(kg3_lib
        draw_lib
        console_lib
        widgets_lib
        layout_manager_lib
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

add_library(kg4_lib
        kg4/Lab04_View.cpp
        kg4/TaskManager.cpp
        kg4/MeasureTime.cpp
        kg4/measureSteps.cpp
        )
target_link_libraries(kg4_lib
        draw_lib
        console_lib
        widgets_lib
        layout_manager_lib
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

add_library(kg5_lib
        kg5/Lab05_View.cpp
        kg5/TaskManager.cpp
        kg5/MeasureTime.cpp
        )
target_link_libraries(kg5_lib
        draw_lib
        console_lib
        widgets_lib
        layout_manager_lib
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

add_library(kg6_lib
        kg6/Lab06_View.cpp
        kg6/TaskManager.cpp
        )
target_link_libraries(kg6_lib
        draw_lib
        console_lib
        widgets_lib
        layout_manager_lib
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )


add_library(kg7_lib
        kg7/Lab07_View.cpp
        kg7/TaskManager.cpp
        )
target_link_libraries(kg7_lib
        draw_lib
        console_lib
        widgets_lib
        layout_manager_lib
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

add_library(kg8_lib
        kg8/Lab08_View.cpp
        kg8/TaskManager.cpp
        )
target_link_libraries(kg8_lib
        draw_lib
        console_lib
        widgets_lib
        layout_manager_lib
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

add_library(kg9_lib
        kg9/Lab09_View.cpp
        kg9/TaskManager.cpp
        )
target_link_libraries(kg9_lib
        draw_lib
        console_lib
        widgets_lib
        layout_manager_lib
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

add_library(kg10_lib
        kg10/Lab10_View.cpp
        kg10/TaskManager.cpp
        )
target_link_libraries(kg10_lib
        draw_lib
        console_lib
        widgets_lib
        layout_manager_lib
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

add_executable(kg3.exe
        main.cpp)
target_compile_definitions(kg3.exe PUBLIC
        LAB_NUM=3
        )
target_link_libraries(kg3.exe
        kg3_lib)

add_executable(kg4.exe
        main.cpp)
target_compile_definitions(kg4.exe PUBLIC
        LAB_NUM=4
        )
target_link_libraries(kg4.exe
        kg4_lib)


add_executable(kg5.exe
        main.cpp)
target_compile_definitions(kg5.exe PUBLIC
        LAB_NUM=5
        )
target_link_libraries(kg5.exe
        kg5_lib)

add_executable(kg6.exe
        main.cpp)
target_compile_definitions(kg6.exe PUBLIC
        LAB_NUM=6
        )
target_link_libraries(kg6.exe
        kg6_lib)

add_executable(kg7.exe
        main.cpp)
target_compile_definitions(kg7.exe PUBLIC
        LAB_NUM=7
        )
target_link_libraries(kg7.exe
        kg7_lib)

add_executable(kg8.exe
        main.cpp)
target_compile_definitions(kg8.exe PUBLIC
        LAB_NUM=8
        )
target_link_libraries(kg8.exe
        kg8_lib)

add_executable(kg9.exe
        main.cpp)
target_compile_definitions(kg9.exe PUBLIC
        LAB_NUM=9
        )
target_link_libraries(kg9.exe
        kg9_lib)

add_executable(kg10.exe
        main.cpp)
target_compile_definitions(kg10.exe PUBLIC
        LAB_NUM=10
        )
target_link_libraries(kg10.exe
        kg10_lib)
