set(source_list "")

add_subdirectory(exceptions)
add_subdirectory(containers)
add_subdirectory(math)
add_subdirectory(objects)
add_subdirectory(textures)

include_directories(objects)
message( STATUS "Variable BAR in ./bar/ = ${source_list}" )

add_library(3domain_lib
        ${source_list}
        )