/****************************************************************************
**
** This file is part of KDAlgorithms
**
** SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include <kdalgorithms/kdalgorithms.h>

#include <iostream>
#include <string>
#include <vector>

int main()
{
    auto vec = kdalgorithms::iota(1, 100);
    auto odds = kdalgorithms::filtered(vec, [](int i) { return i % 2 == 1; });
    auto result = kdalgorithms::accumulate(odds, [](const std::string &partial, int value) {
        return partial + "," + std::to_string(value);
    });

    std::cout << result << "\n";

    return 0;
}
