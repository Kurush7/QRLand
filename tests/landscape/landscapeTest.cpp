//
// Created by kurush on 29.05.2020.
//


#include "gtest/gtest.h"
#include "landscape/QRLandscapeSurface.h"

using namespace std;

TEST(LandscapeTests, basic)
{
    auto surf = QRLandscapeSurface(10, 10);
    for (auto p = surf.getPoints(); p; ++p)
        cout << p->get()->getVector() << '\n';

}