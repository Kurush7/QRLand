set(path ${path}/objects)
add_subdirectory(components)
add_subdirectory(mementos)
add_subdirectory(managers)
add_subdirectory(lights)
add_subdirectory(composites)

set(source_list ${source_list}
        ${path}/exceptions/QRObjectException.cpp
        ${path}/visitors/QRVisitor.cpp
        PARENT_SCOPE)