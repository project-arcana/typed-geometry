#pragma once

#include "../../types/objects/line.hh"
#include "../../types/objects/plane.hh"
#include "../../types/objects/segment.hh"
#include "../../types/pos.hh"
#include "../../types/vec.hh"
#include "../operators/ops_vec.hh"
#include "../special_values.hh"
#include "coordinates.hh"

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, ScalarT> project(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return b * (dot(a, b) / dot(b, b));
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, line<D, ScalarT> const& l)
{
    return l.p + project(p - l.p, l.dir);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, segment<D, ScalarT> const& s)
{
    auto t = coordinates(s, p);
    t = clamp(t, ScalarT(0), ScalarT(1));
    return mix(s.a, s.b, t);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, hyperplane<D, ScalarT> const& pl)
{
    return p - pl.n * (dot(p - zero<pos<D, ScalarT>>(), pl.n) - pl.d);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, ScalarT> project(vec<D, ScalarT> const& v, hyperplane<D, ScalarT> const& pl)
{
    return v - pl.n * dot(v, pl.n);
}
} // namespace tg
