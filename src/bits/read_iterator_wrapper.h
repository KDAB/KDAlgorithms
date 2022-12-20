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
#include "method_tests.h"
#include <iterator>
#include <tuple>
#include <type_traits>

namespace kdalgorithms {

namespace detail {
    template <typename Iterator>
    class IteratorPair
    {
    public:
        IteratorPair(Iterator begin, Iterator end)
            : m_begin(std::move(begin))
            , m_end(std::move(end))
        {
        }
        inline auto begin() { return m_begin; }
        inline auto end() { return m_end; }

    private:
        Iterator m_begin;
        Iterator m_end;
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
        return make_iteratorPair(std::cbegin(container), std::cend(container));
    }

    // used for r-value containers
    template <typename Container>
    auto read_iterator_wrapper_helper(Container &&container, std::false_type, std::false_type)
    {
        return make_iteratorPair(std::make_move_iterator(std::begin(container)),
                                 std::make_move_iterator(std::end(container)));
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
