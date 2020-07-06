set(path ${path}/exceptions)

set(source_list ${source_list}
        ${path}/QRContainerException.cpp
        ${path}/SetError.cpp
        ${path}/HashTableError.cpp

        ${path}/VectorError/ErrorAlloc.cpp
        ${path}/VectorError/ErrorDeletedObj.cpp
        ${path}/VectorError/ErrorIndex.cpp
        ${path}/VectorError/ErrorNullptr.cpp
        ${path}/VectorError/ErrorSize.cpp
        ${path}/VectorError/ErrorZeroDivision.cpp
        ${path}/VectorError/VectorError.cpp

        PARENT_SCOPE)