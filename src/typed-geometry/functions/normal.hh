#pragma once

#include <typed-geometry/types/objects/halfspace.hh>
#include <typed-geometry/types/objects/hyperplane.hh>
#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/ray.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include "normalize.hh"
#include "perpendicular.hh"

// Computes the normal at the surface of an object
// Some objects have a fixed normal everywhere, some only at defined positions
// Evaluating the normal at other positions might be undefined or wrong

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr dir<D, ScalarT> normal(hyperplane<D, ScalarT> const& p)
{
    return p.normal;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr dir<D, ScalarT> normal(halfspace<D, ScalarT> const& h)
{
    return h.normal;
}

template <class ScalarT>
TG_NODISCARD constexpr dir<2, ScalarT> normal(line<2, ScalarT> const& l)
{
    return perpendicular(l.dir);
}

template <class ScalarT>
TG_NODISCARD constexpr dir<2, ScalarT> normal(ray<2, ScalarT> const& r)
{
    return perpendicular(r.dir);
}

template <class ScalarT>
TG_NODISCARD constexpr dir<2, fractional_result<ScalarT>> normal(segment<2, ScalarT> const& s)
{
    return normalize(perpendicular(s.pos1 - s.pos0));
}

template <class ScalarT>
TG_NODISCARD constexpr dir<3, fractional_result<ScalarT>> normal(triangle<3, ScalarT> const& t)
{
    return normalize(cross(t.pos1 - t.pos0, t.pos2 - t.pos0));
}

template <class ScalarT>
TG_NODISCARD constexpr dir<3, ScalarT> any_normal(vec<3, ScalarT> const& v)
{
    auto b = vec3::zero;
    if (abs(v.x) > abs(v.y))
        b[1] = ScalarT(1);
    else
        b[0] = ScalarT(1);
    return normalize(cross(v, b));
}

template <class ScalarT>
TG_NODISCARD constexpr dir<3, ScalarT> any_normal(dir<3, ScalarT> const& d)
{
    return any_normal(vec<3, ScalarT>(d));
}
} // namespace tg
