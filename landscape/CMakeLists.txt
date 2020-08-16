set(path "landscape")

add_subdirectory(managers)

set(source_list ${source_list}
        ${path}/basic/QRLandscapeTexture.cpp
        ${path}/basic/QRLandscapeSurface.cpp
        ${path}/basic/QRLandscapeSurfaceCreator.cpp

        ${path}/ROAM/RoamLandscape.cpp
        ${path}/ROAM/RoamTree.cpp

        PARENT_SCOPE)