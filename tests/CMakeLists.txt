cmake_minimum_required(VERSION 3.10)
project(test)

add_subdirectory(googletest)

include_directories(../3Domain/containers)
include_directories(../3Domain/math)
include_directories(../3Domain/objects)
include_directories(../3Domain)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(tests.exe
        test.cpp
        containers/hashTableTest.cpp
        containers/hashTableIteratorTest.cpp
        containers/setTest.cpp
        containers/mapTest.cpp
        containers/vectorTest.cpp
        math/mathTest.cpp
        objects/objectsTest.cpp
        landscape/landscapeTest.cpp
        )
target_link_libraries(tests.exe
        all_lib
        gtest
        gtest_main
        )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/objects/test.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)