set(path "2Domain")
set(source_list ${source_list}
        ${path}/QRCanvas.cpp
        ${path}/QRCanvasViewer.cpp
        ${path}/QRActionManager.cpp
        ${path}/ImageQT.cpp

        ${path}/figures/QRLine2D.cpp
        ${path}/figures/QRFrame2D.cpp

        ${path}/transformers/ScaleTransformer.cpp

        ${path}/algos/DrawLine.cpp
        ${path}/algos/VoronoiDiagram.cpp
        ${path}/algos/GrahamConvex.cpp

        PARENT_SCOPE)