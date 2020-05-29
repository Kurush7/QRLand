//
// Created by kurush on 28.04.2020.
//

#include <iostream>
#include <gtest/gtest.h>

using namespace std;

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    int err = RUN_ALL_TESTS();
    return err;
}
