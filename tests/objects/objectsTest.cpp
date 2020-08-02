//
// Created by kurush on 29.05.2020.
//


#include "gtest/gtest.h"
#include "3Domain.h"

using namespace std;

TEST(ObjectTests, polygon)
{
    sptr<QRPoint3D> p1(new Point3D(0, 0, 0));
    sptr<QRPoint3D> p2(new Point3D(5, 0, 0));
    sptr<QRPoint3D> p3(new Point3D(0, 7, 0));
    sptr<QRPoint3D> p4(new Point3D(0, 0, 4));

    auto txt = sptr<QRTexture>(new ColorTexture(QRColor("red")));
    auto cr = Polygon3DCreator(txt);
    auto x = sptr<QRPolygon3D>(cr.create({p1,p2,p3}));
    EXPECT_EQ(**x->getPoints(), *p1);
    EXPECT_EQ(**(x->getPoints()+1), *p2);
    EXPECT_EQ(**(x->getPoints()+2), *p3);
    EXPECT_EQ(x->getSize(), 3);
    EXPECT_EQ(x->getNormal() == Vector3D(0,0,1,1) || x->getNormal() == Vector3D(0,0,-1,1), true);
    EXPECT_EQ(x->getTexture()->getColor(), txt->getColor());

    Polygon3D a(QRVector<sptr<QRPoint3D>>({p1,p2,p3}), DEFAULT_TEXTURE);
    Polygon3D b(QRVector<sptr<QRPoint3D>>({p1,p2,p3}), DEFAULT_TEXTURE);
    Polygon3D c = a;
    auto mem = a.save();
    a = b;
    EXPECT_EQ(a, b);
    mem->restore();
    EXPECT_EQ(a, c);
}