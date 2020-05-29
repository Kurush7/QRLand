add_subdirectory(exceptions)
add_subdirectory(QRVector)
add_subdirectory(QRSet)
add_subdirectory(QRHashTable)
add_subdirectory(QRMap)

include_directories(exceptions)

add_library(containers_lib
        QRContainers.h)

target_link_libraries(containers_lib
        qrexceptions_lib
        qrhash_table_lib
        qrset_lib
        qrvector_lib
        qrmap_lib
        )