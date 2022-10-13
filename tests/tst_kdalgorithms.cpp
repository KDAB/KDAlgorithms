/****************************************************************************
**
** This file is part of the KDAlgorithms
**
** SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "../src/kdalgorithms.h"
#include "ContainerObserver.h"
#include "copy_observer.h"
#include <QList>
#include <QTest>
#include <QVector>
#include <algorithm>
#include <deque>
#include <forward_list>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace {
bool isOdd(int x)
{
    return x % 2 == 1;
}

int squareItem(int x)
{
    return x * x;
}

QString toString(int x)
{
    return QString::number(x);
}

std::function<bool(int)> greaterThan(int testValue)
{
    return [testValue](int value) { return value > testValue; };
}

const std::vector<int> emptyIntVector;
const std::vector<int> intVector{1, 2, 3, 4};

std::vector<int> getIntVector()
{
    return std::vector<int>{1, 2, 3, 4};
}

struct Struct
{
    int key;
    int value;
    bool operator==(const Struct &other) const { return key == other.key && value == other.value; }
    bool isKeyGreaterThanValue() const { return key > value; }
    bool lessThanByXY(const Struct &other) const
    {
        if (key != other.key)
            return key < other.key;
        else
            return value < other.value;
    }
    bool hasEqualKeys(const Struct &other) const { return key == other.key; }
    bool hasEqualValues(const Struct &other) const { return value == other.value; }
    bool hasEqualKeyValuePair() const { return key == value; }
    int sumPairs() const { return key + value; }
};

const std::vector<Struct> structVec{{1, 4}, {2, 3}, {3, 2}, {4, 1}};

} // namespace
class TestAlgorithms : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void copy();
    void copyAsMove();
    void filteredChangeContainer();
    void filteredSameContainer();
    void filteredAsMove();
    void filterWithMemberFunction();
    void filterOtherContainers();
    void filter();
    void transformedChangeContainer();
    void transformedSameContainer();
    void transformedFullContainerTypeSpecified();
    void transformedChangeContainerAndDataType();
    void transformedChangeContainerAndDataType2();
    void transformedChangeDataType();
    void transformedWithRValue();
    void transformMemberFunction();
    void transformOtherContainers();
    void transform();
    void anyOf();
    void allOf();
    void noneOf();
    void anyAllNoneOtherContainers();
    void reverse();
    void reversed();
    void reversedEnsureMoveOnly();
    void sort();
    void sorted();
    void sortWithCompare();
    void sortedWithCompare();
    void sortedEnsureMoveOnly();
    void is_sorted();
    void lvalue();
    void contains();
    void value_in();
    void count();
    void count_if();
    void max();
    void min();
    void maxValueLessThan();
    void maxValueLessThanCustomComparisor();
    void minValueGreaterThan();
    void minValueGreaterThanCustomComparisor();
    void isPermutation();
    void accumulate();
    void accumulateWithInitialValue();
    void accumulateDifferentReturnType();
    void accumulateWithAuto();
    void accumulateWithMap();
    void get_first_match();
    void get_first_match_or_default();
    void remove_duplicates();
    void has_duplicates();
    void has_duplicates_data();
    void erase();
    void erase_if();
    void combiningTests();
    void index_of_match();
};

void TestAlgorithms::copy()
{
    // normal copy vector -> QVector
    {
        QVector<int> result;
        kdalgorithms::copy(intVector, result);
        QVector<int> expected{1, 2, 3, 4};
        QCOMPARE(result, expected);
    }

    // std::list doesn't have a reserve method
    {
        std::list<int> list;
        kdalgorithms::copy(intVector, list);
        std::list<int> expected{1, 2, 3, 4};
        QCOMPARE(list, expected);
    }

    // deque
    {
        std::deque<int> from{1, 2, 3, 4};
        std::deque<int> to;
        kdalgorithms::copy(from, to);
        QCOMPARE(from, to);
    }

    // set
    {
        std::set<int> from{1, 2, 3, 4, 1, 3};
        std::set<int> to;
        kdalgorithms::copy(from, to);
        QCOMPARE(from, to);
    }

    // unordered_set
    {
        std::unordered_set<int> from{1, 2, 3, 4, 1, 3};
        std::unordered_set<int> to;
        kdalgorithms::copy(from, to);
        QCOMPARE(from, to);
    }

    // multiset
    {
        std::multiset<int> from{1, 2, 3, 4, 1, 3};
        std::multiset<int> to;
        kdalgorithms::copy(from, to);
        QCOMPARE(from, to);
    }

    // unordered_multiset
    {
        std::unordered_multiset<int> from{1, 2, 3, 4, 1, 3};
        std::unordered_multiset<int> to;
        kdalgorithms::copy(from, to);
        QCOMPARE(from, to);
    }

    // map
    {
        std::map<int, int> from{{1, 2}, {2, 3}, {4, 5}};
        std::map<int, int> to;
        kdalgorithms::copy(from, to);
        QCOMPARE(from, to);
    }

    // multimap
    {
        std::multimap<int, int> from{{1, 2}, {2, 3}, {1, 3}, {4, 5}};
        std::multimap<int, int> to;
        kdalgorithms::copy(from, to);
        QCOMPARE(from, to);
    }

    // unordered_map
    {
        std::unordered_map<int, int> from{{1, 2}, {2, 3}, {4, 5}};
        std::unordered_map<int, int> to;
        kdalgorithms::copy(from, to);
        QCOMPARE(from, to);
    }

    // unordered_multimap
    {
        std::unordered_multimap<int, int> from{{1, 2}, {2, 3}, {1, 3}, {4, 5}};
        std::unordered_multimap<int, int> to;
        kdalgorithms::copy(from, to);
        QCOMPARE(from, to);
    }

    // std::vector to std::set
    {
        std::set<int> set;
        kdalgorithms::copy(intVector, set);
        std::set<int> expected{1, 2, 3, 4};
        QCOMPARE(set, expected);
    }

    // QSet
    {
        QSet<int> from{1, 2, 3, 4, 1, 3};
        QSet<int> to;
        kdalgorithms::copy(from, to);
        QCOMPARE(from, to);
    }

    // QMap
    {
        QMap<int, int> from{{1, 2}, {2, 3}, {4, 5}};
        QMap<int, int> to;
        kdalgorithms::copy(from, to);
        QCOMPARE(from, to);
    }

    // QHash
    {
        QHash<int, int> from{{1, 2}, {2, 3}, {4, 5}};
        QHash<int, int> to;
        kdalgorithms::copy(from, to);
        QCOMPARE(from, to);
    }
}

std::vector<CopyObserver> getObserverVector()
{
    std::vector<CopyObserver> vec;
    vec.emplace_back(1);
    vec.emplace_back(2);
    vec.emplace_back(3);
    return vec;
}

const std::vector<CopyObserver> getConstObserverVector()
{
    return getObserverVector();
}

void TestAlgorithms::copyAsMove()
{
    std::vector<CopyObserver> result;
    CopyObserver::reset();
    kdalgorithms::copy(getObserverVector(), result);
    QCOMPARE(CopyObserver::copies, 0);

    CopyObserver::reset();
    kdalgorithms::copy(getConstObserverVector(), result);
    QCOMPARE(CopyObserver::copies, 3);
}

void TestAlgorithms::filteredSameContainer()
{
    auto result = kdalgorithms::filtered(intVector, isOdd);
    std::vector<int> expected{1, 3};
    QCOMPARE(result, expected);

    // source is an r-value
    result = kdalgorithms::filtered(getIntVector(), isOdd);
    QCOMPARE(result, expected);
}

void TestAlgorithms::filteredChangeContainer()
{
    auto result = kdalgorithms::filtered<QVector>(intVector, isOdd);
    QVector<int> expected{1, 3};
    QCOMPARE(result, expected);

    // source is an r-value
    result = kdalgorithms::filtered<QVector>(getIntVector(), isOdd);
    QCOMPARE(result, expected);
}

void TestAlgorithms::filteredAsMove()
{
    auto isOdd = [](const CopyObserver &observer) { return ::isOdd(observer.value); };

    // First with the filtered method maintaining the container type.
    {
        CopyObserver::reset();
        auto result = kdalgorithms::filtered(getObserverVector(), isOdd);
        QCOMPARE(CopyObserver::copies, 0);
        std::vector<CopyObserver> expected{1, 3};
        QCOMPARE(result, expected);

        CopyObserver::reset();
        result = kdalgorithms::filtered(getConstObserverVector(), isOdd);
        QCOMPARE(CopyObserver::copies, 2);
        QCOMPARE(result, expected);
    }

    // filtered version changing the container type.
    {
        CopyObserver::reset();
        auto result = kdalgorithms::filtered<QVector>(getObserverVector(), isOdd);
        QCOMPARE(CopyObserver::copies, 0);
        QVector<CopyObserver> expected{1, 3};
        QCOMPARE(result, expected);

        CopyObserver::reset();
        result = kdalgorithms::filtered<QVector>(getConstObserverVector(), isOdd);
        QCOMPARE(CopyObserver::copies, 2);
        QCOMPARE(result, expected);
    }
}

void TestAlgorithms::filterWithMemberFunction()
{
    std::vector<Struct> vec{{1, 2}, {2, 1}, {3, 3}, {4, 4}};
    {
        auto result = kdalgorithms::filtered(vec, &Struct::hasEqualKeyValuePair);
        std::vector<Struct> expected{{3, 3}, {4, 4}};
        QCOMPARE(result, expected);
    }

    {
        auto result = kdalgorithms::filtered<std::list>(vec, &Struct::hasEqualKeyValuePair);
        std::list<Struct> expected{{3, 3}, {4, 4}};
        QCOMPARE(result, expected);
    }
}

void TestAlgorithms::filterOtherContainers()
{
    {
        auto result = kdalgorithms::filtered<std::set>(intVector, isOdd);
        std::set<int> expected{1, 3};
        QCOMPARE(result, expected);
    }

    {
        auto result = kdalgorithms::filtered<QSet>(intVector, isOdd);
        QSet<int> expected{1, 3};
        QCOMPARE(result, expected);
    }

    {
        std::map<int, std::string> map{{1, "abc"}, {2, "def"}, {3, "hij"}, {4, "klm"}};
        auto hasOddKey = [](auto item) { return item.first % 2 == 1; };
        auto result = kdalgorithms::filtered(map, hasOddKey);
        std::map<int, std::string> expected{{1, "abc"}, {3, "hij"}};
        QCOMPARE(result, expected);
    }

    {
        QMap<int, std::string> map{{1, "abc"}, {2, "def"}, {3, "hij"}, {4, "klm"}};
        auto hasOddKey = [](auto item) { return item.first % 2 == 1; };
        auto result = kdalgorithms::filtered(map, hasOddKey);
        QMap<int, std::string> expected{{1, "abc"}, {3, "hij"}};
        QCOMPARE(result, expected);
    }
}

void TestAlgorithms::filter()
{
    std::vector<int> vec{1, 2, 3, 4};
    kdalgorithms::filter(vec, isOdd);
    std::vector<int> expected{1, 3};
    QCOMPARE(vec, expected);
}

void TestAlgorithms::transformedChangeContainer()
{
    auto result = kdalgorithms::transformed<QVector>(intVector, squareItem);
    QVector<int> expected{1, 4, 9, 16};
    QCOMPARE(result, expected);

    // r-value
    result = kdalgorithms::transformed<QVector>(getIntVector(), squareItem);
    QCOMPARE(result, expected);
}

void TestAlgorithms::transformedSameContainer()
{
    auto result = kdalgorithms::transformed(intVector, squareItem);
    std::vector<int> expected{1, 4, 9, 16};
    QCOMPARE(result, expected);

    // r-value
    result = kdalgorithms::transformed(getIntVector(), squareItem);
    QCOMPARE(result, expected);
}

void TestAlgorithms::transformedFullContainerTypeSpecified()
{
    {
        auto result = kdalgorithms::transformed<QStringList>(intVector, toString);
        QStringList expected{"1", "2", "3", "4"};
        QCOMPARE(result, expected);
    }

    {
        auto result = kdalgorithms::transformed<std::list<int>>(structVec, &Struct::sumPairs);
        std::list<int> expected{5, 5, 5, 5};
        QCOMPARE(result, expected);
    }
}

void TestAlgorithms::transformedChangeContainerAndDataType()
{
    auto result = kdalgorithms::transformed<QVector>(intVector, toString);
    QVector<QString> expected{"1", "2", "3", "4"};
    QCOMPARE(result, expected);
}

void TestAlgorithms::transformedChangeContainerAndDataType2()
{
    QVector<int> vec{1, 2, 3, 4};
    const auto toString = [](int i) { return QString::number(i); };
    auto result = kdalgorithms::transformed<std::vector>(vec, toString);
    std::vector<QString> expected{"1", "2", "3", "4"};
    QCOMPARE(result, expected);
}

void TestAlgorithms::transformedChangeDataType()
{
    auto result = kdalgorithms::transformed(intVector, toString);
    std::vector<QString> expected{"1", "2", "3", "4"};
    QCOMPARE(result, expected);
}

void TestAlgorithms::transformedWithRValue()
{
    using Container = ContainerObserver<int>;
    auto create = [] {
        Container container;
        container.push_back(1);
        container.push_back(2);
        container.push_back(3);
        return container;
    };

    {
        // l-value must create a new container
        Container container = create();
        Container::reset();
        Container result = kdalgorithms::transformed(container, squareItem);
        QCOMPARE(result.at(0), 1);
        QCOMPARE(result.at(1), 4);
        QCOMPARE(result.at(2), 9);
        QCOMPARE(Container::copies, 1);
    }

    {
        // r-value same container can reuse the container
        Container container = create();
        Container::reset();
        Container result = kdalgorithms::transformed(std::move(container), squareItem);
        QCOMPARE(result.at(0), 1);
        QCOMPARE(result.at(1), 4);
        QCOMPARE(result.at(2), 9);
        QCOMPARE(Container::copies, 0);
    }

    {
        // r-value but different container must create a new one
        Container container = create();
        Container::reset();
        QVector<int> result = kdalgorithms::transformed<QVector>(std::move(container), squareItem);
        QCOMPARE(result.at(0), 1);
        QCOMPARE(result.at(1), 4);
        QCOMPARE(result.at(2), 9);
        // We obviously wont create a new ContainerObserver, but a QVector instead, so this is
        // just to test that that code path works too.
    }
}

void TestAlgorithms::transformMemberFunction()
{
    {
        auto result = kdalgorithms::transformed(structVec, &Struct::sumPairs);
        std::vector<int> expected{5, 5, 5, 5};
        QCOMPARE(result, expected);
    }

    {
        auto result = kdalgorithms::transformed<std::list>(structVec, &Struct::sumPairs);
        std::list<int> expected{5, 5, 5, 5};
        QCOMPARE(result, expected);
    }
}

void TestAlgorithms::transformOtherContainers()
{
    {
        auto result = kdalgorithms::transformed<std::set>(intVector, toString);
        std::set<QString> expected{"1", "2", "3", "4"};
        QCOMPARE(result, expected);
    }

    {
        auto result = kdalgorithms::transformed<QSet>(intVector, toString);
        QSet<QString> expected{"1", "2", "3", "4"};
        QCOMPARE(result, expected);
    }

    // This unfortunately doesn't work as I have no way to deduce the result type of the transform
    // to match a container We would end up with std::vector<std::map<std::pair<int,string>>
    // instead we now have kdalgorithms::transformed_to_same_container
    /*
    {
        std::map<int, std::string> map{{1, "abc"}, {2, "def"}, {3, "hij"}, {4, "klm"}};
        auto doubleKeys = [](const std::pair<int, std::string> &item) {
            return std::pair(item.first * 2, item.second);
        };
        auto result = kdalgorithms::transformed(map, doubleKeys);
        std::map<int, std::string> expected{{1, "abc"}, {4, "def"}, {6, "hij"}, {8, "klm"}};
        QCOMPARE(result, expected);
    }
    */
    {
        std::map<int, std::string> map{{1, "abc"}, {2, "def"}, {3, "hij"}, {4, "klm"}};
        auto doubleKeys = [](const auto &item) {
            return std::make_pair(item.first * 2, item.second);
        };
        auto result = kdalgorithms::transformed_to_same_container(map, doubleKeys);
        std::map<int, std::string> expected{{2, "abc"}, {4, "def"}, {6, "hij"}, {8, "klm"}};
        QCOMPARE(result, expected);
    }

    {
        QMap<int, std::string> map{{1, "abc"}, {2, "def"}, {3, "hij"}, {4, "klm"}};
        auto doubleKeys = [](const auto &item) {
            return std::make_pair(item.first * 2, item.second);
        };
        auto result = kdalgorithms::transformed_to_same_container(map, doubleKeys);
        QMap<int, std::string> expected{{2, "abc"}, {4, "def"}, {6, "hij"}, {8, "klm"}};
        QCOMPARE(result, expected);
    }

    {
        auto squareItem = [](int i) { return i * i; };
        auto result = kdalgorithms::transformed_to_same_container(intVector, squareItem);
        std::vector<int> expected{1, 4, 9, 16};
        QCOMPARE(result, expected);
    }

    {
        std::map<int, std::string> map{{1, "abc"}, {2, "def"}, {3, "hij"}, {4, "klm"}};
        auto result = kdalgorithms::transformed_with_new_return_type<std::map<std::string, int>>(
            map, [](const auto &item) { return std::make_pair(item.second, item.first); });
        std::map<std::string, int> expected{{"abc", 1}, {"def", 2}, {"hij", 3}, {"klm", 4}};
        QCOMPARE(result, expected);
    }

    {
        QMap<int, std::string> map{{1, "abc"}, {2, "def"}, {3, "hij"}, {4, "klm"}};
        auto result = kdalgorithms::transformed_with_new_return_type<QMap<std::string, int>>(
            map, [](const auto &item) { return std::make_pair(item.second, item.first); });
        QMap<std::string, int> expected{{"abc", 1}, {"def", 2}, {"hij", 3}, {"klm", 4}};
        QCOMPARE(result, expected);
    }

    {
        std::map<int, std::string> map{{1, "abc"}, {2, "abc"}, {3, "hij"}, {4, "klm"}};
        auto result =
            kdalgorithms::transformed_with_new_return_type<std::multimap<std::string, int>>(
                map, [](const auto &item) { return std::make_pair(item.second, item.first); });
        std::multimap<std::string, int> expected{{"abc", 1}, {"abc", 2}, {"hij", 3}, {"klm", 4}};
        QCOMPARE(result, expected);
    }
}

