#include "../src/KDAlgorithms.h"
#include "ContainerObserver.h"
#include "CopyObserver.h"
#include <QList>
#include <QTest>
#include <QVector>
#include <algorithm>
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
};

std::vector<Struct> structVec{{1, 4}, {2, 3}, {3, 2}, {4, 1}};

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
    void filter();
    void transformedChangeContainer();
    void transformedSameContainer();
    void transformedChangeContainerAndDataType();
    void transformedChangeContainerAndDataType2();
    void transformedChangeDataType();
    void transformedWithRValue();
    void transform();
    void anyOf();
    void allOf();
    void noneOf();
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
    void count();
    void count_if();
    void max();
    void min();
    void maxValueLessThan();
    void maxValueLessThanCustomComparisor();
    void minValueGreaterThan();
    void isPermutation();
    void accumulate();
    void accumulateWithInitialValue();
    void accumulateDifferentReturnType();
    void accumulateWithAuto();
    void get_first_match();
    void get_first_match_or_default();
    void remove_duplicates();
    void has_duplicates();
    void has_duplicates_data();
    void remove();
    void remove_if();
};

void TestAlgorithms::copy()
{
    // normal copy vector -> QVector
    {
        QVector<int> result;
        KDAlgorithms::copy(intVector, result);
        QVector<int> expected{1, 2, 3, 4};
        QCOMPARE(result, expected);
    }

    // std::list doesn't have a reserve method
    {
        std::list<int> list;
        KDAlgorithms::copy(intVector, list);
        std::list<int> expected{1, 2, 3, 4};
        QCOMPARE(list, expected);
    }

#if 0 // Doesn't compile on mac
    // This is the equivalent when using std::ranges
    {
        QVector<int> result;
        std::ranges::copy(intVector, std::back_inserter(result));
        QVector<int> expected{1, 2, 3, 4};
        QCOMPARE(result, expected);
    }
#endif
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
    KDAlgorithms::copy(getObserverVector(), result);
    QCOMPARE(CopyObserver::copies, 0);

    CopyObserver::reset();
    KDAlgorithms::copy(getConstObserverVector(), result);
    QCOMPARE(CopyObserver::copies, 3);
}

void TestAlgorithms::filteredSameContainer()
{
    auto result = KDAlgorithms::filtered(intVector, isOdd);
    std::vector<int> expected{1, 3};
    QCOMPARE(result, expected);

    // source is an r-value
    result = KDAlgorithms::filtered(getIntVector(), isOdd);
    QCOMPARE(result, expected);
}

void TestAlgorithms::filteredChangeContainer()
{
    auto result = KDAlgorithms::filtered<QVector>(intVector, isOdd);
    QVector<int> expected{1, 3};
    QCOMPARE(result, expected);

    // source is an r-value
    result = KDAlgorithms::filtered<QVector>(getIntVector(), isOdd);
    QCOMPARE(result, expected);
}

void TestAlgorithms::filteredAsMove()
{
    auto isOdd = [](const CopyObserver &observer) { return ::isOdd(observer.value); };

    // First with the filtered method maintaining the container type.
    {
        CopyObserver::reset();
        auto result = KDAlgorithms::filtered(getObserverVector(), isOdd);
        QCOMPARE(CopyObserver::copies, 0);
        std::vector<CopyObserver> expected{1, 3};
        QCOMPARE(result, expected);

        CopyObserver::reset();
        result = KDAlgorithms::filtered(getConstObserverVector(), isOdd);
        QCOMPARE(CopyObserver::copies, 2);
        QCOMPARE(result, expected);
    }

    // filtered version changing the container type.
    {
        CopyObserver::reset();
        auto result = KDAlgorithms::filtered<QVector>(getObserverVector(), isOdd);
        QCOMPARE(CopyObserver::copies, 0);
        QVector<CopyObserver> expected{1, 3};
        QCOMPARE(result, expected);

        CopyObserver::reset();
        result = KDAlgorithms::filtered<QVector>(getConstObserverVector(), isOdd);
        QCOMPARE(CopyObserver::copies, 2);
        QCOMPARE(result, expected);
    }
}

void TestAlgorithms::filter()
{
    std::vector<int> vec{1, 2, 3, 4};
    KDAlgorithms::filter(vec, isOdd);
    std::vector<int> expected{1, 3};
    QCOMPARE(vec, expected);
}

