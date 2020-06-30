set(path "objects/components")
set(source_list ${source_list}
        ${path}/interfaces/QRPoint3D.cpp
        ${path}/interfaces/QREdge3D.cpp
        ${path}/interfaces/QRCamera3D.cpp
        ${path}/interfaces/QRPolygon3D.cpp
        ${path}/interfaces/QRProjection.cpp

        ${path}/impl/Point3D.cpp
        ${path}/impl/Polygon3D.cpp
        ${path}/impl/Camera3D.cpp

        ${path}/legacy/SimplePoint3D.cpp
        ${path}/legacy/SimpleEdge3D.cpp
        ${path}/legacy/SimpleCamera3D.cpp
        PARENT_SCOPE)