void TestAlgorithms::transform()
{
    std::vector<int> vec{1, 2, 3, 4};
    kdalgorithms::transform(vec, squareItem);
    std::vector<int> expected{1, 4, 9, 16};
    QCOMPARE(vec, expected);
}

void TestAlgorithms::anyOf()
{
    bool res = kdalgorithms::any_of(intVector, greaterThan(10));
    QCOMPARE(res, false);

    res = kdalgorithms::any_of(intVector, greaterThan(3));
    QCOMPARE(res, true);

    res = kdalgorithms::any_of(emptyIntVector, greaterThan(3));
    QCOMPARE(res, false);

    res = kdalgorithms::any_of(structVec, &Struct::isKeyGreaterThanValue);
    QCOMPARE(res, true);

    std::vector<Struct> vec{{1, 3}, {2, 4}, {3, 5}, {4, 6}};
    res = kdalgorithms::any_of(vec, &Struct::isKeyGreaterThanValue);
    QCOMPARE(res, false);
}

void TestAlgorithms::allOf()
{
    bool res = kdalgorithms::all_of(intVector, greaterThan(3));
    QCOMPARE(res, false);

    res = kdalgorithms::all_of(intVector, greaterThan(0));
    QCOMPARE(res, true);

    res = kdalgorithms::all_of(emptyIntVector, greaterThan(3));
    QCOMPARE(res, true);

    res = kdalgorithms::all_of(structVec, &Struct::isKeyGreaterThanValue);
    QCOMPARE(res, false);

    std::vector<Struct> vec{{3, 1}, {2, 1}, {3, 2}, {4, 1}};
    res = kdalgorithms::all_of(vec, &Struct::isKeyGreaterThanValue);
    QCOMPARE(res, true);
}

