add_subdirectory(QRExceptions)
add_subdirectory(QRVector)
add_subdirectory(QRSet)
add_subdirectory(QRHashTable)
add_subdirectory(QRMap)

include_directories(QRExceptions)

add_library(containers_lib
        QRContainerAdapter.cpp)

target_link_libraries(containers_lib
        qrexceptions_lib
        qrhash_table_lib
        qrset_lib
        qrvector_lib
        qrmap_lib
        )