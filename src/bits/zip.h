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

#include "shared.h"
#include "tuple_utils.h"
#include <iterator>
#include <tuple>
#include <utility>
#include <vector>

namespace kdalgorithms {
namespace detail {
    template <std::size_t... Indices, typename... Iterators>
    bool at_end_helper(std::tuple<Iterators...> &iterators, std::index_sequence<Indices...>)
    {
        bool anyAtEnd = false;
        int dummy[sizeof...(Iterators)] = {
            ((anyAtEnd = anyAtEnd
                  | (std::get<Indices>(iterators).begin() == std::get<Indices>(iterators).end())),
             0)...};
        (void)dummy;
        return anyAtEnd;
    }

    template <typename... Iterators>
    bool at_end(std::tuple<Iterators...> &iterators)
    {
        return at_end_helper(iterators, std::index_sequence_for<Iterators...>{});
    }
} // namespace detail

template <template <typename...> class ResultContainer = std::vector, typename... Containers>
auto zip(Containers &&...containers)
{
    auto iterators = make_tuple(read_iterator_wrapper(std::forward<Containers>(containers))...);

    using TupleValueType = std::tuple<ValueType<Containers>...>;
    ResultContainer<TupleValueType> result;
    while (!detail::at_end(iterators)) {
        auto oneZip =
            detail::tuple_apply_with_result(iterators, [](auto it) { return *(it.begin()); });
        result.push_back(std::move(oneZip));
        detail::tuple_apply(iterators, [](auto &it) { ++it; });
    }
    return result;
}
}
