add_subdirectory(QRExceptions)
add_subdirectory(QRVector)

add_library(container_library
        exception_lib
        qrvector_lib
        qrset_lib
        qrexception_lib
        )