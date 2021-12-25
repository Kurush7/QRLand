set(path ${path}/renderStuff)
set(source_list ${source_list}
        ${path}/render/SimpleRenderer.cpp
        ${path}/render/FullThreadRenderer.cpp
        ${path}/render/QuickRenderer.cpp
        ${path}/render/QuickShadowRenderer.cpp
        ${path}/render/QuickRenderData.cpp

        ${path}/cutters/PolyRectCutter.cpp
        ${path}/cutters/Poly3DCutter.cpp
        ${path}/cutters/Quick3DCutter.cpp

        ${path}/QRasterizeZBuffer.cpp
        ${path}/QRLightManager.cpp
        PARENT_SCOPE)
