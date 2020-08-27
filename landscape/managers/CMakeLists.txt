set(path ${path}/managers)

set(source_list ${source_list}
        ${path}/LandscapeBuilder.cpp
        ${path}/PlateManager.cpp

        ${path}/tools/QRToolFabric.cpp
        ${path}/tools/QRToolManager.cpp
        ${path}/tools/BFSWalk.cpp
        PARENT_SCOPE)