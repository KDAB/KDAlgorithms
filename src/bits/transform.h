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

#include "insert_wrapper.h"
#include "reserve_helper.h"
#include "shared.h"
#include "to_function_object.h"
#include <algorithm>

namespace kdalgorithms {
namespace detail {
    // Utility function used for meta type programming.
    // See TransformedType for details.
    template <typename ResultItemType, template <typename...> class Container, typename ItemType>
    Container<ResultItemType> replace_item_value_type(const Container<ItemType> &)
    {
    }

    // ResultItemType = resulting type when applying Transform on an item in Container.
    template <typename Container, typename Transform>
    using ResultItemType =
        std::remove_reference_t<detail::invoke_result_t<Transform, ValueType<Container>>>;

    // Given a Container<Input> and a Transform which converts from Input to Output.
    // TransformedType = Container<Output>
    template <typename Container, typename Transform>
    using TransformedType =
        remove_cvref_t<decltype(replace_item_value_type<ResultItemType<Container, Transform>>(
            std::declval<Container>()))>;

    // Version used for l-values or where the container type changes
    template <typename ResultContainer, typename InputContainer, typename Transform>
    ResultContainer transformed(InputContainer &&input, Transform &&transform, std::true_type)
    {
        ResultContainer result;
        detail::reserve(result, input.size());
        auto range = read_iterator_wrapper(std::forward<InputContainer>(input));
        std::transform(range.begin(), range.end(), detail::insert_wrapper(result),
                       std::forward<Transform>(transform));
        return result;
    }

    // optimized version to use for r-values where container type is the same
    // in-place editing is possible in that case.
    template <typename ResultContainer, typename InputContainer, typename Transform>
    ResultContainer transformed(InputContainer &&input, Transform &&transform,
                                std::false_type /* r-value and same containers */)
    {
        std::transform(std::begin(input), std::end(input), std::begin(input),
                       std::forward<Transform>(transform));
        return std::forward<InputContainer>(input);
    }

    template <typename InputContainer, typename ResultContainer>
    static constexpr std::integral_constant<
        bool,
        std::is_lvalue_reference<InputContainer>::value
            || !std::is_same<ResultContainer, InputContainer>::value>
        need_new_container;

    template <typename ResultContainer, typename InputContainer, typename Transform>
    ResultContainer transformed(InputContainer &&input, Transform &&transform)
    {
        return transformed<ResultContainer>(std::forward<InputContainer>(input),
                                            std::forward<Transform>(transform),
                                            need_new_container<InputContainer, ResultContainer>);
    }
} // namespace detail

template <typename InputContainer, typename Transform>
auto transformed(InputContainer &&input, Transform &&transform)
#if __cplusplus >= 202002L
    requires std::is_invocable_v<Transform, ValueType<InputContainer>>
#endif
{
    using ResultType = detail::TransformedType<InputContainer, Transform>;
    return detail::transformed<ResultType>(
        std::forward<InputContainer>(input),
        detail::to_function_object(std::forward<Transform>(transform)));
}

template <template <typename...> class ResultContainer, typename InputContainer, typename Transform>
auto transformed(InputContainer &&input, Transform &&transform)
#if __cplusplus >= 202002L
    requires std::is_invocable_v<Transform, ValueType<InputContainer>>
#endif
{
    return detail::transformed<ResultContainer<detail::ResultItemType<InputContainer, Transform>>>(
        std::forward<InputContainer>(input),
        detail::to_function_object(std::forward<Transform>(transform)));
}

template <typename ResultContainer, typename InputContainer, typename Transform>
auto transformed(InputContainer &&input, Transform &&transform)
#if __cplusplus >= 202002L
    requires std::is_invocable_r_v<ValueType<ResultContainer>, Transform, ValueType<InputContainer>>
#endif
{
    return detail::transformed<ResultContainer>(
        std::forward<InputContainer>(input),
        detail::to_function_object(std::forward<Transform>(transform)));
}

template <typename InputContainer, typename Transform>
auto transformed_to_same_container(InputContainer &&input, Transform &&transform)

{
    return detail::transformed<remove_cvref_t<InputContainer>, InputContainer, Transform>(
        std::forward<InputContainer>(input),
        detail::to_function_object(std::forward<Transform>(transform)));
}

template <typename ReturnType, typename InputContainer, typename Transform>
auto transformed_with_new_return_type(InputContainer &&input, Transform &&transform)

{
    return detail::transformed<ReturnType>(std::forward<InputContainer>(input),
                                           std::forward<Transform>(transform));
}

// -------------------- transform --------------------
template <typename Container, typename UnaryPredicate>
void transform(Container &input, UnaryPredicate &&predicate)
#if __cplusplus >= 202002L
    requires UnaryPredicateOnContainerValues<UnaryPredicate, Container>
#endif

{
    std::transform(std::begin(input), std::end(input), std::begin(input),
                   std::forward<UnaryPredicate>(predicate));
}

// -------------------- filtered_transformed --------------------
namespace detail {
    // Version used for l-values or where the container type changes
    template <typename ResultContainer, typename InputContainer, typename Transform,
              typename UnaryPredicate>
    ResultContainer filtered_transformed(InputContainer &&input, Transform &&transform,
                                         UnaryPredicate &&unaryPredicate, std::true_type)
    {
        ResultContainer result;
        detail::reserve(result, input.size());
        auto range = read_iterator_wrapper(std::forward<InputContainer>(input));
        auto inserter = detail::insert_wrapper(result);
        for (const auto &value : range) {
            if (unaryPredicate(value)) {
                *inserter = transform(value);
                ++inserter;
            }
        }
        return result;
    }

