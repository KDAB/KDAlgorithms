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
#include "method_tests.h"
#include <iterator>
#include <tuple>
#include <type_traits>

namespace kdalgorithms {

namespace detail {
    template <typename Iterator>
    struct IteratorPair
    {
        Iterator begin;
        Iterator end;
    };

    template <typename Iterator>
    auto make_iteratorPair(Iterator begin, Iterator end)
    {
        return IteratorPair<Iterator>{begin, end};
    }

    // used for l-value containers
    template <typename Container>
    auto read_iterator_wrapper_helper(const Container &container, std::true_type, std::false_type)
    {
        return make_iteratorPair(container.cbegin(), container.cend());
    }

    // used for r-value containers
    template <typename Container>
    auto read_iterator_wrapper_helper(Container &&container, std::false_type, std::false_type)
    {
        return make_iteratorPair(std::make_move_iterator(container.begin()),
                                 std::make_move_iterator(container.end()));
    }

    // Used for QHash and QMap containers
    template <typename Container, typename T>
    auto read_iterator_wrapper_helper(Container &&container, T, std::true_type)
    {
        return make_iteratorPair(container.constKeyValueBegin(), container.constKeyValueEnd());
    }
}

// Depending on the value type of container - either l-value or r-value
// return iterators that either copy elements out or that move elements out.
template <typename Container>
auto read_iterator_wrapper(Container &&container)
{
    return detail::read_iterator_wrapper_helper(
        std::forward<Container>(container), typename std::is_lvalue_reference<Container>::type(),
        detail::has_keyValueBegin<Container>());
}

} // namespace kdalgorithms
