#pragma once

#include <typed-geometry/feature/assert.hh>

#include <typed-geometry/types/scalars/default.hh>
#include "../pos.hh"
#include "../vec.hh"
#include "plane.hh"

namespace tg
{
template <class ScalarT>
struct frustum;

// Common frustum types

using ffrustum = frustum<f32>;

using dfrustum = frustum<f64>;

// ======== IMPLEMENTATION ========

template <class ScalarT>
struct frustum
{
    // frustum represented by 6 planes
    array<tg::plane<3, ScalarT>, 6> planes;

    constexpr frustum() = default;
    explicit constexpr frustum(array<tg::plane<3, ScalarT>, 6> const& planes) : planes(planes)
    { /* TG_CONTRACT(..); */
    }

    // extract frustum from a view-projection-matrix (proj * view)
    constexpr frustum(mat<4, 4, ScalarT> const& m); // requires tg.hh

    [[nodiscard]] bool operator==(frustum const& rhs) const
    {
        for (auto i = 0u; i < planes.size(); ++i)
        {
            if (planes[i] != rhs.planes[i])
                return false;
        }
        return true;
    }
    [[nodiscard]] bool operator!=(frustum const& rhs) const { return !operator==(rhs); }
};

template <class I, int D, class ScalarT, class TraitsT>
constexpr void introspect(I&& i, frustum<ScalarT>& v)
{
    i(v.planes, "planes");
}

} // namespace tg
