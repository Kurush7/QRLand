//
// Created by kurush on 29.04.2020.
//

#include <gtest/gtest.h>
#include "QRContainers.h"

#include <string>
#include <vector>

using namespace std;

TEST(HTconstructorTests, constructor_default)
{
    QRHashTable<int> table;
    EXPECT_EQ(table.size(), 0);
    EXPECT_EQ(table.has(0), false);
    EXPECT_EQ(table.has(1e5), false);

    QRHashTable<string> table1;
    EXPECT_EQ(table1.size(), 0);

    QRHashTable<vector<vector<pair<double, string>>>> table2;
    EXPECT_EQ(table2.size(), 0);
}

TEST(HTconstructorTests, constructor_copy)
{
    QRHashTable<long> table1;
    for (long i = -100; i < 100; ++i)
        table1.add(i);

    QRHashTable<long> table2(table1);
    EXPECT_EQ(table2.size(), table1.size());
    for (long i = -100; i < 100; ++i)
        EXPECT_EQ(table2.has(i), true);
}

TEST(HTconstructorTests, constructor_move)
{
    QRHashTable<long> table1;
    for (long i = -500; i < 500; ++i)
        table1.add(i);

    QRHashTable<long> table2( std::move(table1) );
    EXPECT_EQ(table2.size(), 1000);
    for (long i = -500; i < 500; ++i)
        EXPECT_EQ(table2.has(i), true);
}

TEST(HTconstructorTests, constructor_initList)
{
    QRHashTable<int> table({1,2,3,4,5,6,7,8,9,10,11,12,13,14,15});
    for (long i = 1; i <= 5; ++i)
        EXPECT_EQ(table.has(i), true);
}


TEST(HToperatorTests, operator_equal_copy)
{
    QRHashTable<int> table0({1,2,3,4,5});

    QRHashTable<int> table;
    table = table0;
    EXPECT_EQ(table.size(), 5);
    for (long i = 1; i <= 5; ++i)
        EXPECT_EQ(table.has(i), true);

    table = table;
    EXPECT_EQ(table.size(), 5);
    for (long i = 1; i <= 5; ++i)
        EXPECT_EQ(table.has(i), true);
}

TEST(HToperatorTests, operator_equal_move)
{
    QRHashTable<int> table;
    table = QRHashTable<int>({1,2,3,4,5});
    EXPECT_EQ(table.size(), 5);
    for (long i = 1; i <= 5; ++i)
        EXPECT_EQ(table.has(i), true);

    table = move(table);
    table = QRHashTable<int>({1,2,3,4,5});
    EXPECT_EQ(table.size(), 5);
    for (long i = 1; i <= 5; ++i)
        EXPECT_EQ(table.has(i), true);
}

TEST(HToperatorTests, operator_bool)
{
    QRHashTable<int> table;
    EXPECT_EQ((bool)table, false);
    table.add(5);
    EXPECT_EQ((bool)table, true);
    table.erase(5);
    EXPECT_EQ((bool)table, false);
}


TEST(HTlogicTests, add)
{
    QRHashTable<long long> table;
    for (long long i = -100; i <= 100; ++i) {
        table.add(i);
        EXPECT_EQ(table.size(), i + 101);
        for (long long j = -100; j <= 100; ++j)
            EXPECT_EQ(table.has(j), j <= i);
    }
}

TEST(HTlogicTests, erase)
{
    QRHashTable<long long> table;
    long long n = 200;
    for (long long i = 0; i < n; ++i)
        table.add(i);

    for (long long i = 0; i < n; ++i) {
        table.erase(i);
        EXPECT_EQ(table.size(), n-1 - i);
        for (long long j = 0; j < n; ++j)
            EXPECT_EQ(table.has(j), j > i);
    }
}


TEST(HTlogicTests, already_has_element)
{
    QRHashTable<string> table;
    string s = "abc";
    table.add(s);
    table.add(string("xyz"));
    table.add(string("aaa"));
    table.add(string("qwerty"));
    EXPECT_EQ(table.add(s), table.end());

    EXPECT_NO_THROW(QRHashTable<string> table1({"aaa", "abc", "aaa"}));
}

TEST(HTlogicTests, key_not_found)
{
    QRHashTable<char> table({'a','b', 'c', 'd', 'e'});
    EXPECT_EQ(table.erase('x'), table.end());

    table.erase('a');
    EXPECT_EQ(table.erase('a'), table.end());
}

TEST(HTvisualTests, printTable)
{
    QRHashTable<int> table1({1,2,3,4,5,6,7,8});
    QRHashTable<string> table2({"aaa", "abc", "xyz"});
    cout << table1 << '\n' << table2 << '\n';
}

TEST(HTvisualTests, printExceptions)
{
    try {
        time_t t = time(NULL);
        throw QRBadAllocException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to allocate new table!", 10 * sizeof(int));
    }
    catch (exception &exc) {
        cerr << exc.what() << '\n';
    }

    try {
        time_t t = time(NULL);
        throw QRHashTableBadIterator(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to increment iterator!");
    }
    catch (exception &exc) {
        cerr << exc.what() << '\n';
    }
}