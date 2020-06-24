set(path "managers/")
set(source_list ${source_list}
        ${path}/drawManagers/SceneDrawMethod.cpp
        ${path}/drawManagers/DrawVisitors.cpp
        ${path}/commands/Command.cpp
        ${path}/commands/CommandManager.cpp
        PARENT_SCOPE)