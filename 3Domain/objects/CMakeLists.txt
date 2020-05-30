set(path "objects/")
set(source_list ${source_list}
        ${path}/components/Point3D.cpp
        ${path}/components/Edge3D.cpp
        ${path}/components/Camera3D.cpp
        ${path}/components/Projection.cpp
        ${path}/components/Industry.cpp
        ${path}/composites/Frame3D.cpp
        ${path}/exceptions/QRObjectException.cpp
        PARENT_SCOPE)