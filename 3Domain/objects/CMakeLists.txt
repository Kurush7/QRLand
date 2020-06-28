add_subdirectory(components)
add_subdirectory(mementos)
add_subdirectory(managers)
add_subdirectory(composites)

set(path "objects/")
set(source_list ${source_list}
        ${path}/exceptions/QRObjectException.cpp

        ${path}/visitors/QRVisitor.cpp
        PARENT_SCOPE)