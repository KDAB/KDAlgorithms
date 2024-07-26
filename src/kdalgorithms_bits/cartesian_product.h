/****************************************************************************
**
** This file is part of KDAlgorithms
**
** SPDX-FileCopyrightText: 2024 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#pragma once

#include "transform.h"
#include "tuple_utils.h"
#include <tuple>
#include <vector>

namespace kdalgorithms {
namespace detail {
    template <template <typename...> class ResultContainerClass, typename Container>
    auto cartesian_product(Container &&arg)
    {
        return kdalgorithms::transformed<ResultContainerClass>(
            std::forward<Container>(arg), [](auto &&elm) { return std::make_tuple(elm); });
    }

    template <template <typename...> class ResultContainerClass, typename Container,
              typename... REST>
    auto cartesian_product(Container &&container, REST... args)
    {
        auto partialResult = cartesian_product<ResultContainerClass>(args...);

        using TupleType =
            detail::merge_tuple_types_t<ValueType<Container>, ValueType<decltype(partialResult)>>;
        ResultContainerClass<TupleType> result;
        for (auto &&item : container) {
            for (auto &&partialItem : partialResult) {
                result.push_back(std::tuple_cat(std::make_tuple(item), partialItem));
            }
        }
        return result;
    }
} // namespace detail

template <template <typename...> class ResultContainerClass = std::vector, typename... ARGS>
auto cartesian_product(ARGS... args)
{
    return detail::cartesian_product<ResultContainerClass>(std::forward<ARGS>(args)...);
}

} // namespace kdalgorithms