void TestAlgorithms::noneOf()
{
    bool res = kdalgorithms::none_of(intVector, greaterThan(3));
    QCOMPARE(res, false);

    res = kdalgorithms::none_of(intVector, greaterThan(10));
    QCOMPARE(res, true);

    res = kdalgorithms::none_of(emptyIntVector, greaterThan(3));
    QCOMPARE(res, true);

    res = kdalgorithms::none_of(structVec, &Struct::isKeyGreaterThanValue);
    QCOMPARE(res, false);

    std::vector<Struct> vec{{1, 3}, {2, 4}, {3, 5}, {4, 6}};
    res = kdalgorithms::none_of(vec, &Struct::isKeyGreaterThanValue);
    QCOMPARE(res, true);
}

void TestAlgorithms::anyAllNoneOtherContainers()
{
    auto has_key = [](int value) { return [value](auto item) { return item.first == value; }; };

    {
        std::map<int, std::string> map{{1, "abc"}, {2, "def"}, {3, "hij"}, {4, "klm"}};
        QVERIFY(kdalgorithms::any_of(map, has_key(2)));
        QVERIFY(!kdalgorithms::any_of(map, has_key(42)));
        QVERIFY(!kdalgorithms::all_of(map, has_key(2)));
        QVERIFY(kdalgorithms::none_of(map, has_key(42)));
        QVERIFY(!kdalgorithms::none_of(map, has_key(2)));
    }

    {
        QMap<int, std::string> map{{1, "abc"}, {2, "def"}, {3, "hij"}, {4, "klm"}};
        QVERIFY(kdalgorithms::any_of(map, has_key(2)));
        QVERIFY(!kdalgorithms::any_of(map, has_key(42)));
        QVERIFY(!kdalgorithms::all_of(map, has_key(2)));
        QVERIFY(kdalgorithms::none_of(map, has_key(42)));
        QVERIFY(!kdalgorithms::none_of(map, has_key(2)));
    }
}

