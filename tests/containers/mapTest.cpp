//
// Created by kurush on 29.05.2020.
//

#include <map>

#include "gtest/gtest.h"
#include "QRContainers.h"

using namespace std;

TEST(QRMapTests, basic)
{
    map<int, int> visited;
    QRMap<char, int> qrmap;
    for (int i = 0; i < 5; ++i) {
        qrmap.add('0' + i, i);
        visited[i] = 0;
    }
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(qrmap['0' + i], i);
    }

    for (auto it = qrmap.begin(); it != qrmap.end(); ++it) {
        visited[it.value()]++;
        EXPECT_EQ(it.value(), qrmap[it.key()]);
    }

    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(visited[i], 1);
}
