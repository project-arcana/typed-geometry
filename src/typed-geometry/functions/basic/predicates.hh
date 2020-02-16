#pragma once

#include <typed-geometry/types/comp.hh>

namespace tg
{
template <int D>
[[nodiscard]] bool any(comp<D, bool> const& c)
{
    if constexpr (D == 1)
        return c.comp0;
    else if constexpr (D == 2)
        return c.comp0 || c.comp1;
    else if constexpr (D == 3)
        return c.comp0 || c.comp1 || c.comp2;
    else if constexpr (D == 4)
        return c.comp0 || c.comp1 || c.comp2 || c.comp3;
    else
        static_assert(always_false<D>, "only up to 4D supported");
}

template <int D>
[[nodiscard]] bool all(comp<D, bool> const& c)
{
    if constexpr (D == 1)
        return c.comp0;
    else if constexpr (D == 2)
        return c.comp0 && c.comp1;
    else if constexpr (D == 3)
        return c.comp0 && c.comp1 && c.comp2;
    else if constexpr (D == 4)
        return c.comp0 && c.comp1 && c.comp2 && c.comp3;
    else
        static_assert(always_false<D>, "only up to 4D supported");
}
}
