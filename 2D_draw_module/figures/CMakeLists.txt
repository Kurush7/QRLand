find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(figures_lib STATIC
        baseAlgos.cpp
        Matrix.cpp
        Primitives.cpp
        Figures.cpp
        AlgoedLine.cpp
        AlgoedCircle.cpp
        AlgoedEllipse.cpp
        BridgePolyFiller.cpp
        DropPolyFiller.cpp
        LineCutter.cpp
        PolygonCutter.cpp
        Function3D.cpp)

target_link_libraries(figures_lib
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets)