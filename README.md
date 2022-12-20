Algorithm helpers - kdalgorithms
================================

The purpose of this library is to make it easier to work with algorithms in C++14 and up.

* <a href="Documentation/algorithms.md">Documentation of all the algorithms</a>
* <a href="Documentation/deploying.md">Deploying the algorithms with CMake</a>
* <a href="tests/tst_kdalgorithms.cpp">Be inspired by all the unit tests</a>
* <a href="https://youtu.be/iAEIPk64ZJw?list=PL6CJYn40gN6gf-G-o6syFwGrtq3kItEqI">Youtube video introducing the library</a>

Example - filtered
------------------

With C++11 algorithms you might write:

```
std::vector<int> result;
auto is_odd = [] (int i) {return i % 2 == 1; };
std::copy_if(my_list.cbegin(), my_list.cend(), std::back_inserter(result), is_odd);
```

With this library you can instead write:

```
auto is_odd = [] (int i) {return i % 2 == 1; };
auto result = kdalgorithms::filtered(my_list, is_odd);
```

Observe that it:

1. takes a complete collection rather than an iterator pairs
2. returns the result rather than taking an iterator to where it should go

Example - max_value_less_than
-----------------------------

When searching in a collection for the largest element no larger than a given value, 
a good algorithmic choice is std::upper_bound, or ... is it std::lower_bound, and what exactly does is return?

Here is how to do it with this library:

```
std::optional<int> result = kdalgorithms::max_value_less_than(int_vector, 4);
```

By returning an optional rather than an iterator, you do not need to worry whether you should check it against 
int_vector.begin() or int_vector.cbegin(). It is also more obvious that the value might be non-existent -
after all, that is what std::optional is all about.

Combining Algorithms
--------------------
kdalgorithms offers <i>and</i>, <i>or</i> and <i>not</i> operators, which makes it possible to write code like this:

```
using namespace kdalgorithms::Operators;
std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const auto is_odd = [](int num) { return num % 2 == 1; };
const auto is_greather_than = [](int num) { return [num](int value) { return value > num; }; };
const auto is_dividable_by = [](int num) {
    return [num](int value) { return value % num == 0; };
};

auto result = kdalgorithms::filtered(vec, is_odd || (is_greather_than(5) && !is_dividable_by(3)));
```

Using member functions as predicates
------------------------------------
Throughout the documentation we will used lambda expressions for predicates and comparison methods
given to algorithms. It is, however, also possible to use pointer to member functions:

```
struct S
{
    int x;
    int y;
    bool hasEqualKeyValuePair() const { return x == y; }
};
std::vector<Struct> vec{{2, 3}, {1, 1}, {2, 2}, {4, 1}};
kdalgorithms::erase_if(vec, &Struct::hasEqualKeyValuePair);
```

Be aware though that the method must be const.

Lots of Examples
----------------
In this document you will find lots of examples. More, however, may be found in the 
unit tests.

Qt
--
kdalgorithms supports all of Qt's containers, with the exception that with Qt5 QSet isn't supported.

