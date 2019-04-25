#pragma once

#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/detail/special_values.hh>
#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/plane.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

#include "coordinates.hh"
#include "normal.hh"

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, ScalarT> project(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return b * dot(a, b) / dot(b, b);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, ScalarT> project(vec<D, ScalarT> const& a, dir<D, ScalarT> const& b)
{
    return b * dot(a, b);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, line<D, ScalarT> const& l)
{
    return l.pos + project(p - l.pos, l.dir);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, segment<D, ScalarT> const& s)
{
    auto t = coordinates(s, p);
    t = clamp(t, ScalarT(0), ScalarT(1));
    return mix(s.pos0, s.pos1, t);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, hyperplane<D, ScalarT> const& pl)
{
    return p - pl.normal * (dot(p - zero<pos<D, ScalarT>>(), pl.normal) - pl.dis);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, ScalarT> project(vec<D, ScalarT> const& v, hyperplane<D, ScalarT> const& pl)
{
    return v - pl.normal * dot(v, pl.normal);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, ScalarT> project(dir<D, ScalarT> const& v, hyperplane<D, ScalarT> const& pl)
{
    return v - pl.normal * dot(v, pl.normal);
}
} // namespace tg
