set(path ${path}/renderStuff)
set(source_list ${source_list}
        ${path}/render/SimpleRenderer.cpp
        ${path}/render/FullThreadRenderer.cpp
        ${path}/QRPolyRectCutter.cpp
        ${path}/QRasterizeZBuffer.cpp
        ${path}/QRLightManager.cpp
        PARENT_SCOPE)
