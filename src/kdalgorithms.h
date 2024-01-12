/****************************************************************************
**
** This file is part of KDAlgorithms
**
** SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#pragma once

#include "kdalgorithms_bits/filter.h"
#include "kdalgorithms_bits/find_if.h"
#include "kdalgorithms_bits/generate.h"
#include "kdalgorithms_bits/insert_wrapper.h"
#include "kdalgorithms_bits/invoke.h"
#include "kdalgorithms_bits/method_tests.h"
#include "kdalgorithms_bits/operators.h"
#include "kdalgorithms_bits/read_iterator_wrapper.h"
#include "kdalgorithms_bits/reserve_helper.h"
#include "kdalgorithms_bits/return_type_trait.h"
#include "kdalgorithms_bits/shared.h"
#include "kdalgorithms_bits/to_function_object.h"
#include "kdalgorithms_bits/transform.h"
#include "kdalgorithms_bits/zip.h"
#include <algorithm>
#include <map>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

#if __cplusplus >= 201703L
#include <optional>
#endif

namespace kdalgorithms {

// -------------------- copy --------------------
template <typename InputContainer, typename OutputContainer>
#if __cplusplus >= 202002L
    requires ContainerOfType<OutputContainer, ValueType<InputContainer>>
#endif
void copy(InputContainer &&input, OutputContainer &output)
{
    bool foundReserve = detail::reserve(output, input.size() + output.size());
    if (!foundReserve && detail::is_same_object(input, output)) {
        remove_cvref_t<InputContainer> tmp = input;
        copy(tmp, output);
        return;
    }
    auto range = read_iterator_wrapper(std::forward<InputContainer>(input));
    std::copy(range.begin(), range.end(), detail::insert_wrapper(output));
}

// -------------------- copied --------------------
template <typename ResultContainer, typename InputContainer>
#if __cplusplus >= 202002L
    requires ContainerOfType<ResultContainer, ValueType<InputContainer>>
#endif
ResultContainer copied(InputContainer &&input)
{
    static_assert(!std::is_same<ResultContainer, InputContainer>::value,
                  "Use copy constructor instead of kdalgorithms::copied");
    ResultContainer result;
    kdalgorithms::copy(std::forward<InputContainer>(input), result);
    return result;
}

template <template <typename...> class ResultContainer, typename InputContainer>
auto copied(InputContainer &&input)
{
    return copied<ResultContainer<ValueType<InputContainer>>, InputContainer>(
        std::forward<InputContainer>(input));
}

// -------------------- all_of / any_of / none_of --------------------
template <typename Container, typename UnaryPredicate>
bool any_of(const Container &container, UnaryPredicate &&predicate)
{
    auto range = read_iterator_wrapper(container);
    return std::any_of(range.begin(), range.end(),
                       detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
}

template <typename Container, typename UnaryPredicate>
bool all_of(const Container &container, UnaryPredicate &&predicate)
{
    auto range = read_iterator_wrapper(container);
    return std::all_of(range.begin(), range.end(),
                       detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
}

template <typename Container, typename UnaryPredicate>
bool none_of(const Container &container, UnaryPredicate &&predicate)
{
    auto range = read_iterator_wrapper(container);
    return std::none_of(range.begin(), range.end(),
                        detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
}

// -------------------- reverse / reversed --------------------
template <typename Container>
void reverse(Container &container)
{
    std::reverse(std::begin(container), std::end(container));
}

template <typename Container>
Container reversed(Container container)
{
    reverse(container);
    return container;
}

// -------------------- sort / sorted --------------------
template <typename Container, typename Compare = std::less<ValueType<Container>>>
void sort(Container &container, Compare &&compare = {})
{
    std::sort(std::begin(container), std::end(container),
              detail::to_function_object(std::forward<Compare>(compare)));
}

template <typename Container, typename Compare = std::less<ValueType<Container>>>
Container sorted(Container container, Compare &&compare = {})
{
    sort(container, std::forward<Compare>(compare));
    return container;
}

enum sort_direction { ascending, descending };

template <typename Container, typename Member>
void sort_by(Container &container, Member member, sort_direction direction = ascending)
{
    sort(container, [member, direction](const auto &x, const auto &y) {
        if (direction == ascending)
            return detail::invoke(member, x) < detail::invoke(member, y);
        else
            return detail::invoke(member, x) > detail::invoke(member, y);
    });
}

template <typename Container, typename Member>
auto sorted_by(Container container, Member member, sort_direction direction = ascending)
{
    sort_by(container, member, direction);
    return container;
}

// -------------------- is_sorted --------------------
template <typename Container, typename Compare = std::less<ValueType<Container>>>
bool is_sorted(const Container &container, Compare &&compare = {})
{
    return std::is_sorted(std::cbegin(container), std::cend(container),
                          detail::to_function_object(std::forward<Compare>(compare)));
}

// -------------------- contains / value_in --------------------
template <typename Container, typename Value>
#if __cplusplus >= 202002L
    requires std::equality_comparable_with<ValueType<Container>, Value>
#endif
bool contains(const Container &container, Value &&value)
{
    return std::find(std::cbegin(container), std::cend(container), std::forward<Value>(value))
        != std::cend(container);
}

template <typename Value, typename ContainerValue>
#if __cplusplus >= 202002L
    requires std::equality_comparable_with<ContainerValue, Value>
#endif
bool contains(std::initializer_list<ContainerValue> container, Value &&value)
{
    const auto it =
        std::find(std::begin(container), std::end(container), std::forward<Value>(value));
    return it != std::cend(container);
}

template <typename Value, typename ContainerValue>
#if __cplusplus >= 202002L
    requires std::equality_comparable_with<ContainerValue, Value>
#endif
bool value_in(Value &&value, std::initializer_list<ContainerValue> container)
{
    return contains(container, std::forward<Value>(value));
}

// -------------------- count / count_if --------------------
template <typename Container, typename Value>
#if __cplusplus >= 202002L
    requires ContainerOfType<Container, Value>
#endif
int count(const Container &container, Value &&value)
{
    return std::count(std::cbegin(container), std::cend(container), std::forward<Value>(value));
}

template <typename Container, typename UnaryPredicate>
#if __cplusplus >= 202002L
    requires UnaryPredicateOnContainerValues<UnaryPredicate, Container>
#endif
int count_if(const Container &container, UnaryPredicate &&predicate)
{
    auto range = read_iterator_wrapper(container);
    return std::count_if(range.begin(), range.end(),
                         detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
}

// -------------------- min_value / max_value --------------------
#if __cplusplus >= 201703L
template <typename Container, typename Compare = std::less<ValueType<Container>>>
#if __cplusplus >= 202002L
    requires BinaryPredicateOnContainerValues<Compare, Container>
#endif
std::optional<ValueType<Container>> max_value(const Container &container, Compare &&compare = {})
{
    if (std::cbegin(container) == std::cend(container))
        return {};

    return *std::max_element(std::cbegin(container), std::cend(container),
                             detail::to_function_object(std::forward<Compare>(compare)));
}

template <typename Container, typename Compare = std::less<ValueType<Container>>>
[[deprecated("use max_value instead")]] std::optional<ValueType<Container>>
max_element(const Container &container, Compare &&compare = {})
{
    return max_value(container, std::forward<Compare>(compare));
}

template <typename Container, typename Compare = std::less<ValueType<Container>>>
#if __cplusplus >= 202002L
    requires BinaryPredicateOnContainerValues<Compare, Container>
#endif
std::optional<ValueType<Container>> min_value(const Container &container, Compare &&compare = {})
{
    if (std::cbegin(container) == std::cend(container))
        return {};
    return *std::min_element(std::cbegin(container), std::cend(container),
                             detail::to_function_object(std::forward<Compare>(compare)));
}

template <typename Container, typename Compare = std::less<ValueType<Container>>>
[[deprecated("use max_value instead")]] std::optional<ValueType<Container>>
min_element(const Container &container, Compare &&compare = {})
{
    return min_value(container, std::forward<Compare>(compare));
}
#endif

// -------------------- max_value_less_than / min_value_greater_than --------------------
#if __cplusplus >= 201703L
template <typename Container, typename Item, typename Compare = std::less<ValueType<Container>>>
#if __cplusplus >= 202002L
    requires BinaryPredicateOnContainerValues<Compare, Container>
    && ContainerOfType<Container, Item>
#endif
std::optional<Item> max_value_less_than(const Container &container, Item &&needle,
                                        Compare &&compare = {})
{
    auto it =
        std::lower_bound(std::cbegin(container), std::cend(container), std::forward<Item>(needle),
                         detail::to_function_object(std::forward<Compare>(compare)));

    if (it == std::cbegin(container))
        return {};
    --it;
    return *it;
}

template <typename Container, typename Item, typename Compare = std::less<ValueType<Container>>>
#if __cplusplus >= 202002L
    requires BinaryPredicateOnContainerValues<Compare, Container>
    && ContainerOfType<Container, Item>
#endif
std::optional<Item> max_value_less_than_unordered(const Container &container, const Item &&needle,
                                                  Compare &&compare = {})
{
    auto _compare = detail::to_function_object(std::forward<Compare>(compare));

    std::optional<Item> result;

    for (const auto &item : container) {
        if (_compare(item, needle) && (!result || _compare(*result, item))) {
            result = item;
        }
    }

    return result;
}

template <typename Container, typename Item, typename Compare = std::less<ValueType<Container>>>
#if __cplusplus >= 202002L
    requires BinaryPredicateOnContainerValues<Compare, Container>
    && ContainerOfType<Container, Item>
#endif
std::optional<Item> min_value_greater_than(const Container &container, Item &&needle,
                                           Compare &&compare = {})
{
    auto it =
        std::upper_bound(std::cbegin(container), std::cend(container), std::forward<Item>(needle),
                         detail::to_function_object(std::forward<Compare>(compare)));
    if (it == std::cend(container))
        return {};

    return *it;
}

template <typename Container, typename Item, typename Compare = std::less<ValueType<Container>>>
#if __cplusplus >= 202002L
    requires BinaryPredicateOnContainerValues<Compare, Container>
    && ContainerOfType<Container, Item>
#endif
std::optional<Item> min_value_greater_than_unordered(const Container &container,
                                                     const Item &&needle, Compare &&compare = {})
{
    auto _compare = detail::to_function_object(std::forward<Compare>(compare));

    std::optional<Item> result;

    for (const auto &item : container) {
        if (_compare(needle, item) && (!result || _compare(item, *result))) {
            result = item;
        }
    }

    return result;
}
#endif

// -------------------- is_permutation --------------------
template <typename Container1, typename Container2,
          typename Compare = std::equal_to<ValueType<Container1>>>
#if __cplusplus >= 202002L
    requires BinaryPredicateOnContainerValues<Compare, Container1>
    && BinaryPredicateOnContainerValues<Compare, Container2>
#endif
bool is_permutation(const Container1 &container1, const Container2 &container2,
                    Compare &&compare = {})
{
    return std::is_permutation(std::cbegin(container1), std::cend(container1),
                               std::cbegin(container2), std::cend(container2),
                               detail::to_function_object(std::forward<Compare>(compare)));
}

// -------------------- accumulate --------------------
template <typename Container, typename BinaryOperation = std::plus<ValueType<Container>>,
          typename ReturnType = remove_cvref_t<detail::return_type_of_t<BinaryOperation>>>
#if __cplusplus >= 202002L
    requires std::is_invocable_r_v<ReturnType, BinaryOperation, ReturnType, ValueType<Container>>
#endif
ReturnType accumulate(const Container &container, BinaryOperation &&accumulateFunction = {},
                      ReturnType initialValue = {})
{
    auto range = read_iterator_wrapper(container);
    return std::accumulate(
        range.begin(), range.end(), initialValue,
        detail::to_function_object(std::forward<BinaryOperation>(accumulateFunction)));
}

// -------------------- accumulate_if --------------------
template <typename Container, typename BinaryOperation, typename UnaryPredicate,
          typename ReturnType = remove_cvref_t<detail::return_type_of_t<BinaryOperation>>>
#if __cplusplus >= 202002L
    requires std::is_invocable_r_v<ReturnType, BinaryOperation, ReturnType, ValueType<Container>>
    && UnaryPredicateOnContainerValues<UnaryPredicate, Container>
#endif
ReturnType accumulate_if(const Container &container, BinaryOperation &&accumulate,
                         UnaryPredicate &&predicate, ReturnType initialValue = {})
{
    auto predicateFunction = detail::to_function_object(std::forward<UnaryPredicate>(predicate));
    auto accumulateFunction = detail::to_function_object(std::forward<BinaryOperation>(accumulate));
    auto fn = [&](const ReturnType &subResult, const ValueType<Container> &item) -> ReturnType {
        if (predicateFunction(item))
            return accumulateFunction(subResult, item);
        else
            return subResult;
    };

    return kdalgorithms::accumulate(container, fn, initialValue);
}
// -------------------- get_first_match --------------------
#if __cplusplus >= 201703L
template <typename Container, typename UnaryPredicate>
#if __cplusplus >= 202002L
    requires UnaryPredicateOnContainerValues<UnaryPredicate, Container>
#endif
std::optional<ValueType<Container>> get_match(const Container &container,
                                              UnaryPredicate &&predicate)
{
    auto result = kdalgorithms::find_if(container, std::forward<UnaryPredicate>(predicate));
    if (result)
        return *result;
    else
        return {};
}
#endif

// -------------------- get_first_match_or_default --------------------
template <typename Container, typename UnaryPredicate>
#if __cplusplus >= 202002L
    requires UnaryPredicateOnContainerValues<UnaryPredicate, Container>
#endif
ValueType<Container> get_match_or_default(const Container &container, UnaryPredicate &&predicate,
                                          const ValueType<Container> &defaultValue = {})
{
    auto result = kdalgorithms::find_if(container, std::forward<UnaryPredicate>(predicate));
    if (result)
        return *result;
    else
        return defaultValue;
}

// -------------------- remove_duplicates --------------------
enum SortOption { do_sort, do_not_sort };
template <typename Container>
auto remove_duplicates(Container &container, SortOption sort)
{
    if (sort == do_sort)
        kdalgorithms::sort(container);
    auto it = std::unique(std::begin(container), std::end(container));
    auto count = std::distance(it, std::end(container));
    container.erase(it, std::end(container));
    return count;
}

// -------------------- has_duplicates --------------------
template <typename Container>
bool has_duplicates(Container &&container, SortOption sort)
{
    auto hasDuplicates = [](const remove_cvref_t<Container> &container) {
        auto pos = std::adjacent_find(std::cbegin(container), std::cend(container));
        return pos != std::cend(container);
    };

    if (sort == do_not_sort || std::is_sorted(std::cbegin(container), std::cend(container)))
        return hasDuplicates(container);
    else
        return hasDuplicates(sorted(std::forward<Container>(container)));
}

// -------------------- erase / erase_if --------------------
namespace detail {
    // erase/erase_if fallbacks for the case that C++20-style uniform erase is not available
    template <typename Container, typename Value>
    auto erase_fallback(Container &container, Value &&value)
    {
        auto it =
            std::remove(std::begin(container), std::end(container), std::forward<Value>(value));
        auto count = std::distance(it, std::end(container));
        container.erase(it, std::end(container));
        return count;
    }

    template <typename Container, typename UnaryPredicate>
    auto erase_if_fallback(Container &container, UnaryPredicate &&predicate)
    {
        auto it =
            std::remove_if(std::begin(container), std::end(container),
                           detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
        auto count = std::distance(it, std::end(container));
        container.erase(it, std::end(container));
        return count;
    }

} // namespace detail

template <typename Container, typename Value>
#if __cplusplus >= 202002L
    requires ContainerOfType<Container, Value>
#endif
auto erase(Container &container, Value &&value)
{
#if __cplusplus >= 202002L
    using std::erase;
    if constexpr (requires { erase(container, std::forward<Value>(value)); }) {
        return erase(container, std::forward<Value>(value));
    } else {
        return detail::erase_fallback(container, std::forward<Value>(value));
    }
#else
    return detail::erase_fallback(container, std::forward<Value>(value));
#endif
}

template <typename Container, typename UnaryPredicate>
#if __cplusplus >= 202002L
    requires UnaryPredicateOnContainerValues<UnaryPredicate, Container>
#endif
auto erase_if(Container &container, UnaryPredicate &&predicate)
{
#if __cplusplus >= 202002L
    using std::erase_if;
    if constexpr (requires {
                      erase_if(container,
                               detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
                  }) {
        return erase_if(container,
                        detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
    } else {
        return detail::erase_if_fallback(
            container, detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
    }
#else
    return detail::erase_if_fallback(
        container, detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
#endif
}
// -------------------- index_of_match --------------------
template <typename Container, typename UnaryPredicate>
auto index_of_match(const Container &container, UnaryPredicate &&predicate)
{
    auto result = kdalgorithms::find_if(container, std::forward<UnaryPredicate>(predicate));
    return result.has_result() ? std::distance(result.begin, result.iterator) : -1;
}

// -------------------- iota --------------------
template <template <typename...> class Container = std::vector, typename Value>
Container<Value> iota(Value initial, int count)
{
    Container<Value> result(count);
    std::iota(std::begin(result), std::end(result), initial);
    return result;
}

template <template <typename...> class Container = std::vector>
Container<int> iota(int count)
{
    Container<int> result(count);
    std::iota(std::begin(result), std::end(result), 0);
    return result;
}

// -------------------- partitioned --------------------
template <typename T>
struct partition_result
{
    T in;
    T out;
};

template <typename ResultContainer, typename Container, typename UnaryPredicate>
#if __cplusplus >= 202002L
    requires UnaryPredicateOnContainerValues<UnaryPredicate, Container>
    && std::is_convertible_v<ValueType<Container>, ValueType<ResultContainer>>
#endif
auto partitioned(Container &&container, UnaryPredicate predicate)
{
    partition_result<remove_cvref_t<ResultContainer>> result;
    auto inInserter = detail::insert_wrapper(result.in);
    auto outInserter = detail::insert_wrapper(result.out);

    auto forward = [](auto &&value) {
        if (!std::is_lvalue_reference<Container>::value)
            return std::move(value);
        else
            return value;
    };

    for (auto &val : container) {
        if (detail::to_function_object(std::forward<UnaryPredicate>(predicate))(val))
            *(++inInserter) = forward(val);
        else
            *(++outInserter) = forward(val);
    }
    return result;
}

template <typename Container, typename UnaryPredicate>
auto partitioned(Container &&container, UnaryPredicate predicate)
{
    return partitioned<Container, Container, UnaryPredicate>(
        std::forward<Container>(container), std::forward<UnaryPredicate>(predicate));
}

template <template <typename...> class ResultContainer, typename Container, typename UnaryPredicate>
auto partitioned(Container &&container, UnaryPredicate predicate)
{
    return partitioned<ResultContainer<ValueType<Container>>>(
        std::forward<Container>(container), std::forward<UnaryPredicate>(predicate));
}

// -------------------- multi_partitioned --------------------
#if __cplusplus >= 202002L
namespace detail {
    template <typename ResultContainer, typename InputContainer, typename KeyFunction>
    constexpr bool KeyFunctionMapsContainerValueToMapKey =
        std::is_convertible_v<detail::invoke_result_t<KeyFunction, ValueType<InputContainer>>,
                              typename ResultContainer::key_type>;

    template <typename ResultContainer, typename InputContainer>
    constexpr bool InputValueCanConvertToResultContainerValue =
        std::is_convertible_v<ValueType<InputContainer>,
                              typename ResultContainer::mapped_type::value_type>;
}
#endif

template <typename ResultContainer, typename InputContainer, typename KeyFunction>
#if __cplusplus >= 202002L
    requires std::is_invocable_v<KeyFunction, ValueType<InputContainer>>
    && detail::KeyFunctionMapsContainerValueToMapKey<ResultContainer, InputContainer, KeyFunction>
    && detail::InputValueCanConvertToResultContainerValue<ResultContainer, InputContainer>
#endif
auto multi_partitioned(InputContainer &&container, KeyFunction keyFunction)
{
    ResultContainer result;
    auto iterators = read_iterator_wrapper(std::forward<InputContainer>(container));
    for (auto it = iterators.begin(); it != iterators.end(); ++it) {
        // Originally the code below simply gave *it to `invoke`:
        // result[detail::invoke(keyFunction, *it)].push_back(*it);
        // Now imagine two things:
        // 1) `container` is an r-value
        // 2) keyFunction takes it's one parameter as a value (in contrast to a reference)
        // *it would yield an rvalue (due to the move iterators returned by read_iterator_wrapper
        // so the call to the function would move over the value, with the result that
        // what is pushed to `result` would be a default constructed value
        // This is tested in multi_partitioned_with_function_taking_a_value
        const auto &cvalue = *it;
        result[detail::invoke(keyFunction, cvalue)].push_back(*it);
    }
    return result;
}

template <template <typename...> class ResultContainerClass, typename InputContainer,
          typename KeyFunction>
#if __cplusplus >= 202002L
    requires std::is_invocable_v<KeyFunction, ValueType<InputContainer>>
#endif
auto multi_partitioned(InputContainer &&container, KeyFunction keyFunction)
{
    using KeyType = remove_cvref_t<decltype(detail::invoke(keyFunction, *container.begin()))>;
    using ResultContainer = ResultContainerClass<KeyType, std::remove_reference_t<InputContainer>>;
    return multi_partitioned<ResultContainer>(std::forward<InputContainer>(container),
                                              std::forward<KeyFunction>(keyFunction));
}

template <typename InputContainer, typename KeyFunction>
#if __cplusplus >= 202002L
    requires std::is_invocable_v<KeyFunction, ValueType<InputContainer>>
#endif
auto multi_partitioned(InputContainer &&container, KeyFunction keyFunction)
{
    return multi_partitioned<std::map>(std::forward<InputContainer>(container),
                                       std::forward<KeyFunction>(keyFunction));
}

template <typename Container, typename UnaryFunction>
#if __cplusplus >= 202002L
    requires UnaryFunctionOnContainerValues<UnaryFunction, Container>
#endif

// -------------------- for_each --------------------

void for_each(Container &&container, UnaryFunction &&function)
{
    auto range = read_iterator_wrapper(std::forward<Container>(container));
    std::for_each(range.begin(), range.end(),
                  detail::to_function_object(std::forward<UnaryFunction>(function)));
}
} // namespace kdalgorithms
