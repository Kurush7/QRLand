find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(source_list "" renderStuff/QRColorManager.cpp renderStuff/QRColorManager.h renderStuff.h)
add_subdirectory(managers)
add_subdirectory(renderStuff)

add_library(control_lib
        ${source_list}
        )

target_link_libraries(control_lib
        Qt5::Widgets
        )