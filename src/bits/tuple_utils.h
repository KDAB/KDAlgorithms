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

#include <tuple>
namespace kdalgorithms {
namespace detail {

    template <std::size_t... Indices, typename... Types, typename Fn>
    void tuple_apply_helper(std::tuple<Types...> &tuple, std::index_sequence<Indices...>, Fn fn)
    {
        int dummy[sizeof...(Types)] = {(fn(std::get<Indices>(tuple)), 0)...};
        (void)dummy;
    }

    template <typename... Types, typename Fn>
    void tuple_apply(std::tuple<Types...> &tuple, Fn fn)
    {
        tuple_apply_helper(tuple, std::index_sequence_for<Types...>{}, fn);
    }

    template <std::size_t... Indices, typename... Values, typename Fn>
    auto tuple_apply_with_result_helper(const std::tuple<Values...> &values,
                                        std::index_sequence<Indices...>, Fn fn)
    {
        return std::make_tuple(fn(std::get<Indices>(values))...);
    }

    template <typename... Iterators, typename Fn>
    auto tuple_apply_with_result(const std::tuple<Iterators...> &iterators, Fn fn)
    {
        return tuple_apply_with_result_helper(iterators, std::index_sequence_for<Iterators...>{},
                                              fn);
    }

} // namespace detail
} // namespace kdalgorithms
