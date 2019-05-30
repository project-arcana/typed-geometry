#pragma once

#include "closest_points.hh"
#include "normalize.hh"

namespace tg
{
// Default implementation:
template <class A, class B>
[[nodiscard]] constexpr auto direction(A const& a, B const& b) -> decltype(normalize_safe(closest_points(a, b).second - closest_points(a, b).first))
{
    auto cp = closest_points(a, b);
    return normalize_safe(cp.second - cp.first);
}
} // namespace tg
