#pragma once

namespace KDAlgorithms {
namespace Operators {
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
} // namespace Compare
} // namespace KDAlgorithms
