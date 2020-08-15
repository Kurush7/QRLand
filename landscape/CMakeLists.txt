set(path "landscape")

set(source_list ${source_list}
        ${path}/basic/QRLandscapeTexture.cpp
        ${path}/basic/QRLandscapeSurface.cpp
        ${path}/basic/QRLandscapeSurfaceCreator.cpp

        ${path}/ROAM/RoamLandscape.cpp
        ${path}/ROAM/RoamTree.cpp

        ${path}/managers/LandscapeBuilder.cpp
        PARENT_SCOPE)