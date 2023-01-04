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

#include "is_detected.h"
#include "shared.h"

namespace kdalgorithms {

namespace detail {
    namespace tests {
        template <typename Container>
        using has_reserve = decltype(std::declval<Container>().reserve(42));

        template <typename Container>
        using has_keyValueBegin = decltype(std::declval<Container &>().keyValueBegin());

        template <typename Generator, typename Size>
        using has_generator_with_index = invoke_result_t<Generator, Size>;

        template <typename Container>
        using value_type = typename remove_cvref_t<Container>::value_type;

        template <typename Container>
        using key_type = typename remove_cvref_t<Container>::key_type;

        template <typename Container>
        using mapped_type = typename remove_cvref_t<Container>::mapped_type;

        template <typename Container>
        constexpr bool has_value_type = is_detected_v<value_type, Container>;

        template <typename Container>
        constexpr bool has_key_and_mapped_type = is_detected_v<key_type, Container> and is_detected_v<mapped_type, Container>;

        template <typename Container, bool value_type, typename Void = void>
        struct has_push_back_helper : std::false_type { };

        template <typename Container>
        struct has_push_back_helper<Container, true, detail::void_t<
            decltype(std::declval<Container &>().push_back(std::declval<typename Container::value_type>()))>> : std::true_type {};

        template <typename Container, bool value_type, typename Void = void>
        struct has_insert_helper : std::false_type { };

        template <typename Container>
        struct has_insert_helper<Container, true, detail::void_t<
            decltype(std::declval<Container &>().insert(std::declval<typename Container::value_type>()))>> : std::true_type {};

        template <class Container>
        constexpr bool has_push_back = has_push_back_helper<Container, has_value_type<Container>>::value;

        template <class Container>
        constexpr bool has_insert = has_insert_helper<Container, has_value_type<Container>>::value;
    }

    template <typename Container>
    constexpr bool has_reserve_method_v = detail::is_detected_v<tests::has_reserve, Container>;

    template <typename Container>
    using has_keyValueBegin = detail::is_detected<tests::has_keyValueBegin, Container>;

    template <typename Container>
    constexpr bool has_keyValueBegin_v = detail::is_detected_v<tests::has_keyValueBegin, Container>;

    template <typename Generator, typename Size>
    constexpr bool has_generator_with_index_v =
        detail::is_detected_v<tests::has_generator_with_index, Generator, Size>;

} // namespace detail
} // namespace kdalgorithms
