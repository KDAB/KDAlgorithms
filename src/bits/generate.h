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
#include "insert_wrapper.h"
#include "method_tests.h"
#include "shared.h"
#include <type_traits>
#include <vector>

namespace kdalgorithms {

namespace detail {
    template <typename Generator, typename Size>
    auto call_generator_helper(Generator &&generator, Size /*index*/, std::false_type)
    {
        return generator();
    }

    template <typename Generator, typename Size>
    auto call_generator_helper(Generator &&generator, Size index, std::true_type)
    {
        return generator(index);
    }

    template <typename Generator, typename Size>
    auto call_generator(Generator &&generator, Size index)
    {
        return call_generator_helper(
            std::forward<Generator>(generator), index,
            std::integral_constant<bool, has_generator_with_index_v<Generator, Size>>());
    }
} // detail

// -------------------- generate_n --------------------
template <typename Container, typename Size, typename Generator>
#if __cplusplus >= 202002L
    requires std::is_convertible_v<detail::invoke_result_t<Generator, int>, ValueType<Container>>
    || std::is_convertible_v<detail::invoke_result_t<Generator>, ValueType<Container>>
#endif
void generate_n(Container &container, Size count, Generator &&generator)
{
    detail::reserve(container, container.size() + count);

    auto iterator = detail::insert_wrapper(container);

    for (Size index = {}; index < count; ++index) {
        *(++iterator) =
            std::move(detail::call_generator(std::forward<Generator>(generator), index));
    }
}

// -------------------- generate_until --------------------
namespace detail {
    template <typename T>
    using pointed_to_type = remove_cvref_t<decltype(*std::declval<T>())>;

    template <typename Generator>
    using generator_value_type =
        detail::pointed_to_type<typename detail::invoke_result_t<Generator>>;
}
template <typename Container, typename Generator>
#if __cplusplus >= 202002L
    requires std::is_convertible_v<detail::generator_value_type<Generator>, ValueType<Container>>
#endif
auto generate_until(Generator &&generator)
{
    Container result;

    auto iterator = detail::insert_wrapper(result);

    bool more = true;
    do {
        auto value = generator();
        more = static_cast<bool>(value);
        if (more)
            *(++iterator) = std::move(*value);
    } while (more);
    return result;
}

template <template <typename...> class Container = std::vector, typename Generator>
auto generate_until(Generator &&generator)
{
    using ValueType = detail::generator_value_type<Generator>;
    return generate_until<Container<ValueType>>(std::forward<Generator>(generator));
}

} // kdalgorithms
