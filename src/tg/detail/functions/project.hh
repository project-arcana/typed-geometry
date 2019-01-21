#pragma once

#include "../../types/line.hh"
#include "../../types/plane.hh"
#include "../../types/pos.hh"
#include "../../types/vec.hh"
#include "../operators/ops_vec.hh"
namespace tg
{
template <int D, class ScalarT>
constexpr vec<D, ScalarT> project(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return b * (dot(a, b) / dot(b, b));
}

template <int D, class ScalarT>
constexpr vec<D, ScalarT> project(vec<D, ScalarT> const& p, line<D, ScalarT> const& l)
{
    return l.p + project(p - l.p, l.dir);
}

template <class ScalarT>
constexpr vec<3, ScalarT> project(pos<3, ScalarT> const& p, plane<ScalarT> const& pl)
{
    return p - pl.n * (dot(p, pl.n) - pl.d);
}
} // namespace tg
