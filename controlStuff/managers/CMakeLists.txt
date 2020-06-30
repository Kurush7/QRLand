set(path "managers/")
set(source_list ${source_list}
        #${path}/drawManagers/SceneDrawMethod.cpp
        #${path}/drawManagers/DrawVisitors.cpp
        ${path}/commands/PolySceneCommands.cpp
        ${path}/commands/CommandManager.cpp
        #${path}/commands/OldCommands.cpp
        PARENT_SCOPE)