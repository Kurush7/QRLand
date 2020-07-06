set(path "3Domain")
include_directories(objects)

add_subdirectory(objects)
add_subdirectory(textures)

set(source_list ${source_list}
        PARENT_SCOPE)