void TestAlgorithms::transformedChangeContainer()
{
    auto result = KDAlgorithms::transformed<QVector>(intVector, squareItem);
    QVector<int> expected{1, 4, 9, 16};
    QCOMPARE(result, expected);

    // r-value
    result = KDAlgorithms::transformed<QVector>(getIntVector(), squareItem);
    QCOMPARE(result, expected);
}

void TestAlgorithms::transformedSameContainer()
{
    auto result = KDAlgorithms::transformed(intVector, squareItem);
    std::vector<int> expected{1, 4, 9, 16};
    QCOMPARE(result, expected);

    // r-value
    result = KDAlgorithms::transformed(getIntVector(), squareItem);
    QCOMPARE(result, expected);
}

void TestAlgorithms::transformedChangeContainerAndDataType()
{
    auto result = KDAlgorithms::transformed<QVector>(intVector, toString);
    QVector<QString> expected{"1", "2", "3", "4"};
    QCOMPARE(result, expected);
}

void TestAlgorithms::transformedChangeContainerAndDataType2()
{
    QVector<int> vec{1, 2, 3, 4};
    const auto toString = [](int i) { return QString::number(i); };
    auto result = KDAlgorithms::transformed<std::vector>(vec, toString);
    std::vector<QString> expected{"1", "2", "3", "4"};
    QCOMPARE(result, expected);
}

void TestAlgorithms::transformedChangeDataType()
{
    auto result = KDAlgorithms::transformed(intVector, toString);
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
        Container result = KDAlgorithms::transformed(container, squareItem);
        QCOMPARE(result.at(0), 1);
        QCOMPARE(result.at(1), 4);
        QCOMPARE(result.at(2), 9);
        QCOMPARE(Container::copies, 1);
    }

    {
        // r-value same container can reuse the container
        Container container = create();
        Container::reset();
        Container result = KDAlgorithms::transformed(std::move(container), squareItem);
        QCOMPARE(result.at(0), 1);
        QCOMPARE(result.at(1), 4);
        QCOMPARE(result.at(2), 9);
        QCOMPARE(Container::copies, 0);
    }

    {
        // r-value but different container must create a new one
        Container container = create();
        Container::reset();
        QVector<int> result = KDAlgorithms::transformed<QVector>(std::move(container), squareItem);
        QCOMPARE(result.at(0), 1);
        QCOMPARE(result.at(1), 4);
        QCOMPARE(result.at(2), 9);
        // We obviously wont create a new ContainerObserver, but a QVector instead, so this is just
        // to test that that code path works too.
    }
}

void TestAlgorithms::transform()
{
    std::vector<int> vec{1, 2, 3, 4};
    KDAlgorithms::transform(vec, squareItem);
    std::vector<int> expected{1, 4, 9, 16};
    QCOMPARE(vec, expected);
}

void TestAlgorithms::anyOf()
{
    bool res = KDAlgorithms::any_of(intVector, greaterThan(10));
    QCOMPARE(res, false);

    res = KDAlgorithms::any_of(intVector, greaterThan(3));
    QCOMPARE(res, true);

    res = KDAlgorithms::any_of(emptyIntVector, greaterThan(3));
    QCOMPARE(res, false);
}

void TestAlgorithms::allOf()
{
    bool res = KDAlgorithms::all_of(intVector, greaterThan(3));
    QCOMPARE(res, false);

    res = KDAlgorithms::all_of(intVector, greaterThan(0));
    QCOMPARE(res, true);

    res = KDAlgorithms::all_of(emptyIntVector, greaterThan(3));
    QCOMPARE(res, true);
}

void TestAlgorithms::noneOf()
{
    bool res = KDAlgorithms::none_of(intVector, greaterThan(3));
    QCOMPARE(res, false);

    res = KDAlgorithms::none_of(intVector, greaterThan(10));
    QCOMPARE(res, true);

    res = KDAlgorithms::none_of(emptyIntVector, greaterThan(3));
    QCOMPARE(res, true);
}

void TestAlgorithms::reverse()
{
    std::vector<int> vec{1, 2, 3, 4};
    KDAlgorithms::reverse(vec);
    std::vector<int> expected{4, 3, 2, 1};
    QCOMPARE(vec, expected);
}

void TestAlgorithms::reversed()
{
    auto result = KDAlgorithms::reversed(intVector);
    std::vector<int> expected{4, 3, 2, 1};
    QCOMPARE(result, expected);
}

