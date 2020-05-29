//
// Created by kurush on 01.05.2020.
//

#include <map>

#include "gtest/gtest.h"
#include "QRContainers.h"

using namespace std;

TEST(QRSetConstructorTests, constructor_default)
{
    QRSet<int> s;
    EXPECT_EQ(s.size(), 0);
    EXPECT_EQ(s.has(1), false);
}

TEST(QRSetConstructorTests, constructor_copy)
{
    QRSet<char> s;
    for (char i = 'a'; i < 'z'; ++i)
        s.add(i);

    auto s1 = s;
    EXPECT_EQ(s1.size(), s.size());
    for (char i = 'a'; i < 'z'; ++i)
        EXPECT_EQ(s1.has(i), true);
}

TEST(QRSetConstructorTests, constructor_move)
{
    int n = 100;
    QRSet<long long> s;
    for (long long i =0; i < n; ++i)
        s.add(i);

    auto s1(move(s));
    EXPECT_EQ(s1.size(), n);
    for (long long i =0; i < n; ++i)
        EXPECT_EQ(s1.has(i), true);
}

TEST(QRSetConstructorTests, constructor_initList)
{
    QRSet<int> s = {0,1,2,3,4,5,6,7,8,9};
    EXPECT_EQ(s.size(), 10);
    for (size_t i = 0; i < 10; ++i)
        EXPECT_EQ(s.has(i), true);

    s = {0,-1,-2,-3,-4,-5};
    EXPECT_EQ(s.size(), 6);
    for (size_t i = 0; i < 6; ++i)
        EXPECT_EQ(s.has(-i), true);
}

TEST(QRSetConstructorTests, constructor_array)
{
    int n = 40;
    int *arr = new int[n];
    for (int i = 0; i < n; ++i)
        arr[i] = i;

    QRSet<int> s(arr, n);

    EXPECT_EQ(s.size(), n);
    for (size_t i = 0; i < n; ++i)
        EXPECT_EQ(s.has(arr[i]), true);

    delete[] arr;

    EXPECT_THROW(s = QRSet<int>(nullptr, 5), QRSetExceptionBadPointer);
    EXPECT_THROW(s = QRSet<int>(nullptr, 5), QRSetException);
    EXPECT_NO_THROW(s = QRSet<int>(nullptr, 0));
}

TEST(QRSetLogicTests, operator_equal)
{
    QRSet<int> s = {0,1,2,3,4,5,6,7,8,9};
    QRSet<int> s1;
    s1 = s;
    EXPECT_EQ(s1.size(), 10);
    for (size_t i = 0; i < 10; ++i)
        EXPECT_EQ(s1.has(i), true);


    s1 = move(s);
    EXPECT_EQ(s1.size(), 10);
    for (size_t i = 0; i < 10; ++i)
        EXPECT_EQ(s1.has(i), true);
}

TEST(QRSetLogicTests, add_has)
{
    QRSet<int> s;
    int n = 180;
    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(s.size(), i);
        s.add(i);
    }
    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(s.has(i), true);
        EXPECT_EQ(s.has(n+i), false);
    }
}

TEST(QRSetLogicTests, remove)
{
    QRSet<long> s;
    int n = 400;
    for (long i = 0; i < n; ++i)
        s.add(i);

    for (long i = 0; i < n; ++i) {
        EXPECT_EQ(s.size(), n-i);
        s.erase(i);
    }
}

TEST(QRSetLogicTests, clear_empty)
{
    QRSet<long> s;
    int n = 400;
    for (long i = 0; i < n; ++i)
        s.add(i);

    EXPECT_EQ(s.size(), n);
    EXPECT_EQ(s.isEmpty(), false);
    s.clear();
    EXPECT_EQ(s.size(), 0);
    EXPECT_EQ(s.isEmpty(), true);
}

TEST(QRSetLogicTests, make_array)
{
    QRSet<int> s;
    map<int, int> has;

    int n = 10;
    for (int i = 0; i < n; ++i)
        s.add(i), has[i] = 0;

    auto arr = s.make_array();
    for (int i = 0; i < n; ++i)
        has[arr[i]]++;
    delete[] arr;

    for (auto x: has)
        EXPECT_EQ(x.second, 1);
}

TEST(QRSetLogic2Tests, equility)
{
    QRSet<int> s0 = {1,2,3,4,5};
    QRSet<int> s1 = {1,2,3,4,5}, s2 = {1,2,3,4}, s3 = {6};
    QRSet<int> s4 = {};

    EXPECT_EQ(s0.isEqual(s1), true);
    EXPECT_EQ(s0 == s1, true);
    EXPECT_EQ(s0.isNotEqual(s1), false);
    EXPECT_EQ(s0 != s1, false);
    EXPECT_EQ(s0 == s2, false);
    EXPECT_EQ(s0 != s2, true);
    EXPECT_EQ(s0 == s3, false);
    EXPECT_EQ(s0 != s3, true);
    EXPECT_EQ(s0 == s4, false);
    EXPECT_EQ(s0 != s4, true);
    EXPECT_EQ(s0 == s0, true);
    EXPECT_EQ(s0 != s0, false);
    EXPECT_EQ(s4 == s4, true);
    EXPECT_EQ(s4 != s4, false);
}


