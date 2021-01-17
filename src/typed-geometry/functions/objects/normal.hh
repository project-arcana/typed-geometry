#pragma once

#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/ellipse.hh>
#include <typed-geometry/types/objects/halfspace.hh>
#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/plane.hh>
#include <typed-geometry/types/objects/quad.hh>
#include <typed-geometry/types/objects/ray.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include <typed-geometry/functions/vector/normalize.hh>
#include <typed-geometry/functions/vector/perpendicular.hh>

#include "typed-geometry/functions/tests/vec_tests.hh"

// Computes the normal at the surface of an object
// Some objects have a fixed normal everywhere, some only at defined positions
// Evaluating the normal at other positions might be undefined or wrong

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr dir<D, ScalarT> normal_of(plane<D, ScalarT> const& p)
{
    return p.normal;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr dir<D, ScalarT> normal_of(halfspace<D, ScalarT> const& h)
{
    return h.normal;
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr dir<3, ScalarT> normal_of(sphere<2, ScalarT, 3, TraitsT> const& d)
{
    return d.normal;
}

template <class ScalarT>
[[nodiscard]] constexpr dir<2, ScalarT> normal_of(line<2, ScalarT> const& l)
{
    return perpendicular(l.dir);
}

template <class ScalarT>
[[nodiscard]] constexpr dir<2, ScalarT> normal_of(ray<2, ScalarT> const& r)
{
    return perpendicular(r.dir);
}

template <class ScalarT>
[[nodiscard]] constexpr dir<2, fractional_result<ScalarT>> normal_of(segment<2, ScalarT> const& s)
{
    return normalize(perpendicular(s.pos1 - s.pos0));
}

template <class ScalarT>
[[nodiscard]] constexpr dir<3, fractional_result<ScalarT>> normal_of(triangle<3, ScalarT> const& t)
{
    return normalize(cross(t.pos1 - t.pos0, t.pos2 - t.pos0));
}

template <class ScalarT>
[[nodiscard]] constexpr dir<3, fractional_result<ScalarT>> normal_of(quad<3, ScalarT> const& q)
{
    // Assumes the quad is planar, as it is a requirement for pyramid<quad>
    const auto res = normalize(cross(q.pos01 - q.pos00, q.pos10 - q.pos00));
    TG_ASSERT(tg::are_orthogonal(q.pos11 - q.pos00, vec(res))); // Checks that the four points are indeed coplanar
    return res;
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr dir<3, fractional_result<ScalarT>> normal_of(box<2, ScalarT, 3, TraitsT> const& b)
{
    return normalize(cross(b.half_extents[0], b.half_extents[1]));
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr dir<3, fractional_result<ScalarT>> normal_of(ellipse<2, ScalarT, 3, TraitsT> const& e)
{
    return normalize(cross(e.semi_axes[0], e.semi_axes[1]));
}

template <class ScalarT>
[[nodiscard]] constexpr dir<3, ScalarT> any_normal(vec<3, ScalarT> const& v)
{
    auto b = vec<3, ScalarT>::zero;
    if (abs(v.x) > abs(v.y))
        b[1] = ScalarT(1);
    else
        b[0] = ScalarT(1);
    return normalize(cross(v, b));
}

template <class ScalarT>
[[nodiscard]] constexpr dir<3, ScalarT> any_normal(dir<3, ScalarT> const& d)
{
    return any_normal(vec<3, ScalarT>(d));
}


template <class ObjectT>
[[deprecated("use normal_of")]] [[nodiscard]] constexpr auto normal(ObjectT const& o) -> decltype(normal_of(o))
{
    return normal_of(o);
}
} // namespace tg
