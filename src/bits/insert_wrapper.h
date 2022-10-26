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
#include "shared.h"
#include <iterator>
#include <utility>

namespace kdalgorithms {
namespace detail {
    template <class Container, typename T>
    using has_push_back = decltype(std::declval<Container &>().push_back(std::declval<T>()));

    template <class Container, typename Value>
    using has_insert = decltype(std::declval<Container &>().insert(std::declval<Value>()));

    template <typename Container>
    auto insert_wrapper(
        Container &c,
        std::enable_if_t<
            detail::is_detected_v<has_push_back, Container, typename Container::value_type>, int> =
            0)
    {
        return std::back_inserter(c);
    }

    template <typename Container>
    auto insert_wrapper(
        Container &c,
        std::enable_if_t<
            detail::is_detected_v<has_insert, Container, typename Container::value_type>, int> = 0)
    {
        return std::inserter<Container>(c, c.end());
    }

    // similar to std::inserter, but for QMap/QHash which usually accept a value in their operator=
    template <typename T>
    class qmap_inserter
    {
    public:
        using iterator_category = std::output_iterator_tag;
        using value_type = ValueType<T>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

        qmap_inserter(T &map)
            : m_map(map)
        {
        }

        qmap_inserter &operator++() { return *this; }
        qmap_inserter &operator*() { return *this; }
        qmap_inserter &
        operator=(const std::pair<typename T::key_type, typename T::mapped_type> &pair)
        {
            m_map.insert(pair.first, pair.second);
            return *this;
        }

    private:
        T &m_map;
    };

    template <typename Container>
    auto insert_wrapper(Container &c,
                        std::enable_if_t<detail::has_keyValueBegin_v<Container>, int> = 0)
    {
        return qmap_inserter<Container>(c);
    }

} // namespace detail
} // namespace kdalgorithms