void TestAlgorithms::reverse()
{
    std::vector<int> vec{1, 2, 3, 4};
    kdalgorithms::reverse(vec);
    std::vector<int> expected{4, 3, 2, 1};
    QCOMPARE(vec, expected);
}

void TestAlgorithms::reversed()
{
    auto result = kdalgorithms::reversed(intVector);
    std::vector<int> expected{4, 3, 2, 1};
    QCOMPARE(result, expected);
}

void TestAlgorithms::reversedEnsureMoveOnly()
{
    std::vector<CopyObserver> vec{1, 2, 3, 4, 5};
    CopyObserver::reset();
    auto result = kdalgorithms::reversed(std::move(vec));
    QCOMPARE(CopyObserver::copies, 0);

    // 3 for each pair I need to swap (1,5) (2,3)
    QCOMPARE(CopyObserver::moves, 6);
}

void TestAlgorithms::sort()
{
    std::vector<int> vec{3, 2, 4, 1};
    kdalgorithms::sort(vec);
    std::vector<int> expected{1, 2, 3, 4};
    QCOMPARE(vec, expected);
}

void TestAlgorithms::sorted()
{
    const std::vector<int> vec{3, 2, 4, 1};
    auto result = kdalgorithms::sorted(vec);
    std::vector<int> expected{1, 2, 3, 4};
    QCOMPARE(result, expected);
}

