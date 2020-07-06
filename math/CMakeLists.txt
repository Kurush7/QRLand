set(path "math")

set(source_list ${source_list}
        ${path}/Vector3D.cpp
        ${path}/Matrix3D.cpp
        ${path}/Transformer3D.cpp
        ${path}/Functions.cpp
        ${path}/exceptions/MathException.cpp
        PARENT_SCOPE)