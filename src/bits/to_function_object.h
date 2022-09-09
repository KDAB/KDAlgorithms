#pragma once
#include "is_const_method.h"
#include <functional>
#include <type_traits>
#include <utility>

namespace kdalgorithms {
namespace detail {
    // Used for member functions
    // The method must be const, e.g. bool lessThan(...) const
#if __cplusplus >= 202002L
    template <typename F>
    requires is_const_method<F>::value
#else
    template <typename F, typename std::enable_if<is_const_method<F>::value, bool>::type = true>
#endif
        auto
        to_function_object_helper(F &&f, std::true_type)
    {
        return std::mem_fn(std::forward<F>(f));
    }

    // Used for regular function objects
    template <typename F>
    decltype(auto) to_function_object_helper(F &&f, std::false_type)
    {
        return std::forward<F>(f);
    }

    template <typename F>
    auto to_function_object(F &&f)
    {
        return detail::to_function_object_helper(std::forward<F>(f),
                                                 typename std::is_member_pointer<F>::type{});
    }
} // namespace detail
} // namespace kdalgorithms
