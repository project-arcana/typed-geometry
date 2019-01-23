#pragma once

#include "../../types/halfspace.hh"
#include "../../types/hyperplane.hh"
#include "../../types/line.hh"
#include "../../types/ray.hh"
#include "../../types/segment.hh"
#include "../../types/triangle.hh"
#include "normalize.hh"

// Computes the normal of an object

namespace tg
{
template <int D, class ScalarT>
constexpr vec<D, ScalarT> normal(hyperplane<D, ScalarT> const& p)
{
    return p.n;
}

template <int D, class ScalarT>
constexpr vec<D, ScalarT> normal(halfspace<D, ScalarT> const& h)
{
    return h.n;
}

template <class ScalarT>
constexpr vec<2, ScalarT> normal_unnorm(vec<2, ScalarT> const& v)
{
    return vec<2, ScalarT>(-v.y, v.x);
}

template <class ScalarT>
constexpr vec<2, fractional_result<ScalarT>> normal(vec<2, ScalarT> const& v)
{
    return normalize_safe(normal_unnorm(v));
}

template <class ScalarT>
constexpr vec<2, fractional_result<ScalarT>> normal(line<2, ScalarT> const& l)
{
    return normal_unnorm(l.dir);
}

template <class ScalarT>
constexpr vec<2, fractional_result<ScalarT>> normal(ray<2, ScalarT> const& r)
{
    return normal_unnorm(r.dir);
}

template <class ScalarT>
constexpr vec<2, fractional_result<ScalarT>> normal(segment<2, ScalarT> const& s)
{
    return normal(s.b - s.a);
}

template <class ScalarT>
constexpr vec<3, fractional_result<ScalarT>> normal(triangle<3, ScalarT> const& t)
{
    return normalize_safe(cross(t.v1 - t.v0, t.v2 - t.v0));
}
} // namespace tg
