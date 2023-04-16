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
#include "operators.h"
#include "read_iterator_wrapper.h"
#include "shared.h"
#include "to_function_object.h"
#include <algorithm>
#include <cassert>
#include <memory>

namespace kdalgorithms {

namespace detail {
    template <typename Container>
    auto find_if_iterator_wrapper_helper(const Container &container, std::false_type)
    {
        return make_iteratorPair(std::cbegin(container), std::cend(container));
    }

    template <typename Container>
    auto find_if_iterator_wrapper_helper(Container &container, std::false_type)
    {
        return make_iteratorPair(std::begin(container), std::end(container));
    }

    // Used for QHash and QMap containers
    template <typename Container>
    auto find_if_iterator_wrapper_helper(const Container &container, std::true_type)
    {
        return make_iteratorPair(container.constKeyValueBegin(), container.constKeyValueEnd());
    }

    // Used for QHash and QMap containers
    template <typename Container>
    auto find_if_iterator_wrapper_helper(Container &container, std::true_type)
    {
        return make_iteratorPair(container.keyValueBegin(), container.keyValueEnd());
    }

    // Return a proper iterator pair for the container.
    template <typename Container>
    auto find_if_iterator_wrapper(Container &&container)
    {
        return find_if_iterator_wrapper_helper(std::forward<Container>(container),
                                               detail::has_keyValueBegin<Container>());
    }
}

template <typename Iterator>
class iterator_result
{
public:
    iterator_result(Iterator iterator, Iterator begin, Iterator end)
        : iterator(std::move(iterator))
        , begin(std::move(begin))
        , end(std::move(end))
    {
    }
    operator bool() const { return iterator != end; }
    bool has_result() const { return iterator != end; }
    decltype(auto) operator*() const
    {
        assert(iterator != end);
        return *iterator;
    }
    decltype(auto) operator->() const
    {
        assert(iterator != end);
        return iterator;
    }

    Iterator iterator;
    Iterator begin;
    Iterator end;
};

template <typename Iterator>
class iterator_result_rvalue
{
public:
    using value_type = typename Iterator::value_type;
    iterator_result_rvalue(Iterator iterator, Iterator end)
    {
        if (iterator != end)
            m_value = std::make_unique<value_type>(std::move(*iterator));
    }
    operator bool() const { return m_value.get() != nullptr; }
    bool has_result() const { return m_value.get() != nullptr; }
    decltype(auto) operator*() const { return *m_value; }
    decltype(auto) operator->() const { return m_value.get(); }

    std::unique_ptr<value_type> m_value;
};

namespace detail {
    // In the case where the container given to find_if is an r-value, we have to keep a copy of the
    // found value, as the iterator to it will outlive the value.
    // For details see https://github.com/KDAB/KDAlgorithms/issues/50
    template <typename Iterator>
    auto create_result(Iterator iterator, Iterator begin, Iterator end,
                       std::true_type /*is_lvalue_reference*/)
    {
        return iterator_result<Iterator>(std::move(iterator), std::move(begin), std::move(end));
    }

    template <typename Iterator>
    auto create_result(Iterator iterator, Iterator /*begin*/, Iterator end,
                       std::false_type /*is_lvalue_reference*/)
    {
        return iterator_result_rvalue<Iterator>(std::move(iterator), std::move(end));
    };
}

template <typename Container, typename UnaryPredicate>
#if __cplusplus >= 202002L
    requires UnaryPredicateOnContainerValues<UnaryPredicate, Container>
#endif
auto find_if(Container &&container, UnaryPredicate &&predicate)
{
    auto range = detail::find_if_iterator_wrapper(container);
    auto it = std::find_if(range.begin(), range.end(),
                           detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
    using Iterator = decltype(range.begin());
    return detail::create_result<Iterator>(std::move(it), std::move(range.begin()),
                                           std::move(range.end()),
                                           std::is_lvalue_reference<Container>());
}

template <typename Container, typename Predicate>
auto mutable_find_if(Container &container, Predicate &&predicate)
{
    auto range = detail::find_if_iterator_wrapper(container);
    auto it = std::find_if(range.begin(), range.end(),
                           detail::to_function_object(std::forward<Predicate>(predicate)));
    using Iterator = decltype(range.begin());
    return iterator_result<Iterator>(it, range.begin(), range.end());
}

template <typename Container, typename UnaryPredicate>
#if __cplusplus >= 202002L
    requires UnaryPredicateOnContainerValues<UnaryPredicate, Container>
#endif
auto find_if_not(Container &&container, UnaryPredicate &&predicate)
{
    using namespace kdalgorithms::operators;
    return find_if(std::forward<Container>(container), !predicate);
}

template <typename Container, typename Predicate>
auto mutable_find_if_not(Container &container, Predicate &&predicate)
{
    using namespace kdalgorithms::operators;
    return mutable_find_if(container, !predicate);
}

}
