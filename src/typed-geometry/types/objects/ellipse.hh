#pragma once

#include <typed-geometry/types/scalars/default.hh>
#include "../mat.hh"
#include "../pos.hh"
#include "traits.hh"

#include "sphere.hh"

// An ellipse / ellipsoid
// stored as:
//   center position
//   orthogonal matrix containing the semi-axes

namespace tg
{
template <int ObjectD, class ScalarT, int DomainD = ObjectD, class TraitsT = default_object_tag>
struct ellipse;

// Common ellipse types

using ellipse1 = ellipse<1, f32>;
using ellipse2 = ellipse<2, f32>;
using ellipse3 = ellipse<3, f32>;
using ellipse4 = ellipse<4, f32>;

using fellipse1 = ellipse<1, f32>;
using fellipse2 = ellipse<2, f32>;
using fellipse3 = ellipse<3, f32>;
using fellipse4 = ellipse<4, f32>;

using dellipse1 = ellipse<1, f64>;
using dellipse2 = ellipse<2, f64>;
using dellipse3 = ellipse<3, f64>;
using dellipse4 = ellipse<4, f64>;

using iellipse1 = ellipse<1, i32>;
using iellipse2 = ellipse<2, i32>;
using iellipse3 = ellipse<3, i32>;
using iellipse4 = ellipse<4, i32>;

using uellipse1 = ellipse<1, u32>;
using uellipse2 = ellipse<2, u32>;
using uellipse3 = ellipse<3, u32>;
using uellipse4 = ellipse<4, u32>;

using ellipse2in3 = ellipse<2, f32, 3>;
using fellipse2in3 = ellipse<2, f32, 3>;
using dellipse2in3 = ellipse<2, f64, 3>;
using iellipse2in3 = ellipse<2, i32, 3>;
using uellipse2in3 = ellipse<2, u32, 3>;

template <int ObjectD, class ScalarT, int DomainD = ObjectD>
using ellipse_boundary = ellipse<ObjectD, ScalarT, DomainD, boundary_tag>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT, class TraitsT>
struct ellipse<D, ScalarT, D, TraitsT>
{
    using scalar_t = ScalarT;
    using pos_t = pos<D, ScalarT>;
    using mat_t = mat<D, D, ScalarT>;

    pos_t center;
    mat_t semi_axes;

    constexpr ellipse() = default;
    constexpr ellipse(pos_t center, mat_t const& semi_axes) : center(center), semi_axes(semi_axes) {}
    constexpr ellipse(sphere<D, ScalarT, D, TraitsT> const& b); // requires tg.hh

    template <class OtherT, class OtherTraitsT>
    explicit constexpr ellipse(ellipse<D, OtherT, D, OtherTraitsT> const& v) : center(v.center), semi_axes(v.semi_axes)
    {
    }

    [[nodiscard]] bool operator==(ellipse const& rhs) const { return center == rhs.center && semi_axes == rhs.semi_axes; }
    [[nodiscard]] bool operator!=(ellipse const& rhs) const { return !operator==(rhs); }
};

template <class ScalarT, class TraitsT>
struct ellipse<2, ScalarT, 3, TraitsT>
{
    using scalar_t = ScalarT;
    using pos_t = pos<3, ScalarT>;
    using mat_t = mat<2, 3, ScalarT>;

    pos_t center;
    mat_t semi_axes;

    constexpr ellipse() = default;
    constexpr ellipse(pos_t center, mat_t const& semi_axes) : center(center), semi_axes(semi_axes) {}
    constexpr ellipse(sphere<2, ScalarT, 3, TraitsT> const& b); // requires tg.hh

    template <class OtherT, class OtherTraitsT>
    explicit constexpr ellipse(ellipse<2, OtherT, 3, OtherTraitsT> const& v) : center(v.center), semi_axes(v.semi_axes)
    {
    }

    [[nodiscard]] bool operator==(ellipse const& rhs) const { return center == rhs.center && semi_axes == rhs.semi_axes; }
    [[nodiscard]] bool operator!=(ellipse const& rhs) const { return !operator==(rhs); }
};

template <class I, int ObjectD, class ScalarT, int DomainD, class TraitsT>
constexpr void introspect(I&& i, ellipse<ObjectD, ScalarT, DomainD, TraitsT>& v)
{
    i(v.center, "center");
    i(v.semi_axes, "semi_axes");
}

template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
struct object_traits<ellipse<ObjectD, ScalarT, DomainD, TraitsT>> : detail::finite_object_traits<ObjectD, ScalarT, DomainD, TraitsT>
{
};
} // namespace tg
