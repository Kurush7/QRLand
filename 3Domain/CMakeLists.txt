set(source_list "Lab3Facade.cpp")

add_subdirectory(containers)
add_subdirectory(math)
add_subdirectory(objects)
add_subdirectory(exceptions)

include_directories(objects)
message( STATUS "Variable BAR in ./bar/ = ${source_list}" )

add_library(3domain_lib
        ${source_list}
        )
        #Lab3Facade.cpp
        #${qrexceptions_list}
        #${containers_lib}
        #${math_lib}
        #${objects_list})