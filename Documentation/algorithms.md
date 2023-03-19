Algorithms
==========

Modifying algorithms

- <a href="#copy">copy</a>
- <a href="#copied">copied</a>
- <a href="#filter">filter</a>
- <a href="#transform">transform / transformed</a>
- <a href="#filtered_transformed">filtered_transformed</a>
- <a href="#reverse">reverse</a>
- <a href="#sort">sort</a>
- <a href="#remove_duplicates">remove_duplicates</a>
- <a href="#erase">erase / erase_if</a>

Queries

- <a href="#all_any_none">all_of / any_of / none_of</a>
- <a href="#contains">contains</a>
- <a href="#value_in">value_in</a>
- <a href="#find_if">find_if / mutable_find_if</a>
- <a href="#count">count</a>
- <a href="#min_max_value">min_value / max_value</a> (C++17)
- <a href="#min_max_with_pivot">min_value_greater_than / max_value_less_than / max_value_less_than_unordered / min_value_greater_than_unordered</a> (C++17)
- <a href="#is_permutation">is_permutation</a>
- <a href="#get_match">get_match (C++17) / get_match_or_default</a>
- <a href="#index_of_match">index_of_match</a>
- <a href="#has_duplicates">has_duplicates</a>
- <a href="#is_sorted">is_sorted</a>

Other

- <a href="#accumulate">accumulate</a>
- <a href="#accumulate_if">accumulate_if</a>
- <a href="#iota">iota</a>
- <a href="#generate_n">generate_n</a>
- <a href="#generate_until">generate_until</a>
- <a href="#for_each">for_each</a>
- <a href="#partitioned">partitioned</a>
- <a href="#zip">zip</a>



<a name="copy">copy</a>
-----------------------
This algorithm appends the content of one collection to another.

```
std::vector<int> src = ...;
std::list<int> dest = ...;
kdalgorithms::copy(src, dest);
```

To only copy some elements, see the algorithms <a href="#filter">filter and filtered</a>