void TestAlgorithms::reversedEnsureMoveOnly()
{
    std::vector<CopyObserver> vec{1, 2, 3, 4, 5};
    CopyObserver::reset();
    auto result = KDAlgorithms::reversed(std::move(vec));
    QCOMPARE(CopyObserver::copies, 0);

    // 3 for each pair I need to swap (1,5) (2,3)
    QCOMPARE(CopyObserver::moves, 6);
}

void TestAlgorithms::sort()
{
    std::vector<int> vec{3, 2, 4, 1};
    KDAlgorithms::sort(vec);
    std::vector<int> expected{1, 2, 3, 4};
    QCOMPARE(vec, expected);
}

void TestAlgorithms::sorted()
{
    const std::vector<int> vec{3, 2, 4, 1};
    auto result = KDAlgorithms::sorted(vec);
    std::vector<int> expected{1, 2, 3, 4};
    QCOMPARE(result, expected);
}

void TestAlgorithms::sortWithCompare()
{
    std::vector<int> vec{3, 2, 4, 1};
    KDAlgorithms::sort(vec, std::greater<int>());
    std::vector<int> expected{4, 3, 2, 1};
    QCOMPARE(vec, expected);
}

void TestAlgorithms::sortedWithCompare()
{
    const std::vector<int> vec{3, 2, 4, 1};
    auto result = KDAlgorithms::sorted(vec, std::greater<int>());
    std::vector<int> expected{4, 3, 2, 1};
    QCOMPARE(result, expected);
}

void TestAlgorithms::sortedEnsureMoveOnly()
{
    std::vector<CopyObserver> vec{3, 2, 4, 1};
    CopyObserver::reset();
    KDAlgorithms::sorted(std::move(vec));
    QCOMPARE(CopyObserver::copies, 0);
}

void TestAlgorithms::is_sorted()
{
    auto byKey = [](const Struct &x, const Struct &y) { return x.key < y.key; };
    QCOMPARE(KDAlgorithms::is_sorted(intVector), true);
    QCOMPARE(KDAlgorithms::is_sorted(std::vector<int>{1, 3, 2, 4}), false);
    QCOMPARE(KDAlgorithms::is_sorted(structVec, byKey), true);

    std::vector<Struct> unsorted{{2, 3}, {1, 4}, {3, 2}, {4, 1}};
    QCOMPARE(KDAlgorithms::is_sorted(unsorted, byKey), false);
}

void TestAlgorithms::lvalue()
{
    const auto isOddLValue = [](int i) { return i % 2 == 1; };
    auto result = KDAlgorithms::filtered(intVector, isOddLValue);
    std::vector<int> expected{1, 3};
    QCOMPARE(result, expected);
}

void TestAlgorithms::contains()
{
    auto result = KDAlgorithms::contains(intVector, 3);
    QCOMPARE(result, true);

    // Test an lvalue
    int value = 3;
    result = KDAlgorithms::contains(intVector, value);
    QCOMPARE(result, true);

    result = KDAlgorithms::contains(intVector, -1);
    QCOMPARE(result, false);

    result = KDAlgorithms::contains(emptyIntVector, 3);
    QCOMPARE(result, false);

    result = KDAlgorithms::contains({1, 2, 3, 4}, 3);
    QCOMPARE(result, true);

    result = KDAlgorithms::value_in(3, {1, 2, 3, 4});
    QCOMPARE(result, true);
}

void TestAlgorithms::count()
{
    std::vector<int> vec{1, 2, 1, 3, 2, 1, 5};
    auto result = KDAlgorithms::count(vec, 1);
    QCOMPARE(result, 3);
}

void TestAlgorithms::count_if()
{
    std::vector<int> vec{1, 2, 1, 3, 2, 1, 5};
    auto result = KDAlgorithms::count_if(vec, [](int i) { return i > 2; });
    QCOMPARE(result, 2);
}

void TestAlgorithms::max()
{
#if __cplusplus >= 201703L
    std::vector<int> unsortedIts{4, 1, 3, 2};
    auto result = KDAlgorithms::max_element(unsortedIts, std::less<int>());
    QCOMPARE(result.value(), 4);

    result = KDAlgorithms::max_element(unsortedIts, std::greater<int>());
    QCOMPARE(result.value(), 1);

    result = KDAlgorithms::max_element(emptyIntVector, std::less<int>());
    QVERIFY(!result.has_value());

    result = KDAlgorithms::max_element(unsortedIts);
    QCOMPARE(result.value(), 4);
#endif
}

