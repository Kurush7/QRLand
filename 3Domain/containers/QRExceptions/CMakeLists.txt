set(vector_errors
        VectorError/ErrorAlloc.cpp
        VectorError/ErrorDeletedObj.cpp
        VectorError/ErrorIndex.cpp
        VectorError/ErrorNullptr.cpp
        VectorError/ErrorSize.cpp
        VectorError/ErrorZeroDivision.cpp
        VectorError/VectorError.cpp)

add_library(qrexceptions_lib
        QRContainerException.cpp
        SetError.cpp
        HashTableError.cpp
        ${vector_errors})