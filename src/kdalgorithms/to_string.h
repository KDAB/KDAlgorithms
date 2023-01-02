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

namespace kdalgorithms {

using std::to_string;

std::string to_string(bool b)
{
    return b ? "true" : "false";
}

std::string to_string(const std::string &s)
{
    return "\"" + s + "\"";
}

std::string to_string(const char *s)
{
    return "\"" + std::string(s) + "\"";
}
template <typename Container,
          typename enabled = decltype(std::declval<Container>().push_back(
              std::declval<typename Container::value_type>()))>
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

#if 0 // IVAN1
template <typename Map,
          typename enabled =
              decltype(std::declval<Map>().insert(std::declval<typename Map::value_type>()))>
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
#endif

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