void TestAlgorithms::sortWithCompare()
{
    {
        std::vector<int> vec{3, 2, 4, 1};
        kdalgorithms::sort(vec, std::greater<int>());
        std::vector<int> expected{4, 3, 2, 1};
        QCOMPARE(vec, expected);
    }

    {
        std::vector<Struct> structVec{{1, 3}, {3, 4}, {3, 2}, {1, 2}};
        kdalgorithms::sort(structVec, &Struct::lessThanByXY);
        std::vector<Struct> expected{{1, 2}, {1, 3}, {3, 2}, {3, 4}};
        QCOMPARE(structVec, expected);
    }
}

void TestAlgorithms::sortedWithCompare()
{
    {
        const std::vector<int> vec{3, 2, 4, 1};
        auto result = kdalgorithms::sorted(vec, std::greater<int>());
        std::vector<int> expected{4, 3, 2, 1};
        QCOMPARE(result, expected);
    }

    {
        std::vector<Struct> structVec{{1, 3}, {3, 4}, {3, 2}, {1, 2}};
        auto result = kdalgorithms::sorted(structVec, &Struct::lessThanByXY);
        std::vector<Struct> expected{{1, 2}, {1, 3}, {3, 2}, {3, 4}};
        QCOMPARE(result, expected);
    }
}

void TestAlgorithms::sortedEnsureMoveOnly()
{
    std::vector<CopyObserver> vec{3, 2, 4, 1};
    CopyObserver::reset();
    kdalgorithms::sorted(std::move(vec));
    QCOMPARE(CopyObserver::copies, 0);
}

void TestAlgorithms::is_sorted()
{
    {
        QCOMPARE(kdalgorithms::is_sorted(intVector), true);
        QCOMPARE(kdalgorithms::is_sorted(std::vector<int>{1, 3, 2, 4}), false);
    }

    {
        auto byKey = [](const Struct &x, const Struct &y) { return x.key < y.key; };
        QCOMPARE(kdalgorithms::is_sorted(structVec, byKey), true);
        std::vector<Struct> unsorted{{2, 3}, {1, 4}, {3, 2}, {4, 1}};
        QCOMPARE(kdalgorithms::is_sorted(unsorted, byKey), false);
    }

    {
        std::vector<Struct> sorted{{1, 2}, {1, 3}, {3, 2}, {3, 4}};
        QCOMPARE(kdalgorithms::is_sorted(sorted, &Struct::lessThanByXY), true);
    }

    {
        std::vector<Struct> sorted{{1, 4}, {1, 3}, {3, 2}, {3, 4}};
        QCOMPARE(kdalgorithms::is_sorted(sorted, &Struct::lessThanByXY), false);
    }
}

void TestAlgorithms::lvalue()
{
    const auto isOddLValue = [](int i) { return i % 2 == 1; };
    auto result = kdalgorithms::filtered(intVector, isOddLValue);
    std::vector<int> expected{1, 3};
    QCOMPARE(result, expected);
}

void TestAlgorithms::contains()
{
    auto result = kdalgorithms::contains(intVector, 3);
    QCOMPARE(result, true);

    // Test an lvalue
    int value = 3;
    result = kdalgorithms::contains(intVector, value);
    QCOMPARE(result, true);

    result = kdalgorithms::contains(intVector, -1);
    QCOMPARE(result, false);

    result = kdalgorithms::contains(emptyIntVector, 3);
    QCOMPARE(result, false);

    result = kdalgorithms::contains({1, 2, 3, 4}, 3);
    QCOMPARE(result, true);
}

void TestAlgorithms::value_in()
{
    auto result = kdalgorithms::value_in(3, {1, 2, 3, 4});
    QCOMPARE(result, true);

    int val = 3;
    result = kdalgorithms::value_in(val, {1, 2, 3, 4});
    QCOMPARE(result, true);

    result = kdalgorithms::value_in(QString("abc"), {"abc", "def", "hij"});
    QCOMPARE(result, true);
}

void TestAlgorithms::count()
{
    std::vector<int> vec{1, 2, 1, 3, 2, 1, 5};
    auto result = kdalgorithms::count(vec, 1);
    QCOMPARE(result, 3);
}

void TestAlgorithms::count_if()
{
    {
        std::vector<int> vec{1, 2, 1, 3, 2, 1, 5};
        auto result = kdalgorithms::count_if(vec, [](int i) { return i > 2; });
        QCOMPARE(result, 2);

        result = kdalgorithms::count_if(structVec, &Struct::isKeyGreaterThanValue);
        QCOMPARE(result, 2);
    }

    {
        std::map<int, std::string> map{{2, "abc"}, {4, "def"}, {6, "hij"}, {8, "klm"}};
        auto result = kdalgorithms::count_if(map, [](auto item) { return item.first > 5; });
        QCOMPARE(result, 2);
    }

    {
        QMap<int, std::string> map{{2, "abc"}, {4, "def"}, {6, "hij"}, {8, "klm"}};
        auto result = kdalgorithms::count_if(map, [](auto item) { return item.first > 5; });
        QCOMPARE(result, 2);
    }
}

