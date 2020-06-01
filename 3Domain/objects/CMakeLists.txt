set(path "objects/")
set(source_list ${source_list}
        ${path}/components/Point3D.cpp
        ${path}/components/Edge3D.cpp
        ${path}/components/Camera3D.cpp
        ${path}/components/Projection.cpp
        ${path}/components/Industry.cpp
        ${path}/composites/BaseComposite.cpp
        ${path}/composites/Frame3D.cpp
        ${path}/composites/Scene3D.cpp
        ${path}/managers/LoadSource.cpp
        ${path}/managers/FrameLoader.cpp
        ${path}/managers/SceneBuilder.cpp
        ${path}/managers/SelectionManager.cpp
        ${path}/exceptions/QRObjectException.cpp
        ${path}/visitors/Visitor.cpp
        PARENT_SCOPE)