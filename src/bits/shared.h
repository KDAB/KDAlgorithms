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

#include <type_traits>
#include <utility>
#if __cplusplus >= 202002L
#include <concepts>
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
    template <typename T, typename = void>
    struct ValueTypeHelper
    {
        using value_type = typename T::value_type;
    };

    // QMap doesn't have the value_type typedef, so we have to build that ourselves.
    template <typename T>
    struct ValueTypeHelper<T, std::void_t<typename T::mapped_type>>
    {
        using value_type = std::pair<const typename T::key_type, typename T::mapped_type>;
    };
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

template <typename BinaryPredicate, typename Container>
concept BinaryPredicateOnContainerValues =
    std::is_invocable_r_v<bool, BinaryPredicate, ValueType<Container>, ValueType<Container>>;
#endif

} // namespace kdalgorithms
