/****************************************************************************
**
** This file is part of the KDAlgorithms
**
** SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#pragma once

#include "bits/filter.h"
#include "bits/find_if.h"
#include "bits/insert_wrapper.h"
#include "bits/method_tests.h"
#include "bits/operators.h"
#include "bits/read_iterator_wrapper.h"
#include "bits/return_type_trait.h"
#include "bits/shared.h"
#include "bits/to_function_object.h"
#include "bits/transform.h"
#include <algorithm>
#include <numeric>
#include <type_traits>
#include <utility>

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
#if __cplusplus >= 201703L
    if constexpr (detail::has_reserve_method_v<OutputContainer>) {
        output.reserve(input.size() + output.size());
    }
#endif
    auto range = read_iterator_wrapper(std::forward<InputContainer>(input));
    std::copy(range.begin, range.end, detail::insert_wrapper(output));
}

// -------------------- all_of / any_of / none_of --------------------
template <typename Container, typename UnaryPredicate>
bool any_of(const Container &container, UnaryPredicate &&predicate)
{
    auto range = read_iterator_wrapper(container);
    return std::any_of(range.begin, range.end,
                       detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
}

template <typename Container, typename UnaryPredicate>
bool all_of(const Container &container, UnaryPredicate &&predicate)
{
    auto range = read_iterator_wrapper(container);
    return std::all_of(range.begin, range.end,
                       detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
}

template <typename Container, typename UnaryPredicate>
bool none_of(const Container &container, UnaryPredicate &&predicate)
{
    auto range = read_iterator_wrapper(container);
    return std::none_of(range.begin, range.end,
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
    return std::count_if(range.begin, range.end,
                         detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
}

// -------------------- min / max --------------------
#if __cplusplus >= 201703L
template <typename Container, typename Compare = std::less<ValueType<Container>>>
#if __cplusplus >= 202002L
requires BinaryPredicateOnContainerValues<Compare, Container>
#endif
    std::optional<ValueType<Container>> max_element(const Container &container,
                                                    Compare &&compare = {})
{
    if (container.empty())
        return {};

    return *std::max_element(std::cbegin(container), std::cend(container),
                             detail::to_function_object(std::forward<Compare>(compare)));
}

template <typename Container, typename Compare = std::less<ValueType<Container>>>
#if __cplusplus >= 202002L
requires BinaryPredicateOnContainerValues<Compare, Container>
#endif
    std::optional<ValueType<Container>> min_element(const Container &container,
                                                    Compare &&compare = {})
{
    if (container.empty())
        return {};
    return *std::min_element(std::cbegin(container), std::cend(container),
                             detail::to_function_object(std::forward<Compare>(compare)));
}
#endif

// -------------------- max_value_less_than / min_value_greater_than --------------------
#if __cplusplus >= 201703L
template <typename Container, typename Item, typename Compare = std::less<ValueType<Container>>>
#if __cplusplus >= 202002L
requires BinaryPredicateOnContainerValues<Compare, Container> && ContainerOfType<Container, Item>
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
requires BinaryPredicateOnContainerValues<Compare, Container> && ContainerOfType<Container, Item>
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
#endif

// -------------------- is_permutation --------------------
template <typename Container1, typename Container2,
          typename Compare = std::equal_to<ValueType<Container1>>>
#if __cplusplus >= 202002L
requires BinaryPredicateOnContainerValues<Compare, Container1> && BinaryPredicateOnContainerValues<
    Compare, Container2>
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
          typename ReturnType = remove_cvref_t<traits::return_type_of_t<BinaryOperation>>>
#if __cplusplus >= 202002L
requires std::is_invocable_r_v<ReturnType, BinaryOperation, ReturnType, ValueType<Container>>
#endif
    ReturnType accumulate(const Container &container, BinaryOperation &&accumulateFunction = {},
                          ReturnType initialValue = {})
{
    auto range = read_iterator_wrapper(container);
    return std::accumulate(
        range.begin, range.end, initialValue,
        detail::to_function_object(std::forward<BinaryOperation>(accumulateFunction)));
}

// -------------------- accumulate_if --------------------
template <typename Container, typename BinaryOperation, typename UnaryPredicate,
          typename ReturnType = remove_cvref_t<traits::return_type_of_t<BinaryOperation>>>
#if __cplusplus >= 202002L
requires std::is_invocable_r_v<
    ReturnType, BinaryOperation, ReturnType,
    ValueType<Container>> && UnaryPredicateOnContainerValues<UnaryPredicate, Container>
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
    ValueType<Container> get_match_or_default(const Container &container,
                                              UnaryPredicate &&predicate,
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
template <typename Container, typename Value>
#if __cplusplus >= 202002L
requires ContainerOfType<Container, Value>
#endif
auto erase(Container &container, Value &&value)
{
    auto it = std::remove(std::begin(container), std::end(container), std::forward<Value>(value));
    auto count = std::distance(it, std::end(container));
    container.erase(it, std::end(container));
    return count;
}

template <typename Container, typename UnaryPredicate>
#if __cplusplus >= 202002L
requires UnaryPredicateOnContainerValues<UnaryPredicate, Container>
#endif
auto erase_if(Container &container, UnaryPredicate &&predicate)
{
    auto it = std::remove_if(std::begin(container), std::end(container),
                             detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
    auto count = std::distance(it, std::end(container));
    container.erase(it, std::end(container));
    return count;
}

// -------------------- index_of_match --------------------
template <typename Container, typename UnaryPredicate>
auto index_of_match(const Container &container, UnaryPredicate &&predicate)
{
    auto result = kdalgorithms::find_if(container, std::forward<UnaryPredicate>(predicate));
    return result.has_result() ? std::distance(result.begin, result.iterator) : -1;
}

} // namespace kdalgorithms