See [std::copy](https://en.cppreference.com/w/cpp/algorithm/copy) for the algorithm from the standard.

<a name="copied">copied</a>
---------------------------
This algorithm returns a copy of the input elements, with the container changed in the process, simlimar to <a href="#transformed">transformed</a>.
If you do not need to change the container, then simply use the contaioners copy constrcutor instead of this algorithm.

```
std::vector<int> vec{1, 2, 3, 4, 1, 3};
auto result = kdalgorithms::copied<std::unordered_set>(vec);
// result is now an unordered_set with the items 1,2,3,4 in.
```

The items themselves may also be implicit converted in the process:

```
std::vector<int> vec{1, 2, 3, 4, 1, 3};
auto result = kdalgorithms::copied<std::unordered_set<double>>(vec);
// result is now an unordered_set of doubles with the items 1,2,3,4 in.
```

<a name="filter">filter / filtered</a>
--------------------------------------
The simplest way to use filtered is the code below, which takes a vector of ints 
and returns a new vector, with the elements matching the provided predicate.

>
```
auto is_odd = [] (int i) { return i%2 == 1; };
std::vector<int> ints{1,2,3,4,5};
auto odds = kdalgorithms::filtered(ints, is_odd);
```

The algorithm may change the container on the fly:

>
```
auto is_odd = [] (int i) { return i%2 == 1; };
std::list<int> ints{1,2,3,4,5};
auto odds = kdalgorithms::filtered<std::vector>(ints, is_odd);
// ods = std::vector<int>{1,3,5}
```


There is also a variant, which does the filtering inline (that is the result will be in the provided container).

>
```
auto is_odd = [] (int i) { return i%2 == 1; };
std::vector<int> ints{1,2,3,4,5};
kdalgorithms::filter(ints, is_odd);
// ints = {1, 3, 5}
```

See [std::copy_if](https://en.cppreference.com/w/cpp/algorithm/copy) for the algorithm from the standard.


<a name="transform">transform / transformed</a>
-----------------------------------------------
This set of algorithms takes a collection and return a collection of the same size
with all the elements transformed by the provided method.

The first example has the same container as input and output:

```
std::vector<int> ints{1,2,3};
auto square = [] (int i) { return i*i; }
auto result = kdalgorithms::transformed(ints, square);
```

The type of container and items may also change during the transformation

```
std::vector<int> ints{1,2,3};
auto toString = [] (int i) { return QString::number(i); }
QVector<QString> result = kdalgorithms::transformed<QVector>(ints, toString);
```

Finally there is a version which does an inline transform:

```
std::vector<int> ints{1,2,3};
auto square = [] (int i) { return i*i; }
kdalgorithms::transform(ints, square);
// ints = {1,4,9}
```

transformed on maps
-------------------
The transform functions can unfortunately not automatically deduce the
type of the result container - even when it is just the same type as the input container,
therefore two additional functions are provided:

```
std::map<int, std::string> map{{1, "abc"}, {2, "def"}, {3, "hij"}, {4, "klm"}};
auto doubleKeys = [](const auto& item) {
    return std::make_pair(item.first * 2, item.second);
};
auto result = kdalgorithms::transformed_to_same_container(map, doubleKeys);
// result = {2, "abc"}, {4, "def"}, {6, "hij"}, {8, "klm"}
```


```
std::map<int, std::string> map{{1, "abc"}, {2, "def"}, {3, "hij"}, {4, "klm"}};
auto result = kdalgorithms::transformed_with_new_return_type<std::map<std::string, int>>(
    map, [](auto item) { return std::make_pair(item.second, item.first); });
// result = {{"abc", 1}, {"def", 2}, {"hij", 3}, {"klm", 4}};
```

See [std::transform](https://en.cppreference.com/w/cpp/algorithm/transform) for the algorithm from the standard.

<a name="filtered_transformed">filtered_transformed</a>
-------------------------------------------
<b>filtered_transformed</b> is a combination of <a href="#transform">transformed</a> and <a href="#filter">filtered</a>. 
The function takes a container and throws away any item that doesn't match the filter, and applies the transform on the remaining.

```
std::vector<int> intVector{1, 2, 3, 4};
auto squareItem = [] (int i) { return i*i; };
auto isOdd = [] (int i) { return i%2 == 1; };

auto result = kdalgorithms::filtered_transformed(intVector, squareItem, isOdd);
// result = {1, 9}
```

<b>filtered_transformed</b> supports the same features as regular <a href="#transform">transformed</a> does, 
including using member functions, and using a different container for the result:

<b>Member functions:</b>

```
auto result = kdalgorithms::filtered_transformed(structVec, &Struct::sumPairs,
                                           &Struct::isKeyGreaterThanValue);
```

<b>Changing container</b>

```
auto result = kdalgorithms::filtered_transformed<std::deque>(intVector, squareItem, isOdd);
```


<a name="reverse">reverse / reversed</a>
-----------------------------------------

```
std::vector<int> ints{1,2,3};
auto result = kdalgorithms::reversed(ints);
// result = {3,2,1}
```

And there is also an inline version:

```
std::vector<int> ints{1,2,3};
kdalgorithms::reverse(ints);
// ints = {3,2,1}
```

See [std::reverse](https://en.cppreference.com/w/cpp/algorithm/reverse)  for the algorithm from the standard.


<a name="sort">sort / sorted</a>
--------------------------------
This algorithm comes in two version sort (inline) and sorted (returns the result). Besides the collection
it may also take a comparison function.

```
std::vector<int> ints{3,1,2};
auto result = kdalgorithms::sorted(ints);
// result = 1,2,3
```

Inline:

```
std::vector<int> ints{3,1,2};
kdalgorithms::sort(ints);
// ints = 1,2,3
```

Inline and with comparison function

```
std::vector<int> ints{3,1,2};
kdalgorithms::sort(ints, std::greater<int>());
// ints = 3,2,1
```

See [std::sort](https://en.cppreference.com/w/cpp/algorithm/sort) for the algorithm from the standard.

<a name="is_sorted">is_sorted</a>
---------------------------------
Tells whether a sequence is sorted. An optional second argument is used for comparison.

```
auto result = kdalgorithms::is_sorted(std::vector<int>{1, 3, 2, 4});
// result = true
```

```
struct S
{
    int x;
    int y;
};
std::vector<Struct> unsorted{{2, 3}, {1, 4}, {3, 2}, {4, 1}};
auto result = kdalgorithms::is_sorted(unsorted, [](Struct x, Struct y) { return x.key < y.key; });
// result = false
```

See [std::is_sorted](https://en.cppreference.com/w/cpp/algorithm/is_sorted) for the algorithm from the standard.


<a name="all_any_none">all_of / any_of / none_of</a>
---------------------------------------------------
```
std::vector<int> ints{1,2,3};
auto is_odd = [] (int i) { return i%2 == 1; };
bool answer = kdalgorithms::any_of(ints, is_odd);
```

See [std::all_of, std::any_of, std:none_of](https://en.cppreference.com/w/cpp/algorithm/all_any_none_of) for the algorithms from the standard.

<a name="contains">contains</a>
--------------------------------
The algorithm *contains* is searching for an item, but in contrast to *any_of* it does so by simply specifying the item.

```
std::vector<int> ints{1,2,3};
bool answer = kdalgorithms::contains(ints, 4);
```

The function is also overloaded with an initializer list, which makes it possible to write code like this:

```
enum class Column {ColumnA, ColumnB, ColumnC, ColumnD, ColumnE};
Column column = ...;

if (kdalgorithms::contains({ColumnA, ColumnC, ColumnE}, column))
{ 
   ....
}
```

However, <a href="value_in">kdalgorithms::value_in</a> has a more pleasing syntax for that usecase.

see [std::find](https://en.cppreference.com/w/cpp/algorithm/find) for the algorithm from the standard.

<a name="value_in">value_in</a>
--------------------------------

This is similar to *contains* but with the arguments switched around. This makes it possible to write code
that feels a lot like *value in range* from languages like Python:

```
if (kdalgorithms::value_in(column, {ColumnA, ColumnC, ColumnE}))
{ 
   ....
}
```

<a name="find_if">find_if / mutable_find_if</a>
----------------------------------------------
KDAlgorithms' version of find_if, takes a complete collection rather than two iterators. 
This, however, poses the problem of which iterator to use when testing for <i>no result</i>. We've
solved that by returns a proxy object with a simple boolean test method on.

```
    std::vector<int> vec{1, 2, 3, 4, 5};
    auto result = kdalgorithms::find_if(vec, [](int i) { return i > 2; });
    if (result)
       std::cout << *result << std::endl;
    else
       std::cout << "ahh nothing, right\n";
    
    // prints: 3
```

If you want to modify the result of find_if, then you explicitly need to ask for a mutable version:

```
    std::vector<int> vec{1, 2, 3, 4, 5};
    auto result = kdalgorithms::mutable_find_if(vec, [](int i) { return i > 2; });
    assert(result.has_result());
    *result = 42;
    for (auto i : vec)
        std::cout << i << ",";
    std::cout << std::endl;

    // prints: 1,2,42,4,5,
```

It is possible to get to the underlying iterator from the result. You, however, need to ensure to use the **begin** and **end** iterators found in the result object.

```
    struct Person
    {
        int age;
        bool isDeveloper;
    };
    
    std::vector<Person> vec{{20, true}, {21, false}, {30, true}, {35, false}, {35, true}};
    auto result =
        kdalgorithms::mutable_find_if(vec, [](const auto &person) { return person.age > 30; });
    
    // Observe we use result.begin and result.iterator
    std::partition(result.begin, result.iterator,
                   [](const auto &person) { return person.isDeveloper; });
    
    std::for_each(result.begin, result.iterator(), [](const auto &person) {
        std::cout << std::boolalpha << "(" << person.age << "," << person.isDeveloper << ") ";
    });
    std::cout << std::endl;
    
    // prints: (20,true) (30,true) (21,false) 
```

see [std::find_if](https://en.cppreference.com/w/cpp/algorithm/find_if) for the algorithm from the standard.

<a name="count">count / count_if</a>
-------------------------------------
```
std::vector<int> vec{1, 2, 1, 3, 2, 1, 5};
auto result = kdalgorithms::count(vec, 1);
```

```
std::vector<int> vec{1, 2, 1, 3, 2, 1, 5};
auto result = kdalgorithms::count_if(vec, [](int i) { return i > 2; });
```

See [std::count](https://en.cppreference.com/w/cpp/algorithm/count) and [std::count_if](https://en.cppreference.com/w/cpp/algorithm/count_if)  for the algorithm from the standard.

<a name="min_max_value">min_value / max_value</a> (C++17)
--------------------------------------------------------------
[std::min](https://en.cppreference.com/w/cpp/algorithm/min) and [std::max](https://en.cppreference.com/w/cpp/algorithm/max)
can compare two values, and in addition to that it can find the smallest/largest item in a initializer_list. On the other hand,
[std::min_element](https://en.cppreference.com/w/cpp/algorithm/min_element) and [std::max_element](https://en.cppreference.com/w/cpp/algorithm/max_element)
can search for items in generic containers, but returns an iterator

Our version works on general containers and returns a [std::optional](https://en.cppreference.com/w/cpp/utility/optional) with the item.

```
std::vector<int> ints{4,1,3,2};
std::optional<int> item = kdalgorithms::max_value(ints);
// item.value() = 4

std::vector<int> ints{};
std::optional<int> item = kdalgorithms::max_value(ints);
// item.has_value() = false
```

It is also possible to provide a comparison function:

```
std::vector<int> ints{4,1,3,2};
auto result = kdalgorithms::min_value(int_vector, std::greater<int>());
// result.value() == 4
```

OK, I admit, that's a silly example.

See [std::min_element](https://en.cppreference.com/w/cpp/algorithm/min_element) and [std::max_element](https://en.cppreference.com/w/cpp/algorithm/max_element) for the algorithm from the standard.


<a name="min_max_with_pivot">min_value_greater_than | max_value_less_than | max_value_less_than_unordered | min_value_greater_than_unordered</a> (C++17)
------------------------------------------------------------------------------------------------
When searching for the minimum value greater than a specific item or when searching for the maximum value
less than a specific item, the algorithms lower_bound and upper_bound comes into play.

They can, however, be hard to wrap your head around, which is why this library provides the two
most common use cases above.
Both functions returns a [std::optional](https://en.cppreference.com/w/cpp/utility/optional).

```
std::vector<int> ints{1,2,3,4};
std::vector<int> unsortedInts{2,4,1,3};
std::optional<int> result = kdalgorithms::max_value_less_than(ints, 4);
// result.value() = 3
std::optional<int> resultForUnsortedContainer = kdalgorithms::max_value_less_than_unordered(unsortedInts, 4);
// max_value_less_than_unordered works on the unsorted container and it has O(n) complexity, so use it as per your use case :)
// resultForUnsorted.value() = 3;

result = kdalgorithms::max_value_less_than(ints, -1);
// result.has_value() = false
```

The algorithms may also take a comparison function:

```
struct S
{
    int x;
    int y;
};

std::vector<S> vec{{1, 4}, {2, 3}, {3, 2}, {4, 1}};
auto compare = [](const S &v1, const S &v2) { return v1.x < v2.x; };

auto result = kdalgorithms::max_value_less_than(vec, S{4, 4}, compare);
// result == S{3,2}
```

*Observe, that due to the usage of std::optional, the above functions are only available when
compiling with C++17 or above.*

<a name="is_permutation">is_permutation</a>
------------------------------------------
```
std::vector<int> x{1,2,3,4}
std::vector<int> y{4, 1, 3, 2};
bool b = kdalgorithms::is_permutation(x,y));
// b = true
```

See [std::is_permutation](https://en.cppreference.com/w/cpp/algorithm/is_permutation) for the algorithm from the standard.

<a name="accumulate">accumulate</a>
-----------------------------------
The simplest form of accumulate takes a collection, and applies *plus* on the items:

```
std::vector<int> ints{1,2,3,4};
auto result = kdalgorithms::accumulate(ints);
// result = 10
```

A much more common usage is to specify a mapping function:

```    
std::vector<int> ints{1,2,3,4};
auto sum_doubles = [](int subResult, int x) { return subResult + x * x; };
int result = kdalgorithms::accumulate(ints, sum_doubles);
// result = 1*1 + 2*2 + 3*3 + 4*4
```

Observe that the initial value doesn't need to be provided in the above.
If you need to specify it, you can do so as an optional third parameter:

```
std::vector<int> ints{1,2,3,4};
auto factorial = [](int sub_total, int value) { return sub_total * value; };
auto result = kdalgorithms::accumulate(int_vector, factorial, 1);
// result = 1*1*2*3*4
```

The initial value is the default value for the return type of the mapping function.
This value must, however, be deducable without having to instantiate the arguments to the lambda expression.
That, for example, isn't the case for this function:

```
auto to_comma_seperated_string = [](auto sub_total, int i) {
    return sub_total + "," + QString::number(i);
};
```

Here, the type of the function is the type of calling opertor+ on whatever the type of sub_total is.
In such cases, where you are using auto in the lambda expression, you have to provide an initial value:

```
std::vector<int> ints{1,2,3,4};
auto to_comma_seperated_string = [](auto sub_total, int i) {
    return sub_total + "," + QString::number(i);
};
auto result = kdalgorithms::accumulate(int_vector, to_comma_seperated_string, QString("0"));
// result = "0,1,2,3,4"
```

### Accumulate and maps

It is also possible to use accumulate with maps as this example shows:

```
std::map<int, int> map{{1, 10}, {2, 20}, {3, 30}, {4, 40}};
int result =
    kdalgorithms::accumulate(map, [](int res, const std::pair<const int, int> &pair) {
        return res + pair.first * pair.second;
    });
// result = 1*10 + 2*20 + 3*30 + 4*40
```

Notice, however, that you in the above need to explicitly specify the type of the second parameter to the lambda expression, 
which admittedly is a bit cumbersome, so in those cases, it is easier to simply specify the initial value:

```
std::map<int, int> map{{1, 10}, {2, 20}, {3, 30}, {4, 40}};
int result = kdalgorithms::accumulate(
    map, [](int res, const auto &pair) { return res + pair.first * pair.second; }, 0);
```

See [std::accumulate](https://en.cppreference.com/w/cpp/algorithm/accumulate) for the algorithm from the standard.

### Accumulating with a builder object

Given that accumulate accepts pointer to member functions as argument, a neat way to build the result is using a builder object:

```
struct ResultBuilder
{
    ResultBuilder &append(const std::string &other)
    {
        result += "/" + other;
        return *this;
    }
    std::string result;
};

{
    std::vector<std::string> list{"abc", "def", "hij"};
    auto result = kdalgorithms::accumulate(list, &ResultBuilder::append, ResultBuilder());
    result.result = "/abc/def/hij";
}
```


<a name="accumulate_if">accumulate_if</a>
-----------------------------------------
This is similar to <a href="#accumulate">accumulate</a>, with the addition that it only accumulate if the provided predicate evaluates to true.

```
std::vector<int> ints{1,2,3,4};
auto sumDoubles = [](int x, int y) { return x + y * y; };
auto greaterThan = [](int value) { return [value](int test) { return test > value; }; };
int result = kdalgorithms::accumulate_if(ints, sumDoubles, greatherThan(2));
// result = 25
```

<a name="get_match">get_match (C++17) / get_match_or_default</a>
-------------------------------------------------
This function exist in two variants, they differ on what they do in case the item searched for
doesn't exist in the collection. **get_match** returns an optional (and thus requires C++17),
while **get_match_or_default** returns a default constructed item.

```
struct Struct
{
    int key;
    int value;
};
std::vector<Struct> vec { ... };

std::optional<Struct> result1
   = kdalgorithms::get_match(vec, [] (Struct s) { return s.key == 42; };

Struct result2
   = kdalgorithms::get_match_or_default(vec, [] (Struct s) { return s.key == 42; };
```

<a name="index_of_match">index_of_match</a>
-------------------------------------------
This function returns the index of the first match in the given container

```
std::vector<Struct> vec{{1, 2}, {2, 1}, {3, 3}, {4, 4}};
result = kdalgorithms::index_of_match(vec, &Struct::hasEqualKeyValuePair);
// result = 2
```

<a name="remove_duplicates">remove_duplicates</a>
-------------------------------------
**remove_duplicates** takes a collection and removes duplicates.
It does so inline (that is, it doesn't return a new collection).

The second parameter tells whether the collection should be sorted first.

```
std::vector vec{3, 1, 2, 2, 1};
kdalgorithms::remove_duplicates(vec, kdalgorithms::do_not_sort);
// vec = {3, 1, 2, 1}
```

```
std::vector vec{3, 1, 2, 2, 1};
kdalgorithms::remove_duplicates(vec, kdalgorithms::do_sort);
// vec = {1, 2, 3}
```

See [std::unique](https://en.cppreference.com/w/cpp/algorithm/unique) for the algorithm from the standard.

<a name="has_duplicates">has_duplicates</a>
-------------------------------------------
**has_duplicates** takes a collection (which doesn't need to be ordered), and tells whether there are any duplicates in there.

```
std::vector vec{3, 1, 2, 1};
auto result = kdalgorithms::has_duplicates(vec, kdalgorithms::do_sort);
// result = true
```

<a name="erase">erase / erase_if</a>
---------------------------------------
**erase** removes all instances of a given value, while **erase_if** remove all instances matching a predicate.

```
std::vector<int> vec{1, 2, 1, 3};
kdalgorithms::erase(vec, 1);
// vec = {2,3}
```

```
struct Struct
{
    int key;
    int value;
};
auto with_key = [](int key) { return [key](const Struct &s) { return s.key == key; }; };

std::vector<Struct> vec{{2, 3}, {1, 4}, {2, 2}, {4, 1}};
kdalgorithms::erase_if(vec, with_key(2));
// vec = {{1,4}, {4,1}}
```

- See [std::remove / std::remove_if](https://en.cppreference.com/w/cpp/algorithm/remove) for the algorithm from the standard,
- See [std::ranges::remove / std::ranges::remove_if](https://en.cppreference.com/w/cpp/algorithm/ranges/remove) for the C++20 ranges implementation
- See [std::erase / std::erase_if](https://en.cppreference.com/w/cpp/container/vector/erase2) for the C++20 implementation. 

<a name="iota">iota</a>
-----------------------
Generate a container with a number of elements  in incremental order

```
std::vector<int> result = kdalgorithms::iota(10, 5);
// result = {10, 11, 12, 13, 14};
```

*iota* might also be used with just a single argument, in which case it generates a container of that many elements starting at 0:

```
std::vector<int> result = kdalgorithms::iota(5);
// result = {0, 1, 2, 3, 4};
```
If you are familiar with Python, then this one argument version is equivalent to the python expression "`range(5)`"

See [std::iota](https://en.cppreference.com/w/cpp/algorithm/iota) for the algorithm from the standard.

<a name="generate_n">generate_n</a>
------------------------------
This function generates n elements into an existing container by calling a generator function.

```
std::unordered_set<int> result;
kdalgorithms::generate_n(result, 5, [](int index) { return index*index; });
// result = {0, 1, 4, 9, 16};
```

Observe: In contrast to [std::generate_n](https://en.cppreference.com/w/cpp/algorithm/generate_n) our version's generate function <i>can</i> take the index as a parameter.
In other words kdalgorithms::generate_n will call operator(int) if it exists, and otherwise call operator().

So the following version also works, and is closer to the behavior of std::generate_n:

```
std::vector<int> result;
kdalgorithms::generate_n(result, 5, [i = 0]() mutable { return ++i; });
// result = {1, 2, 3, 4, 5};
```

See [std::generate_n](https://en.cppreference.com/w/cpp/algorithm/generate_n) for the algorithm from the standard,

<a name="generate_until">generate_until</a>
-------------------------------
The function produces a container of items generated by a generator.
The generator will continue until it produces an empty <i>element</i>.

####Example with std::optional (requires C++17)

```
auto generator = [x = 0]() mutable -> std::optional<int> {
    if (x < 4) {
        ++x;
        return x*x;
    }
    return {};
};

std::vector<int> result = kdalgorithms::generate(generator);
// result = {1, 4, 9, 16}
```

####Example with a unique_ptr (works with C++14)

```
auto generator = [x = 0]() mutable -> std::unique_ptr<int> {
    if (x < 4) {
        ++x;
        return std::make_unique<int>(x * x);
    }
    return {};
};

auto result = kdalgorithms::generate_until(generator);
// result = {1, 4, 9, 16}
```

Similar to <a href="#transform">transform</a>, the return container can be specified as a template parameter:

```
auto result = kdalgorihms::generate<std::deque>(generator);
```

It is also possible to use your own type as return type from the generator, in which case they must supply these two:

```
int operator*();
operator bool();
```
See the unit tests for an example implementation.

See [std::generate](https://en.cppreference.com/w/cpp/algorithm/generate) for the algorithm from the standard,

<a name="for_each">for_each</a>
-------------------------------
This algorithm invokes a specific function on each element of a container.

```
kdalgorithms::for_each(structVec, &Struct::print);
```

I'm sure there are good and valid usecases for using for_each, but will nevertheless claims that in 98% of the cases there are alternative algorithms which are a better match.

See [std::for_each](https://en.cppreference.com/w/cpp/algorithm/for_each) for the algorithm from the standard,

<a name="partitioned">partitioned</a>
---------------------------------
partitioned takes a container and a predicate, and returns two new containers, the first with the items matching the predicate, the second with the items not matching.

```
std::vector<int> vec{4, 1, 3, 2};
const auto result = kdalgorithms::partition(vec, [](int i) { return i > 2; });
// result.in = {4, 3}, result.out = {1, 2}
```

If your compiler supports C++17, then structural bindings may make the above slightlt more readable.

```
std::vector<int> vec{4, 1, 3, 2};
const auto& [in, out] = kdalgorithms::partition(vec, [](int i) { return i > 2; });
// in = {4, 3}, out = {1, 2}
```

If the container provided is an x-value (expiring value), then the items will be moved over rather than copied over.

Similar to <a href="#transform">transformed</a>, it is possible to change the container type:

```     
std::vector<int> vec{4, 1, 3, 2};
const auto& [in, out] =
    kdalgorithms::partitioned<std::unordered_set>(vec, [](int i) { return i > 2; });
// in = std::unordered_set{4, 3}, out = std::unordered_set{1, 2}
```

See [std::partition](https://en.cppreference.com/w/cpp/algorithm/partition) for the algorithm from the standard.

<a name="zip">zip</a>
---------------------
zip takes a number of containers and return one container with tuples for each of the items in the input containers.

```
std::vector<int> x{1, 2, 3};
std::deque<char> y{'a', 'b', 'c'};
std::list<std::string> x{"hello", "kdalgorithms", "world"};

auto result = kdalgorithms::zip(x, y, z);
// result is a std::vector<std::tuple<int, char, std::string>>
// and the values are {1, 'a', "hello"}, {2, 'b', "kdalgorithms"}, {3, 'c', "world"}
```

As all other algorithms, the items will be moved over if the input container is an xvalue (an expiring value which can be stolen).
Further it is possible to change the result container:

```
auto result = kdalgorithms::zip<std::deque>(v1, v2);
```

See [boost::compine](https://www.boost.org/doc/libs/1_81_0/libs/range/doc/html/range/reference/utilities/combine.html) for similar algorithm in boost, and [std::ranges::views::zip](https://en.cppreference.com/w/cpp/ranges/zip_view) for the C++23 version.
