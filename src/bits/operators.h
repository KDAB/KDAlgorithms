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

namespace kdalgorithms {
namespace operators {
    template <typename L, typename R>
    auto operator&&(L l, R r)
    {
        return [l, r](auto value) { return l(value) && r(value); };
    }

    template <typename L, typename R>
    auto operator||(L l, R r)
    {
        return [l, r](auto value) { return l(value) || r(value); };
    }

    template <typename Expression>
    auto operator!(Expression expression)
    {
        return [expression](auto value) { return !expression(value); };
    }
} // namespace operators
} // namespace kdalgorithms
