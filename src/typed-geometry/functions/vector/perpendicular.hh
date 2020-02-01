#pragma once

#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/ray.hh>

namespace tg
{
template <class ScalarT>
[[nodiscard]] constexpr vec<2, ScalarT> perpendicular(vec<2, ScalarT> const& v)
{
    return vec<2, ScalarT>(-v.y, v.x);
}

template <class ScalarT>
[[nodiscard]] constexpr dir<2, ScalarT> perpendicular(dir<2, ScalarT> const& v)
{
    return dir<2, ScalarT>(-v.y, v.x);
}

template <class ScalarT>
[[nodiscard]] constexpr line<2, ScalarT> perpendicular(line<2, ScalarT> const& l)
{
    return {l.pos, perpendicular(l.dir)};
}

template <class ScalarT>
[[nodiscard]] constexpr ray<2, ScalarT> perpendicular(ray<2, ScalarT> const& r)
{
    return {r.origin, perpendicular(r.dir)};
}
}