void TestAlgorithms::max()
{
#if __cplusplus >= 201703L
    {
        std::vector<int> unsortedIts{4, 1, 3, 2};
        auto result = kdalgorithms::max_element(unsortedIts, std::less<int>());
        QCOMPARE(result.value(), 4);

        result = kdalgorithms::max_element(unsortedIts, std::greater<int>());
        QCOMPARE(result.value(), 1);

        result = kdalgorithms::max_element(emptyIntVector, std::less<int>());
        QVERIFY(!result.has_value());

        result = kdalgorithms::max_element(unsortedIts);
        QCOMPARE(result.value(), 4);
    }

    {
        auto result = kdalgorithms::max_element(structVec, &Struct::lessThanByXY);
        Struct expected{4, 1};
        QCOMPARE(*result, expected);
    }

    {
        std::map<int, int> map{{1, 1}, {2, 3}, {4, 2}};
        auto lessthan = [](auto x, auto y) { return x.second < y.second; };
        auto result = kdalgorithms::max_element(map, lessthan);
        std::pair<const int, int> expected{2, 3};
        QCOMPARE(*result, expected);
    }
#endif
}

void TestAlgorithms::min()
{
#if __cplusplus >= 201703L
    {
        auto result = kdalgorithms::min_element(intVector, std::less<int>());
        QCOMPARE(result.value(), 1);

        result = kdalgorithms::min_element(intVector, std::greater<int>());
        QCOMPARE(result.value(), 4);

        result = kdalgorithms::min_element(emptyIntVector, std::less<int>());
        QVERIFY(!result.has_value());

        result = kdalgorithms::min_element(intVector);
        QCOMPARE(result.value(), 1);
    }

    {
        auto result = kdalgorithms::min_element(structVec, &Struct::lessThanByXY);
        Struct expected{1, 4};
        QCOMPARE(*result, expected);
    }

#endif
}

void TestAlgorithms::maxValueLessThan()
{
#if __cplusplus >= 201703L
    auto result = kdalgorithms::max_value_less_than(intVector, 4);
    QCOMPARE(result.value(), 3);

    result = kdalgorithms::max_value_less_than(intVector, 100);
    QCOMPARE(result.value(), 4);

    result = kdalgorithms::max_value_less_than(intVector, 1);
    QVERIFY(!result.has_value());

    result = kdalgorithms::max_value_less_than(emptyIntVector, 10);
    QVERIFY(!result.has_value());

    std::set<int> set{1, 12, 3, 4, -23};
    result = kdalgorithms::max_value_less_than(set, 4);
    QCOMPARE(*result, 3);
#endif
}

void TestAlgorithms::maxValueLessThanCustomComparisor()
{
#if __cplusplus >= 201703L
    auto compare = [](const Struct &v1, const Struct &v2) { return v1.key < v2.key; };

    auto result = kdalgorithms::max_value_less_than(structVec, Struct{4, 4}, compare);
    Struct expected{3, 2};
    QCOMPARE(result.value(), expected);

    result = kdalgorithms::max_value_less_than(structVec, Struct{4, 4}, &Struct::lessThanByXY);
    expected = {4, 1};
    QCOMPARE(result.value(), expected);
#endif
}

void TestAlgorithms::minValueGreaterThan()
{
#if __cplusplus >= 201703L
    auto result = kdalgorithms::min_value_greater_than(intVector, 0);
    QCOMPARE(result.value(), 1);

    result = kdalgorithms::min_value_greater_than(intVector, 1);
    QCOMPARE(result.value(), 2);

    result = kdalgorithms::min_value_greater_than(intVector, 4);
    QVERIFY(!result.has_value());

    result = kdalgorithms::min_value_greater_than(emptyIntVector, 10);
    QVERIFY(!result.has_value());

    std::set<int> set{1, 12, 3, 4, -23};
    result = kdalgorithms::min_value_greater_than(set, 3);
    QCOMPARE(*result, 4);
#endif
}

void TestAlgorithms::minValueGreaterThanCustomComparisor()
{
#if __cplusplus >= 201703L
    auto compare = [](const Struct &v1, const Struct &v2) { return v1.key < v2.key; };

    auto result = kdalgorithms::min_value_greater_than(structVec, Struct{2, 1}, compare);
    Struct expected{3, 2};
    QCOMPARE(result.value(), expected);

    result = kdalgorithms::min_value_greater_than(structVec, Struct{2, 1}, &Struct::lessThanByXY);
    expected = {2, 3};
    QCOMPARE(result.value(), expected);
#endif
}

void TestAlgorithms::isPermutation()
{
    std::vector<int> permutedVector{4, 1, 3, 2};
    QVERIFY(kdalgorithms::is_permutation(intVector, permutedVector));

    std::vector<int> nonPermutedVector{1, 3, 2};
    QVERIFY(!kdalgorithms::is_permutation(intVector, nonPermutedVector));

    auto onKey = [](Struct x, Struct y) { return x.key == y.key; };
    auto onValue = [](Struct x, Struct y) { return x.value == y.value; };

    std::vector<Struct> v1{{1, 2}, {2, 3}, {3, 4}};
    std::vector<Struct> v2{{2, 4}, {3, 4}, {1, 2}};

    QVERIFY(kdalgorithms::is_permutation(v1, v2, onKey));
    QVERIFY(!kdalgorithms::is_permutation(v1, v2, onValue));

    std::vector<double> permutedDoubleVector{4.0, 1.0, 3.0, 2.0};
    QVERIFY(kdalgorithms::is_permutation(intVector, permutedDoubleVector));

    QVERIFY(kdalgorithms::is_permutation(v1, v2, &Struct::hasEqualKeys));
    QVERIFY(!kdalgorithms::is_permutation(v1, v2, &Struct::hasEqualValues));
}

