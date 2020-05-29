cmake_minimum_required(VERSION 3.10)
project(test)

add_subdirectory(googletest)

include_directories(../3Domain/containers)
include_directories(../3Domain/math)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(tests.exe
        test.cpp
        containers/hashTableTest.cpp
        containers/hashTableIteratorTest.cpp
        containers/setTest.cpp
        containers/mapTest.cpp
        containers/vectorTest.cpp
        math/mathTest.cpp
        )
target_link_libraries(tests.exe
        3domain_lib
        gtest
        gtest_main
        )