//
// Created by kurush on 29.05.2020.
//


#include "gtest/gtest.h"
#include "QRMath.h"

using namespace std;

TEST(MathTests, vector3D)
{
    Vector3D vec;
    EXPECT_EQ(vec.arr[0], 0);
    EXPECT_EQ(vec.arr[1], 0);
    EXPECT_EQ(vec.arr[2], 0);
    EXPECT_EQ(vec.arr[3], 0);

    auto vec2 = Vector3D(1,2,3);
    EXPECT_EQ(vec2.arr[0], 1);
    EXPECT_EQ(vec2.arr[1], 2);
    EXPECT_EQ(vec2.arr[2], 3);
    EXPECT_EQ(vec2.arr[3], 0);

    auto vec3 = Vector3D({1,2,3});
    EXPECT_EQ(vec2, vec3);

    EXPECT_EQ(vec2 == vec, false);
    EXPECT_EQ(vec2 != vec, true);
    vec2 = vec;
    EXPECT_EQ(vec2 == vec, true);
    EXPECT_EQ(vec2 != vec, false);

    EXPECT_THROW(Vector3D({1,2,3,4}), QRMathException);
}

TEST(MathTests, matrix3D)
{
    Matrix3D m1, m2({{1,2,3,4},{5,6,7,8},{9,10,11,12}, {13,14,15,16}});
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            EXPECT_EQ(m1[i][j], i==j?1:0);
            EXPECT_EQ(m2[i][j], i*4+j+1);
        }

    EXPECT_EQ(m1*m2, m2);
}

TEST(MathTests, Transformer3DCreators)
{
    // simply for them to be here) no math checks are made.... pitifully
    MoveTransformer3DCreator mc(1,2,3);
    RotateTransformer3DCreator rc(1,2,3);
    ScaleTransformer3DCreator sc(1,2,3);
    auto trans = mc.create();
}

