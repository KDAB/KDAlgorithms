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

// Inspired from std::experimental::is_detected
// See https://en.cppreference.com/w/cpp/experimental/is_detected

namespace kdalgorithms {
namespace detail {
    template< typename... Ts >
    using void_t = void;

    template <class AlwaysVoid, template <class...> class, typename... Args>
    struct detector : std::false_type
    {
    };

    template <template <class...> class Op, typename... Args>
    struct detector<void_t<Op<Args...>>, Op, Args...> : std::true_type
    {
    };

    template <template <class...> class Op, typename... Args>
    using is_detected = detector<void, Op, Args...>;

    template <template <class...> class Op, typename... Args>
    static constexpr auto is_detected_v = detector<void, Op, Args...>::value;
}
}