    // optimized version to use for r-values where container type is the same
    // in-place editing is possible in that case.
    template <typename ResultContainer, typename InputContainer, typename Transform,
              typename UnaryPredicate>
    ResultContainer filtered_transformed(InputContainer &&input, Transform &&transform,
                                         UnaryPredicate &&unaryPredicate,
                                         std::false_type /* r-value and same containers */)
    {
        auto range = read_iterator_wrapper(std::forward<InputContainer>(input));
        auto writeIterator = std::begin(input);
        for (const auto &value : range) {
            if (unaryPredicate(value)) {
                *writeIterator = transform(value);
                ++writeIterator;
            }
        }
        input.erase(writeIterator, std::end(input));
        return std::forward<InputContainer>(input);
    }

    template <typename ResultContainer, typename InputContainer, typename Transform,
              typename UnaryPredicate>
    ResultContainer filtered_transformed(InputContainer &&input, Transform &&transform,
                                         UnaryPredicate &&unaryPredicate)
    {
        return filtered_transformed<ResultContainer>(
            std::forward<InputContainer>(input), std::forward<Transform>(transform), unaryPredicate,
            need_new_container<InputContainer, ResultContainer>);
    }
}

template <typename InputContainer, typename Transform, typename UnaryPredicate>
#if __cplusplus >= 202002L
    requires UnaryPredicateOnContainerValues<UnaryPredicate, InputContainer>
    && std::is_invocable_v<Transform, ValueType<InputContainer>>
#endif
auto filtered_transformed(InputContainer &&input, Transform &&transform,
                          UnaryPredicate &&unaryPredicate)

{
    using ResultType = detail::TransformedType<InputContainer, Transform>;
    return detail::filtered_transformed<ResultType>(
        std::forward<InputContainer>(input),
        detail::to_function_object(std::forward<Transform>(transform)),
        detail::to_function_object(std::forward<UnaryPredicate>(unaryPredicate)));
}

template <template <typename...> class ResultContainer, typename InputContainer, typename Transform,
          typename UnaryPredicate>
#if __cplusplus >= 202002L
    requires UnaryPredicateOnContainerValues<UnaryPredicate, InputContainer>
    && std::is_invocable_v<Transform, ValueType<InputContainer>>
#endif
auto filtered_transformed(InputContainer &&input, Transform &&transform,
                          UnaryPredicate &&unaryPredicate)
{
    return detail::filtered_transformed<
        ResultContainer<detail::ResultItemType<InputContainer, Transform>>>(
        std::forward<InputContainer>(input),
        detail::to_function_object(std::forward<Transform>(transform)),
        detail::to_function_object(std::forward<UnaryPredicate>(unaryPredicate)));
}

template <typename ResultContainer, typename InputContainer, typename Transform,
          typename UnaryPredicate>
#if __cplusplus >= 202002L
    requires UnaryPredicateOnContainerValues<UnaryPredicate, InputContainer>
    && std::is_invocable_r_v<ValueType<ResultContainer>, Transform, ValueType<InputContainer>>
#endif
auto filtered_transformed(InputContainer &&input, Transform &&transform,
                          UnaryPredicate &&unaryPredicate)
{
    return detail::filtered_transformed<ResultContainer>(
        std::forward<InputContainer>(input),
        detail::to_function_object(std::forward<Transform>(transform)),
        detail::to_function_object(std::forward<UnaryPredicate>(unaryPredicate)));
}

} // namespace kdalgorithms
