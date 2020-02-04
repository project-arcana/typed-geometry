#pragma once

#include <typed-geometry/feature/assert.hh>

#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/size.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/cone.hh>
#include <typed-geometry/types/objects/halfspace.hh>
#include <typed-geometry/types/objects/inf_cone.hh>
#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/plane.hh>

#include <typed-geometry/detail/operators/ops_pos.hh>
#include <typed-geometry/functions/vector/dot.hh>

// Header for all constructors that depend on functions

namespace tg
{
template <int D, class ScalarT, class TraitsT>
constexpr box<D, ScalarT, D, TraitsT>::box(aabb<D, ScalarT, TraitsT> const& b)
{
    static_assert(is_floating_point<ScalarT>, "cannot be guaranteed for integers");
    auto half_e = (b.max - b.min) / ScalarT(2);
    center = b.min + half_e;
    for (auto i = 0; i < D; ++i)
    {
        auto v = vec_t();
        v[i] = ScalarT(1);
        half_extents[i] = v * half_e[i];
    }
}

template <int D, class ScalarT>
constexpr plane<D, ScalarT>::plane(dir_t n, pos_t p) : normal(n), dis(dot(vec<D, ScalarT>(p), n))
{
}
template <int D, class ScalarT>
constexpr halfspace<D, ScalarT>::halfspace(dir_t n, pos_t p) : normal(n), dis(dot(vec<D, ScalarT>(p), n))
{
}

template <int D, class ScalarT>
constexpr line<D, ScalarT> line<D, ScalarT>::from_points(pos_t a, pos_t b)
{
    return line(a, normalize(b - a));
}

template <int D, class ScalarT, class TraitsT>
constexpr inf_cone<D, ScalarT, TraitsT>::inf_cone(cone<D, ScalarT, TraitsT> const& c)
{
    apex = c.base.center + c.height * c.base.normal;
    opening_dir = -c.base.normal;
    opening_angle = ScalarT(2) * angle_between(normalize(c.base.center + any_normal(c.base.normal) * c.base.radius - apex), opening_dir);
}
}
