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

#include "is_detected.h"
namespace kdalgorithms {

namespace detail {
    namespace tests {
        template <typename Container>
        using has_reserve = decltype(std::declval<Container>().reserve(42));

        template <typename Container>
        using has_keyValueBegin = decltype(std::declval<Container &>().keyValueBegin());
    }

    template <typename Container>
    constexpr bool has_reserve_method_v = detail::is_detected_v<tests::has_reserve, Container>;

    template <typename Container>
    using has_keyValueBegin = detail::is_detected<tests::has_keyValueBegin, Container>;

    template <typename Container>
    constexpr bool has_keyValueBegin_v = detail::is_detected_v<tests::has_keyValueBegin, Container>;

} // namespace detail
} // namespace kdalgorithms
