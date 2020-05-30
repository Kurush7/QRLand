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
    // todo perform testing
    FrontalCamera3DCreator x(100);
    auto cam = x.create(100, 100, Vector3D(0,0,0));
}

TEST(ObjectTests, AbstractFactory) {
    // todo perform testing
    auto factory = shared_ptr<AbstractObject3DFactory>(new BasicObject3DFactory());
    shared_ptr<BasePoint3D> p1 = factory->createPoint(Vector3D(1,2,3));
    shared_ptr<BasePoint3D>  p2 = factory->createPoint(Vector3D(4,5,6));
    shared_ptr<BaseEdge3D> e = factory->createEdge(p1, p2);
    shared_ptr<BaseCamera3D> c = factory->createCamera(100, 100, Vector3D(-100,100,0));

    //int cnt = 0;
    //for (auto it = p1->begin(); it != p1->end(); ++it) {
    //    cnt++;
    //}
    //EXPECT_EQ(cnt, 1);
}