void TestAlgorithms::accumulate()
{
    // Simple int function
    auto sumDoubles = [](int x, int y) { return x + y * y; };
    int result = kdalgorithms::accumulate(intVector, sumDoubles);
    QCOMPARE(result, 30);

    // simple string function
    auto slashBetween = [](const QString &x, const QString &y) {
        if (x.isEmpty())
            return y;
        else
            return x + "/" + y;
    };

    QStringList list{"abc", "def", "hij"};
    QString stringResult = kdalgorithms::accumulate(list, slashBetween);
    QCOMPARE(stringResult, "abc/def/hij");
}

void TestAlgorithms::accumulateWithInitialValue()
{
    auto factorial = [](int subTotal, int value) { return subTotal * value; };
    auto result = kdalgorithms::accumulate(intVector, factorial, 1);
    QCOMPARE(result, 24);
}

void TestAlgorithms::accumulateDifferentReturnType()
{
    auto toCommaSeperatedString = [](const QString &subTotal, int i) {
        return subTotal + "," + QString::number(i);
    };
    auto stringResult = kdalgorithms::accumulate(intVector, toCommaSeperatedString);
    QCOMPARE(stringResult, ",1,2,3,4");

    stringResult = kdalgorithms::accumulate(intVector, toCommaSeperatedString, QString("0"));
    QCOMPARE(stringResult, "0,1,2,3,4");
}

void TestAlgorithms::accumulateWithAuto()
{
    auto toCommaSeperatedString = [](auto subTotal, int i) {
        return subTotal + "," + QString::number(i);
    };
    auto stringResult = kdalgorithms::accumulate(intVector, toCommaSeperatedString, QString("0"));
    QCOMPARE(stringResult, "0,1,2,3,4");
}

void TestAlgorithms::accumulateWithMap()
{
    QMap<int, std::string> map{{1, "abc"}, {2, "def"}, {3, "hij"}};
    {
        auto sum = [](int subResult, const std::pair<int, std::string> &pair) {
            return subResult + pair.first;
        };
        auto result = kdalgorithms::accumulate(map, sum);
        QCOMPARE(result, 6);
    }
    {
        auto concatenate = [](const std::string &subResult,
                              const std::pair<int, std::string> &pair) {
            return subResult + "/" + pair.second;
        };
        auto result = kdalgorithms::accumulate(map, concatenate);
        QCOMPARE(result, "/abc/def/hij");
    }
}

void TestAlgorithms::get_first_match()
{
#if __cplusplus >= 201703L
    {
        auto wihtKey = [](int key) { return [key](const Struct &s) { return s.key == key; }; };

        auto value = kdalgorithms::get_first_match(structVec, wihtKey(2));
        Struct expected{2, 3};
        QCOMPARE(value.value(), expected);

        value = kdalgorithms::get_first_match(structVec, wihtKey(-1));
        QVERIFY(!value.has_value());
    }

    {
        std::vector<Struct> vec{{1, 2}, {2, 1}, {3, 3}, {4, 1}};
        auto value = kdalgorithms::get_first_match(vec, &Struct::hasEqualKeyValuePair);
        Struct expected{3, 3};
        QCOMPARE(value.value(), expected);
    }

    {
        std::map<int, int> map{{1, 2}, {2, 1}, {3, 3}, {4, 1}};
        auto valueEqualToKey = [](const auto &pair) { return pair.first == pair.second; };
        auto value = kdalgorithms::get_first_match(map, valueEqualToKey);
        std::pair<const int, int> expected{3, 3};
        QCOMPARE(value.value(), expected);
    }
#endif
}

void TestAlgorithms::get_first_match_or_default()
{
    auto withKey = [](int key) { return [key](const Struct &s) { return s.key == key; }; };

    auto value = kdalgorithms::get_first_match_or_default(structVec, withKey(2));
    Struct expected{2, 3};
    QCOMPARE(value, expected);

    value = kdalgorithms::get_first_match_or_default(structVec, withKey(-1));
    QCOMPARE(value, Struct{});

    Struct defaultValue{42, -42};
    value = kdalgorithms::get_first_match_or_default(structVec, withKey(-1), defaultValue);
    QCOMPARE(value, defaultValue);

    value = kdalgorithms::get_first_match_or_default(structVec, &Struct::hasEqualKeyValuePair,
                                                     defaultValue);
    QCOMPARE(value, defaultValue);

    {
        std::map<int, int> map{{1, 2}, {2, 1}, {13, 3}, {4, 1}};
        auto valueEqualToKey = [](const auto &pair) { return pair.first == pair.second; };
        auto value = kdalgorithms::get_first_match_or_default(map, valueEqualToKey);
        std::pair<const int, int> expected{0, 0};
        QCOMPARE(value, expected);
    }
}

void TestAlgorithms::remove_duplicates()
{
    // unique only remove consecutive mismatches
    {
        std::vector<int> vec{3, 1, 2, 2, 1};
        auto it = std::unique(vec.begin(), vec.end());
        vec.erase(it);
        std::vector<int> expected{3, 1, 2, 1};
        QCOMPARE(vec, expected);
    }

    // Nothing to do
    {
        std::vector<int> vec{3, 1, 2, 4};
        auto expected = vec;
        auto count = kdalgorithms::remove_duplicates(vec, kdalgorithms::do_not_sort);
        QCOMPARE(vec, expected);
        QCOMPARE(count, 0);
    }

    // Do not sort, so like std::unique
    {
        std::vector<int> vec{3, 1, 2, 2, 1};
        auto count = kdalgorithms::remove_duplicates(vec, kdalgorithms::do_not_sort);
        std::vector<int> expected{3, 1, 2, 1};
        QCOMPARE(vec, expected);
        QCOMPARE(count, 1);
    }

    // Sort first
    {
        std::vector<int> vec{3, 1, 2, 2, 1};
        auto count = kdalgorithms::remove_duplicates(vec, kdalgorithms::do_sort);
        std::vector<int> expected{1, 2, 3};
        QCOMPARE(vec, expected);
        QCOMPARE(count, 2);
    }
}

