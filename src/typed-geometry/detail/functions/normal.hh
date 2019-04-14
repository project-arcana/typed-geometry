#pragma once

#include "../../types/objects/halfspace.hh"
#include "../../types/objects/hyperplane.hh"
#include "../../types/objects/line.hh"
#include "../../types/objects/ray.hh"
#include "../../types/objects/segment.hh"
#include "../../types/objects/triangle.hh"
#include "../../types/objects/sphere.hh"
#include "normalize.hh"

// Computes the normal at the surface of an object
// Some objects have a fixed normal everywhere, some only at defined positions
// Evaluating the normal at other positions might be undefined or wrong

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, ScalarT> normal(hyperplane<D, ScalarT> const& p)
{
    return p.n;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, ScalarT> normal(halfspace<D, ScalarT> const& h)
{
    return h.n;
}

template <class ScalarT>
TG_NODISCARD constexpr vec<2, ScalarT> normal_unnorm(vec<2, ScalarT> const& v)
{
    return vec<2, ScalarT>(-v.y, v.x);
}

template <class ScalarT>
TG_NODISCARD constexpr vec<2, fractional_result<ScalarT>> normal(vec<2, ScalarT> const& v)
{
    return normalize_safe(normal_unnorm(v));
}

template <class ScalarT>
TG_NODISCARD constexpr vec<2, fractional_result<ScalarT>> normal(line<2, ScalarT> const& l)
{
    return normal_unnorm(l.dir);
}

template <class ScalarT>
TG_NODISCARD constexpr vec<2, fractional_result<ScalarT>> normal(ray<2, ScalarT> const& r)
{
    return normal_unnorm(r.dir);
}

template <class ScalarT>
TG_NODISCARD constexpr vec<2, fractional_result<ScalarT>> normal(segment<2, ScalarT> const& s)
{
    return normal(s.b - s.a);
}

template <class ScalarT>
TG_NODISCARD constexpr vec<3, fractional_result<ScalarT>> normal(triangle<3, ScalarT> const& t)
{
    return normalize_safe(cross(t.v1 - t.v0, t.v2 - t.v0));
}
} // namespace tg
