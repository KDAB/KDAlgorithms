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
#include <vector>

template <typename T>
struct ContainerObserver : public std::vector<int>
{
    ContainerObserver() { ++copies; }
    static void reset() { copies = 0; }
    static int copies;
};

template <typename T>
int ContainerObserver<T>::copies = 0;
