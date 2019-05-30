#pragma once

#include <typed-geometry/types/objects/halfspace.hh>
#include <typed-geometry/types/objects/hyperplane.hh>
#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/ray.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include "normalize.hh"

// Computes the normal at the surface of an object
// Some objects have a fixed normal everywhere, some only at defined positions
// Evaluating the normal at other positions might be undefined or wrong

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr dir<D, ScalarT> normal(hyperplane<D, ScalarT> const& p)
{
    return p.normal;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr dir<D, ScalarT> normal(halfspace<D, ScalarT> const& h)
{
    return h.normal;
}

template <class ScalarT>
[[nodiscard]] constexpr vec<2, ScalarT> normal_unnorm(vec<2, ScalarT> const& v)
{
    return vec<2, ScalarT>(-v.y, v.x);
}

template <class ScalarT>
[[nodiscard]] constexpr dir<2, ScalarT> normal(dir<2, ScalarT> const& v)
{
    return dir<2, ScalarT>(-v.y, v.x);
}

template <class ScalarT>
[[nodiscard]] constexpr dir<2, fractional_result<ScalarT>> normal(vec<2, ScalarT> const& v)
{
    return normalize(normal_unnorm(v));
}

template <class ScalarT>
[[nodiscard]] constexpr dir<2, fractional_result<ScalarT>> normal(line<2, ScalarT> const& l)
{
    return normal_unnorm(l.dir);
}

template <class ScalarT>
[[nodiscard]] constexpr dir<2, fractional_result<ScalarT>> normal(ray<2, ScalarT> const& r)
{
    return normal_unnorm(r.dir);
}

template <class ScalarT>
[[nodiscard]] constexpr dir<2, fractional_result<ScalarT>> normal(segment<2, ScalarT> const& s)
{
    return normal(s.pos1 - s.pos0);
}

template <class ScalarT>
[[nodiscard]] constexpr dir<3, fractional_result<ScalarT>> normal(triangle<3, ScalarT> const& t)
{
    return normalize(cross(t.pos1 - t.pos0, t.pos2 - t.pos0));
}

template <class ScalarT>
[[nodiscard]] constexpr dir<3, ScalarT> any_normal(vec<3, ScalarT> const& v)
{
    auto b = vec3::zero;
    if (abs(v.x) > abs(v.y))
        b[1] = 1.0f;
    else
        b[0] = 1.0f;
    return normalize(cross(v, b));
}

template <class ScalarT>
[[nodiscard]] constexpr dir<3, ScalarT> any_normal(dir<3, ScalarT> const& d)
{
    return any_normal(vec<3, ScalarT>(d));
}
} // namespace tg
