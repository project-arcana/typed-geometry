#pragma once

#include <typed-geometry/types/objects/capsule.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/ray.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/tube.hh>

#include "closest_points.hh"
#include "normalize.hh"

namespace tg
{
// =================== Default implementation ===================

template <class A, class B>
TG_NODISCARD constexpr auto direction(A const& a, B const& b) -> decltype(normalize_safe(closest_points(a, b).second - closest_points(a, b).first))
{
    auto cp = closest_points(a, b);
    return normalize_safe(cp.second - cp.first);
}

// =================== Object properties ===================

template <int D, class ScalarT>
TG_NODISCARD constexpr dir<D, ScalarT> direction(line<D, ScalarT> const& l)
{
    return l.dir;
}
template <int D, class ScalarT>
TG_NODISCARD constexpr dir<D, ScalarT> direction(ray<D, ScalarT> const& r)
{
    return r.dir;
}
template <int D, class ScalarT>
TG_NODISCARD constexpr dir<D, ScalarT> direction(segment<D, ScalarT> const& s)
{
    return normalize(s.pos1 - s.pos0);
}
template <int D, class ScalarT>
TG_NODISCARD constexpr dir<D, ScalarT> direction(tube<D, ScalarT> const& c)
{
    return direction(c.axis);
}
template <int D, class ScalarT>
TG_NODISCARD constexpr dir<D, ScalarT> direction(cylinder<D, ScalarT> const& c)
{
    return direction(c.axis);
}
template <int D, class ScalarT>
TG_NODISCARD constexpr dir<D, ScalarT> direction(capsule<D, ScalarT> const& c)
{
    return direction(c.axis);
}
} // namespace tg
