#pragma once

#include "closest_points.hh"
#include "normalize.hh"

namespace tg
{
// Default implementation:
template <class A, class B>
TG_NODISCARD constexpr auto direction(A const& a, B const& b) -> decltype(normalize_safe(closest_points(a, b).first, closest_points(a, b).second))
{
    auto cp = closest_points(a, b);
    return normalize_safe(cp.first, cp.second);
}
} // namespace tg
