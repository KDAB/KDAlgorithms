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

struct CopyObserver
{
    CopyObserver(int value);

    CopyObserver(const CopyObserver &other);
    CopyObserver &operator=(const CopyObserver &other);

    CopyObserver(CopyObserver &&other) noexcept;
    CopyObserver &operator=(CopyObserver &&other) noexcept;

    friend bool operator<(const CopyObserver &x, const CopyObserver &y);
    friend bool operator==(const CopyObserver &x, const CopyObserver &y);
    static void reset();

    int value;
    static int copies;
    static int moves;
};
