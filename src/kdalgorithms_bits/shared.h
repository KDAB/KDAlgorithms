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

#include <type_traits>
#include <utility>
#if __cplusplus >= 202002L
#include <concepts>
#endif

#if __cplusplus >= 202302L || (!defined(__APPLE__) && __cplusplus >= 202002L)
#include <ranges>
#endif

namespace kdalgorithms {
// ----- helper functions for older C++ versions -----
#if __cplusplus >= 202002L
template <typename T>
using remove_cvref_t = std::remove_cvref_t<T>;
#else
template <typename T>
using remove_cvref_t = typename std::remove_const<typename std::remove_reference<T>::type>::type;
#endif

// -------------------- general helpers --------------------
namespace detail {
    template <class...>
    using void_t = void; // Is only introduced in C++17

    template <typename T, typename = void>
    struct ValueTypeHelper
    {
        using value_type = typename T::value_type;
    };

    // QMap doesn't have the value_type typedef, so we have to build that ourselves.
    template <typename T>
    struct ValueTypeHelper<T, void_t<typename T::mapped_type>>
    {
        using value_type = std::pair<const typename T::key_type, typename T::mapped_type>;
    };

#if __cplusplus >= 202302L || (!defined(__APPLE__) && __cplusplus >= 202002L)
    template <typename I, typename S>
    struct ValueTypeHelper<std::ranges::subrange<I, S>>
    {
        using value_type = std::iter_value_t<I>;
    };
#endif

// Wrapper around result_of and invoke_result
#if __cplusplus < 201703L
    template <typename FN, typename... ARGS>
    using invoke_result = std::result_of<FN(ARGS...)>;
#else
    template <typename FN, typename... ARGS>
    using invoke_result = std::invoke_result<FN, ARGS...>;
#endif

    template <typename FN, typename... ARGS>
    using invoke_result_t = typename invoke_result<FN, ARGS...>::type;

    // Test if two variables is at the same address.
    template <typename T, typename S>
    bool is_same_object(const T &t, const S &s)
    {
        return static_cast<const void *>(&t) == static_cast<const void *>(&s);
    }
}

template <typename Container>
using ValueType = typename detail::ValueTypeHelper<remove_cvref_t<Container>>::value_type;

// -------------------- concepts --------------------
#if __cplusplus >= 202002L
template <typename Container, typename Value>
concept ContainerOfType = std::convertible_to<Value, ValueType<Container>>;

template <typename UnaryPredicate, typename Container>
concept UnaryPredicateOnContainerValues =
    std::is_invocable_r_v<bool, UnaryPredicate, ValueType<Container>>;

template <typename UnaryFunction, typename Container>
concept UnaryFunctionOnContainerValues = std::is_invocable_v<UnaryFunction, ValueType<Container>>;

template <typename BinaryPredicate, typename Container>
concept BinaryPredicateOnContainerValues =
    std::is_invocable_r_v<bool, BinaryPredicate, ValueType<Container>, ValueType<Container>>;
#endif

} // namespace kdalgorithms
