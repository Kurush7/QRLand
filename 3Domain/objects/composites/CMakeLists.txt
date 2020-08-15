set(path ${path}/composites)
set(source_list ${source_list}
        ${path}/interfaces/QRFrame3D.cpp
        ${path}/interfaces/QRScene3D.cpp
        ${path}/interfaces/QRPolyModel3D.cpp
        ${path}/interfaces/QRPolyScene3D.cpp

        ${path}/legacy/SimpleFrame3D.cpp
        ${path}/legacy/SimpleScene3D.cpp

        ${path}/impl/polyModel/PolyModel.cpp
        ${path}/impl/polyModel/PolyModelCreator.cpp
        ${path}/impl/polyScene/PolyScene3D.cpp
        ${path}/impl/polyScene/PolySceneCreator.cpp
        PARENT_SCOPE)