void TestAlgorithms::min()
{
#if __cplusplus >= 201703L
    auto result = KDAlgorithms::min_element(intVector, std::less<int>());
    QCOMPARE(result.value(), 1);

    result = KDAlgorithms::min_element(intVector, std::greater<int>());
    QCOMPARE(result.value(), 4);

    result = KDAlgorithms::min_element(emptyIntVector, std::less<int>());
    QVERIFY(!result.has_value());

    result = KDAlgorithms::min_element(intVector);
    QCOMPARE(result.value(), 1);
#endif
}

void TestAlgorithms::maxValueLessThan()
{
#if __cplusplus >= 201703L
    auto result = KDAlgorithms::max_value_less_than(intVector, 4);
    QCOMPARE(result.value(), 3);

    result = KDAlgorithms::max_value_less_than(intVector, 100);
    QCOMPARE(result.value(), 4);

    result = KDAlgorithms::max_value_less_than(intVector, 1);
    QVERIFY(!result.has_value());

    result = KDAlgorithms::max_value_less_than(emptyIntVector, 10);
    QVERIFY(!result.has_value());
#endif
}

void TestAlgorithms::maxValueLessThanCustomComparisor()
{
#if __cplusplus >= 201703L
    auto compare = [](const Struct &v1, const Struct &v2) { return v1.key < v2.key; };

    auto result = KDAlgorithms::max_value_less_than(structVec, Struct{4, 4}, compare);
    Struct expected{3, 2};
    QCOMPARE(result.value(), expected);
#endif
}

void TestAlgorithms::minValueGreaterThan()
{
#if __cplusplus >= 201703L
    auto result = KDAlgorithms::min_value_greater_than(intVector, 0);
    QCOMPARE(result.value(), 1);

    result = KDAlgorithms::min_value_greater_than(intVector, 1);
    QCOMPARE(result.value(), 2);

    result = KDAlgorithms::min_value_greater_than(intVector, 4);
    QVERIFY(!result.has_value());

    result = KDAlgorithms::min_value_greater_than(emptyIntVector, 10);
    QVERIFY(!result.has_value());
#endif
}

void TestAlgorithms::isPermutation()
{
    std::vector<int> permutedVector{4, 1, 3, 2};
    QVERIFY(KDAlgorithms::is_permutation(intVector, permutedVector));

    std::vector<int> nonPermutedVector{1, 3, 2};
    QVERIFY(!KDAlgorithms::is_permutation(intVector, nonPermutedVector));

    auto onKey = [](Struct x, Struct y) { return x.key == y.key; };
    auto onValue = [](Struct x, Struct y) { return x.value == y.value; };

    std::vector<Struct> v1{{1, 2}, {2, 3}, {3, 4}};
    std::vector<Struct> v2{{2, 4}, {3, 4}, {1, 2}};

    QVERIFY(KDAlgorithms::is_permutation(v1, v2, onKey));
    QVERIFY(!KDAlgorithms::is_permutation(v1, v2, onValue));

    std::vector<double> permutedDoubleVector{4.0, 1.0, 3.0, 2.0};
    QVERIFY(KDAlgorithms::is_permutation(intVector, permutedDoubleVector));
}

void TestAlgorithms::accumulate()
{
    // Simple int function
    auto sumDoubles = [](int x, int y) { return x + y * y; };
    int result = KDAlgorithms::accumulate(intVector, sumDoubles);
    QCOMPARE(result, 30);

    // simple string function
    auto slashBetween = [](const QString &x, const QString &y) {
        if (x.isEmpty())
            return y;
        else
            return x + "/" + y;
    };

    QStringList list{"abc", "def", "hij"};
    QString stringResult = KDAlgorithms::accumulate(list, slashBetween);
    QCOMPARE(stringResult, "abc/def/hij");
}

void TestAlgorithms::accumulateWithInitialValue()
{
    auto factorial = [](int subTotal, int value) { return subTotal * value; };
    auto result = KDAlgorithms::accumulate(intVector, factorial, 1);
    QCOMPARE(result, 24);
}

