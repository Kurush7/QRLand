set(path "objects/composites")
set(source_list ${source_list}
        ${path}/interfaces/QRComposite.cpp
        ${path}/interfaces/QRFrame3D.cpp
        ${path}/interfaces/QRScene3D.cpp

        ${path}/legacy/SimpleFrame3D.cpp
        ${path}/legacy/SimpleScene3D.cpp
        PARENT_SCOPE)