void TestAlgorithms::has_duplicates()
{
    QFETCH(std::vector<int>, vec);
    QFETCH(bool, expected);
    QFETCH(bool, sort);
    QCOMPARE(
        kdalgorithms::has_duplicates(vec, sort ? kdalgorithms::do_sort : kdalgorithms::do_not_sort),
        expected);
}

void TestAlgorithms::has_duplicates_data()
{
    QTest::addColumn<std::vector<int>>("vec");
    QTest::addColumn<bool>("sort");
    QTest::addColumn<bool>("expected");

    QTest::newRow("empty") << std::vector<int>() << true << false;
    QTest::newRow("sorted unique") << std::vector<int>{1, 2, 3, 4} << false << false;
    QTest::newRow("sorted not unique") << std::vector<int>{1, 2, 3, 3, 4} << false << true;
    QTest::newRow("unsorted unique") << std::vector<int>{3, 1, 2, 4} << true << false;
    QTest::newRow("unsorted not unique") << std::vector<int>{3, 1, 3, 4} << true << true;
}

void TestAlgorithms::erase()
{
    // several duplicates to remove
    {
        std::vector<int> vec{1, 2, 1, 3};
        auto count = kdalgorithms::erase(vec, 1);
        std::vector<int> expected{2, 3};
        QCOMPARE(vec, expected);
        QCOMPARE(count, 2);
    }

    // Nothing to remove
    {
        std::vector<int> vec{1, 2, 1, 3};
        auto count = kdalgorithms::erase(vec, 42);
        std::vector<int> expected{1, 2, 1, 3};
        QCOMPARE(vec, expected);
        QCOMPARE(count, 0);
    }
}

void TestAlgorithms::erase_if()
{
    auto withKey = [](int key) { return [key](const Struct &s) { return s.key == key; }; };
    // several duplicates to remove
    {
        std::vector<Struct> vec{{2, 3}, {1, 4}, {2, 2}, {4, 1}};
        auto count = kdalgorithms::erase_if(vec, withKey(2));
        std::vector<Struct> expected{{1, 4}, {4, 1}};
        QCOMPARE(vec, expected);
        QCOMPARE(count, 2);
    }

    // Nothing to remove
    {
        std::vector<Struct> vec{{2, 3}, {1, 4}, {2, 2}, {4, 1}};
        auto expected = vec;
        auto count = kdalgorithms::erase_if(vec, withKey(42));
        QCOMPARE(vec, expected);
        QCOMPARE(count, 0);
    }

    {
        std::vector<Struct> vec{{2, 3}, {1, 1}, {2, 2}, {4, 1}};
        std::vector<Struct> expected = {{2, 3}, {4, 1}};
        auto count = kdalgorithms::erase_if(vec, &Struct::hasEqualKeyValuePair);
        QCOMPARE(vec, expected);
        QCOMPARE(count, 2);
    }
}

void TestAlgorithms::combiningTests()
{
    using namespace kdalgorithms::operators;
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const auto isOdd = [](int num) { return num % 2 == 1; };
    const auto isGreatherThan = [](int num) { return [num](int value) { return value > num; }; };
    const auto isDividableBy = [](int num) {
        return [num](int value) { return value % num == 0; };
    };
    auto result = kdalgorithms::filtered(vec, isOdd && isGreatherThan(5));
    std::vector<int> expected{7, 9};
    QCOMPARE(result, expected);

    result = kdalgorithms::filtered(vec, isOdd || isGreatherThan(5));
    expected = {1, 3, 5, 6, 7, 8, 9, 10};
    QCOMPARE(result, expected);

    result = kdalgorithms::filtered(vec, isOdd || !isGreatherThan(5));
    expected = {1, 2, 3, 4, 5, 7, 9};
    QCOMPARE(result, expected);

    result = kdalgorithms::filtered(vec, isOdd || (isGreatherThan(5) && !isDividableBy(3)));
    expected = {1, 3, 5, 7, 8, 9, 10};
    QCOMPARE(result, expected);

    result = kdalgorithms::filtered(vec, isOdd && isGreatherThan(5) && isDividableBy(3));
    expected = {9};
    QCOMPARE(result, expected);

    result = kdalgorithms::filtered(vec, isOdd && !isGreatherThan(5));
    expected = {1, 3, 5};
    QCOMPARE(result, expected);
}

void TestAlgorithms::index_of_match()
{
    auto result = kdalgorithms::index_of_match(intVector, isOdd);
    QCOMPARE(result, 0);

    result = kdalgorithms::index_of_match(structVec, &Struct::hasEqualKeyValuePair);
    QCOMPARE(result, -1);

    std::vector<Struct> vec{{1, 2}, {2, 1}, {3, 3}, {4, 4}};
    result = kdalgorithms::index_of_match(vec, &Struct::hasEqualKeyValuePair);
    QCOMPARE(result, 2);

    std::map<int, int> map{{1, 2}, {3, 2}, {4, 5}};
    result = kdalgorithms::index_of_match(
        map, [](const auto &pair) { return pair.first > pair.second; });
    QCOMPARE(result, 1);

    QMap<int, int> qmap{{1, 2}, {3, 2}, {4, 5}};
    result = kdalgorithms::index_of_match(
        qmap, [](const auto &pair) { return pair.first > pair.second; });
    QCOMPARE(result, 1);
}

QTEST_MAIN(TestAlgorithms)

#include "tst_kdalgorithms.moc"
