add_subdirectory(containers)
include_directories(containers)

add_library(3domain_lib
        Lab3Facade.cpp)

target_link_libraries(3domain_lib
        containers_lib)