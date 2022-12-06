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
#include "read_iterator_wrapper.h"
#include "reserve_helper.h"
#include "shared.h"
#include "to_function_object.h"
#include <algorithm>
#include <functional>
#include <iterator>

namespace kdalgorithms {
namespace detail {
    template <typename ResultContainer, typename InputContainer, typename UnaryPredicate>
    ResultContainer filtered(InputContainer &&input, UnaryPredicate &&predicate)
    {
        ResultContainer result;
        detail::reserve(result, input.size());

        auto range = read_iterator_wrapper(std::forward<InputContainer>(input));
        std::copy_if(range.begin(), range.end(), detail::insert_wrapper(result),
                     std::forward<UnaryPredicate>(predicate));
        return result;
    }
}

template <typename Container, typename UnaryPredicate>
auto filtered(Container &&input, UnaryPredicate &&predicate)
#if __cplusplus >= 202002L
    requires UnaryPredicateOnContainerValues<UnaryPredicate, Container>
#endif
{
    return detail::filtered<remove_cvref_t<Container>>(
        std::forward<Container>(input),
        detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
}

template <template <typename...> class ResultContainer, typename InputContainer,
          typename UnaryPredicate>
ResultContainer<ValueType<InputContainer>> filtered(InputContainer &&input,
                                                    UnaryPredicate &&predicate)
#if __cplusplus >= 202002L
    requires UnaryPredicateOnContainerValues<UnaryPredicate, InputContainer>
#endif
{
    return detail::filtered<ResultContainer<ValueType<InputContainer>>>(
        std::forward<InputContainer>(input),
        detail::to_function_object(std::forward<UnaryPredicate>(predicate)));
}

// -------------------- filter --------------------
template <typename Container, typename UnaryPredicate>
void filter(Container &input, UnaryPredicate &&predicate)
#if __cplusplus >= 202002L
    requires UnaryPredicateOnContainerValues<UnaryPredicate, Container>
#endif
{
#if __cplusplus < 201703L
    auto it = std::remove_if(std::begin(input), std::end(input),
                             [&predicate](const ValueType<Container> &v) { return !predicate(v); });
#else
    auto it = std::remove_if(std::begin(input), std::end(input),
                             std::not_fn(std::forward<UnaryPredicate>(predicate)));
#endif
    input.erase(it, std::end(input));
}

} // namespace kdalgorithms
