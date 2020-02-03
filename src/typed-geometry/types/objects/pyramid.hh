#pragma once

#include "../dir.hh"
#include "../pos.hh"

#include "box.hh"
#include "quad.hh"
#include "sphere.hh"
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
template <class BaseT>
struct pyramid;

// Common pyramid types
using box_pyramid3 = pyramid<box2in3>;
using sphere_pyramid3 = pyramid<sphere2in3>;
using triangle_pyramid3 = pyramid<triangle3>;
using quad_pyramid3 = pyramid<quad3>;

// ======== IMPLEMENTATION ========
template <class BaseT>
struct pyramid
{
    using scalar_t = typename BaseT::scalar_t;
    using base_t = BaseT;

    base_t base;
    scalar_t height = 0;

    constexpr pyramid() = default;
    constexpr pyramid(base_t const& base, scalar_t height) : base(base), height(height) {}

    [[nodiscard]] bool operator==(pyramid const& rhs) const { return base == rhs.base && height == rhs.height; }
    [[nodiscard]] bool operator!=(pyramid const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