TEST(QRSetLogic2Tests, join)
{
    QRSet<int> s1 = {1,2,3,4,5};
    QRSet<int> s2 = {3,4,5,6,7,8,9,10};

    EXPECT_EQ(s1 | s2, QRSet<int>({1,2,3,4,5,6,7,8,9,10}));
    EXPECT_EQ(s1 | s1, s1);
    EXPECT_EQ(s1 | QRSet<int>(), s1);
    EXPECT_EQ(s1 | 6, QRSet<int>({1,2,3,4,5,6}));
    EXPECT_EQ(s1 | 1, QRSet<int>({1,2,3,4,5}));
    EXPECT_EQ(6 | s1, QRSet<int>({1,2,3,4,5,6}));
    EXPECT_EQ(1 | s1, QRSet<int>({1,2,3,4,5}));
}

TEST(QRSetLogic2Tests, intersection)
{
    QRSet<int> s1 = {1,2,3,4,5};
    QRSet<int> s2 = {3,4,5,6,7,8,9,10};

    EXPECT_EQ(s1 & s2, QRSet<int>({3,4,5}));
    EXPECT_EQ(s1 & s1, s1);
    EXPECT_EQ(s1 & QRSet<int>(), QRSet<int>());
    EXPECT_EQ(s1 & QRSet<int>({6,7,}), QRSet<int>());
    EXPECT_EQ(s1 & 6, QRSet<int>({}));
    EXPECT_EQ(s1 & 1, QRSet<int>({1}));
    EXPECT_EQ(6 & s1, QRSet<int>({}));
    EXPECT_EQ(1 & s1, QRSet<int>({1}));
}

TEST(QRSetLogic2Tests, difference)
{
    QRSet<int> s1 = {1,2,3,4,5};
    QRSet<int> s2 = {3,4,5,6,7,8,9,10};

    EXPECT_EQ(s1- s2, QRSet<int>({1,2}));
    EXPECT_EQ(s1- s1, QRSet<int>());
    EXPECT_EQ(s1 - QRSet<int>(), s1);
    EXPECT_EQ(s1 - 6, s1);
    EXPECT_EQ(s1 - 1, QRSet<int>({2,3,4,5}));
    EXPECT_EQ(6 - s1, QRSet<int>{6});
    EXPECT_EQ(1 - s1, QRSet<int>({}));
}

TEST(QRSetLogic2Tests, sym_difference)
{
    QRSet<int> s1 = {1,2,3,4,5};
    QRSet<int> s2 = {3,4,5,6,7,8,9,10};

    EXPECT_EQ(s1 ^ s2, QRSet<int>({1,2,6,7,8,9,10}));
    EXPECT_EQ(s1 ^ s1, QRSet<int>());
    EXPECT_EQ(s1 ^ QRSet<int>(), s1);
    EXPECT_EQ(s1 ^ 6, QRSet<int>({1,2,3,4,5,6}));
    EXPECT_EQ(s1 ^ 1, QRSet<int>({2,3,4,5}));
    EXPECT_EQ(6 ^ s1, QRSet<int>({1,2,3,4,5,6}));
    EXPECT_EQ(1 ^ s1, QRSet<int>({2,3,4,5}));
}


TEST(QRSetLogic2Tests, join_update)
{
    QRSet<int> s1 = {1,2,3,4,5}, s2(s1);
    QRSet<int> s3 = {3,4,5,6,7,8,9,10};

    s1 |= s3;
    s2.join_update(s3);

    EXPECT_EQ(s1,QRSet<int>({1,2,3,4,5,6,7,8,9,10}));
    EXPECT_EQ(s2, s1);

    s1 = {1,2,3,4,5};
    s1 |= 6;
    EXPECT_EQ(s1,QRSet<int>({1,2,3,4,5,6}));
    s1 |= 1;
    EXPECT_EQ(s1,QRSet<int>({1,2,3,4,5,6}));
}

TEST(QRSetLogic2Tests, intersection_update)
{
    QRSet<int> s1 = {1,2,3,4,5}, s2(s1);
    QRSet<int> s3 = {3,4,5,6,7,8,9,10};

    s1 &= s3;
    s2.intersection_update(s3);

    EXPECT_EQ(s1,QRSet<int>({3,4,5}));
    EXPECT_EQ(s2, s1);

    s1 = {1,2,3,4,5};
    s1 &= 1;
    EXPECT_EQ(s1,QRSet<int>({1}));
    s1 &= 6;
    EXPECT_EQ(s1,QRSet<int>({}));
}

TEST(QRSetLogic2Tests, difference_update)
{
    QRSet<int> s1 = {1,2,3,4,5}, s2(s1);
    QRSet<int> s3 = {3,4,5,6,7,8,9,10};

    s1-= s3;
    s2.difference_update(s3);

    EXPECT_EQ(s1,QRSet<int>({1,2}));
    EXPECT_EQ(s2, s1);

    s1 = {1,2,3,4,5};
    s1 -= 6;
    EXPECT_EQ(s1,QRSet<int>({1,2,3,4,5}));
    s1 -= 1;
    EXPECT_EQ(s1,QRSet<int>({2,3,4,5}));
}

