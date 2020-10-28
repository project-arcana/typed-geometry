#pragma once

#include "../dir.hh"
#include "../pos.hh"
#include "box.hh"
#include "quad.hh"
#include "sphere.hh"
#include "traits.hh"
#include "triangle.hh"

namespace tg
{
/**
 * 3D pyramid
 *
 * supported base types:
 * - box2in3
 * - sphere2in3
 * - triangle
 * - quad (must be planar)
 */
template <class BaseT, class TraitsT = default_object_tag>
struct pyramid;

// Common pyramid types
using box_pyramid3 = pyramid<box2in3>;
using sphere_pyramid3 = pyramid<sphere2in3>;
using triangle_pyramid3 = pyramid<triangle3>;
using quad_pyramid3 = pyramid<quad3>;

template <class BaseT>
using pyramid_boundary = pyramid<BaseT, boundary_tag>;
template <class BaseT>
using pyramid_boundary_no_caps = pyramid<BaseT, boundary_no_caps_tag>;

// ======== IMPLEMENTATION ========
template <class BaseT, class TraitsT>
struct pyramid
{
    using scalar_t = typename BaseT::scalar_t;
    using base_t = BaseT;

    base_t base;
    scalar_t height = 0;

    constexpr pyramid() = default;
    constexpr pyramid(base_t const& base, scalar_t height) : base(base), height(height) {}

    template <class OtherT, class OtherTraitsT>
    explicit constexpr pyramid(pyramid<OtherT, OtherTraitsT> const& v) : base(v.base), height(v.height)
    {
    }

    [[nodiscard]] bool operator==(pyramid const& rhs) const { return base == rhs.base && height == rhs.height; }
    [[nodiscard]] bool operator!=(pyramid const& rhs) const { return !operator==(rhs); }
};

template <class I, class BaseT, class TraitsT>
constexpr void introspect(I&& i, pyramid<BaseT, TraitsT>& v)
{
    i(v.base, "base");
    i(v.height, "height");
}

template <class BaseT, class TraitsT>
struct object_traits<pyramid<BaseT, TraitsT>>
  : detail::finite_object_traits<object_traits<BaseT>::object_dimension + 1, typename object_traits<BaseT>::scalar_t, object_traits<BaseT>::domain_dimension, TraitsT>
{
};
} // namespace tg
