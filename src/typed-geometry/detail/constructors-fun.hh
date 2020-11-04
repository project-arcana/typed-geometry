#pragma once

#include <typed-geometry/feature/assert.hh>

#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/size.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/ellipse.hh>
#include <typed-geometry/types/objects/halfspace.hh>
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

template <int D, class ScalarT, class TraitsT>
constexpr ellipse<D, ScalarT, D, TraitsT>::ellipse(sphere<D, ScalarT, D, TraitsT> const& s)
{
    center = s.center;
    semi_axes = mat_t::diag(s.radius);
}
template <class ScalarT, class TraitsT>
constexpr ellipse<2, ScalarT, 3, TraitsT>::ellipse(sphere<2, ScalarT, 3, TraitsT> const& s)
{
    center = s.center;
    const auto axis1 = any_normal(s.normal);
    const auto axis2 = normalize(cross(s.normal, axis1));
    semi_axes = mat_t::from_cols(s.radius * axis1, s.radius * axis2);
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
}
