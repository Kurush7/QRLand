//
// Created by kurush on 29.05.2020.
//


#include "gtest/gtest.h"
#include "3Domain.h"

using namespace std;

TEST(ObjectTests, Point3D)
{
    Point3D p0, p1, p2(Vector3D(1,2,3));
    auto cr = Point3DCreator(PointStyle(green));

    auto p = cr.create(Vector3D(1,2,3), Vector3D(1,1,1));
    EXPECT_EQ(p->getPoint(), Vector3D(1,2,3));
    EXPECT_EQ(p->getBind(), Vector3D(1,1,1));
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
    EXPECT_EQ(e1.getStart(), p0.getPointer());
    EXPECT_EQ(e1.getEnd(), p1.getPointer());
    EXPECT_EQ(e1.getStyle(), EdgeStyle(black));

    auto e2 = Edge3D(p1, p2, yellow);
    auto mem = e1.save();
    e1 = e2;
    EXPECT_EQ(e1, e2);
    mem->restore();
    EXPECT_EQ(e1, e0);

    auto cr = Edge3DCreator(EdgeStyle(yellow));
    auto e3 = cr.create(p2.getPointer(), p1.getPointer());
    EXPECT_EQ(e3->getStart(), p2.getPointer());
    EXPECT_EQ(e3->getEnd(), p1.getPointer());
    EXPECT_EQ(e3->getStyle(), EdgeStyle(yellow));
}

TEST(ObjectTests, Camera3D) {
    FrontalCamera3DCreator x(100);
    auto cam = x.create(100, 100, Vector3D(0,0,0));
}