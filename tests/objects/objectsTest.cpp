//
// Created by kurush on 29.05.2020.
//


#include "gtest/gtest.h"
#include "3Domain.h"

using namespace std;

TEST(ObjectTests, Point3D)
{
    Point3D p0, p1, p2(Vector3D(1,2,3));
    Point3DCreator cr(Vector3D(1,2,3), Vector3D(4,5,6), PointStyle(green));

    auto p = cr.create();
    EXPECT_EQ(p->getVector(), Vector3D(1,2,3));
    EXPECT_EQ(p->getBind(), Vector3D(4,5,6));
    EXPECT_EQ(p->getStyle(), PointStyle(green));

    auto mem = p1.save();
    p1 = p2;
    EXPECT_EQ(p1, p2);
    mem->restore();
    EXPECT_EQ(p1, p0);
}

TEST(ObjectTests, Edge3D)
{
    Point3D p0(0,0,0), p1(1,1,1), p2(2,2,2);

    auto e1 = Edge3D(p0, p1, black), e0 = e1;
    EXPECT_EQ(e1.getStart().lock(), p0.getPointer().lock());
    EXPECT_EQ(e1.getEnd().lock(), p1.getPointer().lock());
    EXPECT_EQ(e1.getStyle(), EdgeStyle(black));

    auto e2 = Edge3D(p1, p2, yellow);
    auto mem = e1.save();
    e1 = e2;
    EXPECT_EQ(e1, e2);
    mem->restore();
    EXPECT_EQ(e1, e0);
}

TEST(ObjectTests, Camera3D) {
    Ca
}