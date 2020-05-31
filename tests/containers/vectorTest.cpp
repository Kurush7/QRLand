//
// Created by kurush on 29.05.2020.
//

#include <map>

#include "gtest/gtest.h"
#include "QRContainers.h"

using namespace std;

TEST(QRVectorTests, basic)
{
    map<int, int> visited;
    vector<int> vec;
    for (int i = 0; i < 5; ++i) {
        vec.push_back(i);
        visited[i] = 0;
    }
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(vec[i], i);
    }

    for (auto it = vec.begin(); it != vec.end(); ++it) {
        visited[*it]++;
    }

    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(visited[i], 1);
}

TEST(QRVectorTests, queue_stack)
{
    QRQueue<int> q;
    QRStack<int> s;

    int n = 10;
    for (int i = 0; i < n; ++i)
        q.push(i), s.push(i);
    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(q.pop(), i);
        EXPECT_EQ(s.pop(), n-i-1);
    }
}