TEST(QRSetLogic2Tests, sym_difference_update)
{
    QRSet<int> s1 = {1,2,3,4,5}, s2(s1);
    QRSet<int> s3 = {3,4,5,6,7,8,9,10};

    s1 ^= s3;
    s2.sym_difference_update(s3);

    EXPECT_EQ(s1,QRSet<int>({1,2,6,7,8,9,10}));
    EXPECT_EQ(s2, s1);

    s1 = {1,2,3,4,5};
    s1 ^= 6;
    EXPECT_EQ(s1,QRSet<int>({1,2,3,4,5,6}));
    s1 ^= 1;
    EXPECT_EQ(s1,QRSet<int>({2,3,4,5,6}));
}

TEST(QRSetLogic2Tests, subset_lesseq)
{
    QRSet<int> s1 = {1,2,3,4,5}, s2(s1);
    QRSet<int> s3 = {3,4,5,6,7,8};
    QRSet<int> s4 = {1,3,4};
    QRSet<int> s5 = {5};
    QRSet<int> s6;

    EXPECT_EQ(s1.isSubSet(s3), false);
    EXPECT_EQ(s1 <= s3, false);
    EXPECT_EQ(s4.isSubSet(s3), false);
    EXPECT_EQ(s4 <= s3, false);
    EXPECT_EQ(s4.isSubSet(s1), true);
    EXPECT_EQ(s4 <= s1, true);
    EXPECT_EQ(s1.isSubSet(s1), true);
    EXPECT_EQ(s1 <= s1, true);
    EXPECT_EQ(s1.isSubSet(s2), true);
    EXPECT_EQ(s1 <= s2, true);
    EXPECT_EQ(s5.isSubSet(s3), true);
    EXPECT_EQ(s5 <= s3, true);
    EXPECT_EQ(s6.isSubSet(s3), true);
    EXPECT_EQ(s6 <= s3, true);
}

TEST(QRSetLogic2Tests, superset_greatereq)
{
    QRSet<int> s1 = {1,2,3,4,5}, s2(s1);
    QRSet<int> s3 = {3,4,5,6,7,8};
    QRSet<int> s4 = {1,3,4};
    QRSet<int> s5 = {5};
    QRSet<int> s6;

    EXPECT_EQ(s1.isSuperSet(s3), false);
    EXPECT_EQ(s1 >= s3, false);
    EXPECT_EQ(s4.isSuperSet(s3), false);
    EXPECT_EQ(s4 >= s3, false);
    EXPECT_EQ(s4.isSuperSet(s1), false);
    EXPECT_EQ(s4 >= s1, false);
    EXPECT_EQ(s1.isSuperSet(s1), true);
    EXPECT_EQ(s1 >= s1, true);
    EXPECT_EQ(s1.isSuperSet(s2), true);
    EXPECT_EQ(s1 >= s2, true);
    EXPECT_EQ(s5.isSuperSet(s3), false);
    EXPECT_EQ(s5 >= s3, false);
    EXPECT_EQ(s6.isSuperSet(s3), false);
    EXPECT_EQ(s6 >= s3, false);
    EXPECT_EQ(s1.isSuperSet(s4), true);
    EXPECT_EQ(s1 >= s4, true);
}

TEST(QRSetComplexOperations, single)
{
    QRSet<int> s1 = {1,2,3,4,5}, s2(s1);
    QRSet<int> s3 = {3,4,5,6,7,8};
    QRSet<int> s4 = {1,3,4};
    QRSet<int> s5 = {5};
    QRSet<int> s6;

    EXPECT_EQ(s1 & s4 | s3 , QRSet<int>({1,3,4,5,6,7,8}));
    EXPECT_EQ(s1 & s3 & (s5 | 4 | 9), QRSet<int>({4,5}));
}


TEST(QRSetVisualTests, print)
{
    QRSet<int> s0 = {1,2,3,4,5,6,7,8,9,10};
    cout << s0 << '\n';

    QRSet<int> s1 = {1,2,3,4,5};
    cout << s1 << '\n';

    QRSet<char> s2 = {'a', 'b', 'c', 'd'};
    cout << s2 <<'\n';

    QRSet<string> s3 = {"abc", "xyz", "qwerty"};
    cout << s3 <<'\n';
}

TEST(QRSetVisualTests, printException)
{
    QRSet<int> s0 = {1,2,3,4,5,6,7,8,9,10};
    time_t t = time(NULL);
    try {
        throw QRBadAllocException(__FILE__, __LINE__, asctime(localtime(&t)), "QRSet failed to create an array!", 1024);
    }
    catch (QRBadAllocException &exc) {
        cerr << exc.what() << '\n';
    };

    try {
        throw QRSetExceptionBadPointer(__FILE__, __LINE__, asctime(localtime(&t)), "QRSet failed to create from array!");
    }
    catch (QRSetExceptionBadPointer &exc) {
        cerr << exc.what() << '\n';
    };
}