/****************************************************************************
**
** This file is part of KDAlgorithms
**
** SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "copy_observer.h"
#include <utility>

int CopyObserver::copies = 0;
int CopyObserver::moves = 0;

CopyObserver::CopyObserver(int value)
    : value(value)
{
}

CopyObserver::CopyObserver(const CopyObserver &other)
{
    value = other.value;
    ++copies;
}

CopyObserver::CopyObserver(CopyObserver &&other) noexcept
{
    value = std::exchange(other.value, {});
    ++moves;
}

CopyObserver &CopyObserver::operator=(const CopyObserver &other)
{
    value = other.value;
    ++copies;
    return *this;
}

bool operator<(const CopyObserver &x, const CopyObserver &y)
{
    return x.value < y.value;
}

void CopyObserver::reset()
{
    copies = 0;
    moves = 0;
}

CopyObserver &CopyObserver::operator=(CopyObserver &&other) noexcept
{
    value = std::exchange(other.value, {});
    ++moves;
    return *this;
}

bool operator==(const CopyObserver &x, const CopyObserver &y)
{
    return x.value == y.value;
}
