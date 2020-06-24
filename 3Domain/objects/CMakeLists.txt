add_subdirectory(components)
add_subdirectory(mementos)
add_subdirectory(composites)

set(path "objects/")
set(source_list ${source_list}
        ${path}/managers/LoadSource.cpp
        ${path}/managers/FrameLoader.cpp
        ${path}/managers/SceneBuilder.cpp
        ${path}/managers/SelectionManager.cpp
        ${path}/exceptions/QRObjectException.cpp
        ${path}/visitors/Visitor.cpp
        PARENT_SCOPE)