set(path "landscape")

add_subdirectory(managers)

set(source_list ${source_list}
        ${path}/basic/QRLandscapeTexture.cpp
        ${path}/basic/QRLandscapeSurface.cpp
        ${path}/basic/QRLandscapeSurfaceCreator.cpp

        ${path}/random_generator.cpp

        ${path}/decoration/TopDownVisualizer.cpp

        ${path}/disturbance/disturbance.cpp

        ${path}/climate/ClimateManager.cpp

        ${path}/ROAM/RoamLandscape.cpp
        ${path}/ROAM/RoamTree.cpp

        ${path}/water/WaterManager.cpp
        ${path}/water/WaterErosion.cpp
        ${path}/water/WaterSource.cpp

        PARENT_SCOPE)