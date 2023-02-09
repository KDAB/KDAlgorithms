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

#include "method_tests.h"
#include <cstddef>
#include <type_traits>

namespace kdalgorithms {
namespace detail {
    template <typename Container>
    bool reserve_helper(Container & /*container*/, typename Container::size_type /*size*/,
                        std::false_type)
    {
        return false;
    }

    template <typename Container>
    bool reserve_helper(Container &container, typename Container::size_type size, std::true_type)
    {
        container.reserve(size);
        return true;
    }

    template <typename Container>
    // call reserve on the container if it supports the operation
    // return if operation was supported.
    bool reserve(Container &container, typename Container::size_type size)
    {
        return reserve_helper(container, size,
                              std::integral_constant<bool, has_reserve_method_v<Container>>());
    }
}
}
