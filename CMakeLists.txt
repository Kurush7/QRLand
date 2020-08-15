cmake_minimum_required(VERSION 3.10)
project(big3DFluffy)
set(CMAKE_CXX_STANDARD 14)

set(VERBOSE=1)
set(CMAKE_AUTOGEN_VERBOSE on)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# QT includes
find_package(Qt5 COMPONENTS Core Widgets OpenGL PrintSupport REQUIRED)
find_package(OpenGL)
set(OpenGL_GL_PREFERENCE "LEGACY")
set(QT_INCLUDES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )


include_directories(2Domain)
include_directories(3Domain)
include_directories(containers)
include_directories(exceptions)
include_directories(math)
include_directories(controlStuff)
include_directories(landscape)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (path "")
set(source_list "tests/TimeTest.cpp" landscape/ROAM/RoamTree.cpp landscape/ROAM/RoamTree.h landscape/ROAM/RoamConfig.h landscape/ROAM/RoamLandscapeCreator.cpp landscape/ROAM/RoamLandscapeCreator.h landscape/managers/LandscapeBuilder.cpp landscape/managers/LandscapeBuilder.h landscape/landscapeConfig.h)
add_subdirectory(2Domain)
add_subdirectory(3Domain)
add_subdirectory(containers)
add_subdirectory(math)
add_subdirectory(exceptions)
add_subdirectory(controlStuff)
add_subdirectory(landscape)
add_library(all_lib
        ${source_list})
target_link_libraries(all_lib
        ${QT_INCLUDES}
        )

add_subdirectory(gui)
#add_subdirectory(tests)

add_library(wip1_lib
        gui/wip1/MainWindow.cpp
        gui/wip1/Presenter.cpp
        gui/wip1/Facade.cpp
        )
target_link_libraries(wip1_lib
        ${QT_INCLUDES}
        )

add_library(wip2_lib
        gui/wip2/MainWindow.cpp
        gui/wip2/Presenter.cpp
        gui/wip2/Facade.cpp
        )
target_link_libraries(wip2_lib
        ${QT_INCLUDES}
        )


add_executable(main.exe
        main.cpp)
target_link_libraries(main.exe
        wip2_lib
        widgets_lib
        all_lib
        )

qt5_use_modules(main.exe Widgets OpenGL)
target_link_libraries(main.exe ${QT_LIBRARIES} ${OPENGL_LIBRARIES})

add_custom_command(
        TARGET main.exe POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/gui/style.css
        ${CMAKE_CURRENT_BINARY_DIR}/gui/style.css)