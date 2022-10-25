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
#include <utility>

namespace kdalgorithms {
namespace detail {
    // similar to std::inserter, but calls insert with just one argument - required for QSet in Qt5
    template <typename T>
    class single_arg_inserter
    {
    public:
        using value_type = typename T::value_type;
        single_arg_inserter(T &set)
            : m_set(&set)
        {
        }

        single_arg_inserter &operator++() { return *this; }
        single_arg_inserter &operator*() { return *this; }
        single_arg_inserter &operator=(const value_type &value)
        {
            m_set->insert(value);
            return *this;
        }

    private:
        T *m_set;
    };

    template <class Container, typename T>
    using has_push_back = decltype(std::declval<Container &>().push_back(std::declval<T>()));

    template <class Container, typename T>
    using has_insert = decltype(std::declval<Container &>().insert(std::declval<T>()));

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
        return single_arg_inserter<Container>(c);
    }

    // similar to std::inserter, but for QMap/QHash which usually accept a value in their operator=
    template <typename T>
    class qmap_inserter
    {
    public:
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
