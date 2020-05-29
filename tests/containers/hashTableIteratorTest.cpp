//
// Created by kurush on 30.04.2020.
//

#include <gtest/gtest.h>

#include <vector>
#include <map>

#include "QRContainers.h"

using namespace std;

struct st{
    int x,y;
    bool operator==(const st &s) {return x == s.x && y == s.y;}
};


TEST(HTIteratorTests, base_foreach_test)
{
    QRHashTable<int> table;
    int n = 5;
    for (int i = 0; i < n; ++i) {
        table.add(i);
    }
    vector<int> visited(n, 0);

    for (auto t: table)
        visited[t]++;

    for (auto v: visited)
        EXPECT_EQ(v, 1);
}

TEST(HTIteratorTests, star_arrow)
{
    struct st{
        int x, y;
        bool operator==(st s) {return s.x == x && s.y == y;}
    };
    QRHashTable<st> table;

    int n = 100;
    map<pair<int, int>, int> visited;
    for (int i = 0; i < n; ++i) {
        table.add(st{i, i});
        visited[{i,i}] = 0;
    }


    for (auto it = table.begin(); it != table.end(); ++it) {
        EXPECT_EQ(it->x, it->y);
        EXPECT_EQ((*it).x, (*it).y);
        //(*it).x = 10; // this should never compile
        visited[{it->x, it->y}]++;
    }

    for (auto v: visited) {
        EXPECT_EQ(v.second, 1);
    }
}


TEST(HTIteratorTests, operator_bool_noteq)
{
    QRHashTable<int> table;
    int n = 100;
    for (int i = 0; i < n; ++i) {
        table.add(i);
    }

    QRHashTableIterator<int> it = table.begin();
    while (it != table.end()) {
        EXPECT_EQ(bool(it), true);
        EXPECT_EQ(it == table.end(), false);
        it++;
    }
    EXPECT_EQ(bool(it), false);
}


TEST(HTIteratorTests, stressTest_addRehash)
{
    QRHashTable<int> table;
    int n = 10;
    for (int i = 0; i < n; ++i) {
        table.add(i);
    }

    QRHashTableIterator<int> it = table.begin();
    for (int i = 1; i <= 6; ++i)
        for (int j = 0; j < n; ++j)
            table.add(n * i + j);

    EXPECT_NO_THROW(*it);
    ++it;
    EXPECT_THROW(*it, QRHashTableBadIterator);
    EXPECT_THROW(*it, QRHashTableException);
    EXPECT_EQ(it, table.end());
}

TEST(HTIteratorTests, stressTest_selfErase)
{
    QRHashTable<st> table;
    int n = 40;
    for (int i = 0; i < n; ++i) {
        table.add({i,i});
    }

    QRHashTableIterator<st> it = table.begin();
    table.erase(*it);

    EXPECT_THROW(*it, QRHashTableBadIterator);
    EXPECT_THROW(*it, QRHashTableException);
    ++it;
    EXPECT_NO_THROW(it->x);
}

TEST(HTIteratorTests, stressTest_clear)
{
    QRHashTable<st> table;
    int n = 40;
    for (int i = 0; i < n; ++i) {
        table.add({i,i});
    }

    QRHashTableIterator<st> it = table.begin();
    table.clear();

    EXPECT_THROW(*it, QRHashTableBadIterator);
    EXPECT_THROW(*it, QRHashTableException);
    EXPECT_THROW(it->x, QRHashTableBadIterator);
    EXPECT_THROW(it->x, QRHashTableException);
    EXPECT_EQ((bool)it, true);
    ++it;
    EXPECT_THROW(*it, QRHashTableBadIterator);
    EXPECT_THROW(*it, QRHashTableException);
    EXPECT_THROW(it->x, QRHashTableBadIterator);
    EXPECT_THROW(it->x, QRHashTableException);
    EXPECT_EQ((bool)it, false);
    EXPECT_EQ(it == table.end(), true);
}

TEST(HTIteratorTests, stressTest_delete)
{
    auto table = new QRHashTable<st>();
    int n = 40;
    for (int i = 0; i < n; ++i) {
        table->add({i,i});
    }

    QRHashTableIterator<st> it = table->begin();
    delete table;
    EXPECT_THROW(*it, QRHashTableBadIterator);
    EXPECT_THROW(*it, QRHashTableException);
    EXPECT_THROW(it->x, QRHashTableBadIterator);
    EXPECT_THROW(it->x, QRHashTableException);
    EXPECT_THROW((bool)it, QRHashTableBadIterator);
    EXPECT_THROW((bool)it, QRHashTableException);
    EXPECT_THROW(++it, QRHashTableBadIterator);
    EXPECT_THROW(++it, QRHashTableException);
}