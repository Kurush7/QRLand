add_subdirectory(containers)
add_subdirectory(math)


add_library(3domain_lib
        Lab3Facade.cpp)

target_link_libraries(3domain_lib
        containers_lib
        math_lib
        )