void TestAlgorithms::accumulateDifferentReturnType()
{
    auto toCommaSeperatedString = [](const QString &subTotal, int i) {
        return subTotal + "," + QString::number(i);
    };
    auto stringResult = KDAlgorithms::accumulate(intVector, toCommaSeperatedString);
    QCOMPARE(stringResult, ",1,2,3,4");

    stringResult = KDAlgorithms::accumulate(intVector, toCommaSeperatedString, QString("0"));
    QCOMPARE(stringResult, "0,1,2,3,4");
}

void TestAlgorithms::accumulateWithAuto()
{
    auto toCommaSeperatedString = [](auto subTotal, int i) {
        return subTotal + "," + QString::number(i);
    };
    auto stringResult = KDAlgorithms::accumulate(intVector, toCommaSeperatedString, QString("0"));
    QCOMPARE(stringResult, "0,1,2,3,4");
}

void TestAlgorithms::get_first_match()
{
#if __cplusplus >= 201703L
    auto wihtKey = [](int key) { return [key](const Struct &s) { return s.key == key; }; };

    auto value = KDAlgorithms::get_first_match(structVec, wihtKey(2));
    Struct expected{2, 3};
    QCOMPARE(value.value(), expected);

    value = KDAlgorithms::get_first_match(structVec, wihtKey(-1));
    QVERIFY(!value.has_value());
#endif
}

void TestAlgorithms::get_first_match_or_default()
{
    auto withKey = [](int key) { return [key](const Struct &s) { return s.key == key; }; };

    auto value = KDAlgorithms::get_first_match_or_default(structVec, withKey(2));
    Struct expected{2, 3};
    QCOMPARE(value, expected);

    value = KDAlgorithms::get_first_match_or_default(structVec, withKey(-1));
    QCOMPARE(value, Struct{});

    Struct defaultValue{42, -42};
    value = KDAlgorithms::get_first_match_or_default(structVec, withKey(-1), defaultValue);
    QCOMPARE(value, defaultValue);
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
        KDAlgorithms::remove_duplicates(vec, KDAlgorithms::DoNotSort);
        QCOMPARE(vec, expected);
    }

    // Do not sort, so like std::unique
    {
        std::vector<int> vec{3, 1, 2, 2, 1};
        KDAlgorithms::remove_duplicates(vec, KDAlgorithms::DoNotSort);
        std::vector<int> expected{3, 1, 2, 1};
        QCOMPARE(vec, expected);
    }

    // Sort first
    {
        std::vector<int> vec{3, 1, 2, 2, 1};
        KDAlgorithms::remove_duplicates(vec, KDAlgorithms::DoSort);
        std::vector<int> expected{1, 2, 3};
        QCOMPARE(vec, expected);
    }
}

void TestAlgorithms::has_duplicates()
{
    QFETCH(std::vector<int>, vec);
    QFETCH(bool, expected);
    QFETCH(bool, sort);
    QCOMPARE(
        KDAlgorithms::has_duplicates(vec, sort ? KDAlgorithms::DoSort : KDAlgorithms::DoNotSort),
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

void TestAlgorithms::remove()
{
    // several duplicates to remove
    {
        std::vector<int> vec{1, 2, 1, 3};
        KDAlgorithms::remove(vec, 1);
        std::vector<int> expected{2, 3};
        QCOMPARE(vec, expected);
    }

    // Nothing to remove
    {
        std::vector<int> vec{1, 2, 1, 3};
        KDAlgorithms::remove(vec, 42);
        std::vector<int> expected{1, 2, 1, 3};
        QCOMPARE(vec, expected);
    }
}

void TestAlgorithms::remove_if()
{
    auto withKey = [](int key) { return [key](const Struct &s) { return s.key == key; }; };
    // several duplicates to remove
    {
        std::vector<Struct> vec{{2, 3}, {1, 4}, {2, 2}, {4, 1}};
        KDAlgorithms::remove_if(vec, withKey(2));
        std::vector<Struct> expected{{1, 4}, {4, 1}};
        QCOMPARE(vec, expected);
    }

    // Nothing to remove
    {
        std::vector<Struct> vec{{2, 3}, {1, 4}, {2, 2}, {4, 1}};
        auto expected = vec;
        KDAlgorithms::remove_if(vec, withKey(42));
        QCOMPARE(vec, expected);
    }
}

QTEST_MAIN(TestAlgorithms)

#include "tst_KDAlgorithms.moc"
