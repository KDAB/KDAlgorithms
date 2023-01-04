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

#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../bits/is_detected.h"
#include "../bits/method_tests.h"

namespace kdalgorithms {

using std::to_string;

inline std::string to_string(bool b)
{
    return b ? "true" : "false";
}

inline std::string to_string(const std::string &s)
{
    return "\"" + s + "\"";
}

inline std::string to_string(const char *s)
{
    return "\"" + std::string(s) + "\"";
}

template <typename Container,
         std::enable_if_t<detail::tests::has_value_type<Container> and not detail::tests::has_key_and_mapped_type<Container>, int> = 0>
std::string to_string(const Container &container)
{
    std::string result = "[";
    bool first = true;
    for (auto &&item : container) {
        if (!first)
            result += ", ";
        first = false;
        result += to_string(item);
    }
    return result + "]";
}

template <typename Map,
         std::enable_if_t<detail::tests::has_value_type<Map> and detail::tests::has_key_and_mapped_type<Map>, int> = 0>
std::string to_string(const Map &map)
{
    std::string result = "{";
    bool first = true;
    for (const auto &pair : map) {
        if (!first)
            result += ", ";
        first = false;
        result += "{" + to_string(pair.first) + " -> " + to_string(pair.second) + "}";
    }
    return result + "}";
}

template <typename T1, typename T2>
std::string to_string(const std::pair<T1, T2> &pair)
{
    return "(" + to_string(pair.first) + ", " + to_string(pair.second) + ")";
}

namespace detail {
    template <typename... Types, std::size_t... Indices>
    std::string to_string(const std::tuple<Types...> &tuple, std::index_sequence<Indices...>)
    {
        std::vector<std::string> resultVec{kdalgorithms::to_string(std::get<Indices>(tuple))...};
        std::string result = "(";
        if (!resultVec.empty())
            result += resultVec.at(0);
        for (std::size_t i = 1; i < resultVec.size(); ++i)
            result += ", " + resultVec.at(i);
        return result + ")";
    }
}

template <typename... Types>
std::string to_string(const std::tuple<Types...> &tuple)
{
    return detail::to_string(tuple, std::index_sequence_for<Types...>{});
}

}
