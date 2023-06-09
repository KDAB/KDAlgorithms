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

namespace kdalgorithms {
namespace detail {
    template <typename method_t>
    struct is_const_method;

    template <typename Class, typename ReturnType, typename... ArgType>
    struct is_const_method<ReturnType (Class::*)(ArgType...)>
    {
        static constexpr bool value = false;
    };

    template <typename Class, typename ReturnType, typename... ArgType>
    struct is_const_method<ReturnType (Class::*)(ArgType...) const>
    {
        static constexpr bool value = true;
    };
} // namespace detail
} // namespace kdalgorithms
