set(path ${path}/renderStuff)
set(source_list ${source_list}
        ${path}/render/SimpleRenderer.cpp
        ${path}/render/FullThreadRenderer.cpp
        ${path}/cutters/PolyRectCutter.cpp
        ${path}/cutters/Poly3DCutter.cpp
        ${path}/QRasterizeZBuffer.cpp
        ${path}/QRLightManager.cpp
        PARENT_SCOPE)
