/****************************************************************************
**
** This file is part of KDAlgorithms
**
** SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#pragma once
#include <functional>

namespace kdalgorithms {
namespace detail {
    template <typename Function, typename Object, typename DontCare1, typename DontCare2,
              typename... Args>
    decltype(auto) invoke_helper(Function function, Object &&object,
                                 std::false_type /* is member pointer*/, DontCare1, DontCare2,
                                 Args &&...args)
    {
        return function(object, std::forward<Args>(args)...);
    }

    template <typename Member, typename Object, typename... Args>
    decltype(auto) invoke_helper(Member member, Object &&object,
                                 std::true_type /* is member pointer */,
                                 std::true_type /* is member function */,
                                 std::true_type /* is object a pointer */, Args &&...args)
    {
        return (std::forward<Object>(object)->*member)(std::forward<Args>(args)...);
    }

    template <typename Member, typename Object>
    decltype(auto)
    invoke_helper(Member member, Object &&object, std::true_type /* is member pointer */,
                  std::false_type /* is member function */, std::true_type /* is object a pointer*/)
    {
        return std::forward<Object>(object)->*member;
    }

    template <typename Member, typename Object, typename... Args>
    decltype(auto) invoke_helper(Member member, Object &&object,
                                 std::true_type /* is member pointer */,
                                 std::true_type /* is member function */,
                                 std::false_type /* is object a pointer */, Args &&...args)
    {
        return (std::forward<Object>(object).*member)(std::forward<Args>(args)...);
    }

    template <typename Member, typename Object>
    decltype(auto) invoke_helper(Member member, Object &&object,
                                 std::true_type /* is member pointer */,
                                 std::false_type /* is member function */,
                                 std::false_type /* is object a pointer */)
    {
        return std::forward<Object>(object).*member;
    }

    template <typename Function, typename Object, typename... Args>
    decltype(auto) invoke(Function function, Object &&object, Args &&...args)
    {
        using Type = std::remove_reference_t<Object>;
        return invoke_helper(function, std::forward<Object>(object),
                             std::is_member_pointer<Function>(),
                             std::is_member_function_pointer<Function>(), std::is_pointer<Type>(),
                             std::forward<Args>(args)...);
    }

} // namespace detail
} // kdalgorithms
