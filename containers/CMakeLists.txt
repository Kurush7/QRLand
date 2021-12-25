set(path "containers")
add_subdirectory(exceptions)
add_subdirectory(QRVector)
add_subdirectory(QRSet)
add_subdirectory(QRHashTable)
add_subdirectory(QRMap)

set(source_list ${source_list}
        PARENT_SCOPE)