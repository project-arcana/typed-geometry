#pragma once

#include <typed-geometry/feature/assert.hh>

#include <typed-geometry/types/scalars/default.hh>
#include "../pos.hh"
#include "../vec.hh"
#include "aabb.hh"
#include "plane.hh"

// A representation of a view frustum without far plane
// NOTE: the defaults are for reverse_z with z=1 near and z=0 far
// NOTE: in contrast to the normal frustum, vertex storage is not as useful
//      (there are no far vertices, and the near vertices are usually really close and thus almost useless)

namespace tg
{
template <int D, class ScalarT, class TraitsT = default_object_tag>
struct inf_frustum;

// Common inf_frustum types

using inf_frustum3 = inf_frustum<3, f32>;
using finf_frustum3 = inf_frustum<3, f32>;
using dinf_frustum3 = inf_frustum<3, f64>;

// ======== IMPLEMENTATION ========

template <class ScalarT, class TraitsT>
struct inf_frustum<3, ScalarT, TraitsT>
{
    enum
    {
        plane_idx_neg_x = 0,
        plane_idx_neg_y,
        plane_idx_pos_x,
        plane_idx_pos_y,
        plane_idx_pos_z, // near plane
        plane_count
    };

    /// inf_frustum represented by 5 planes
    /// NOTE: planes point outwards, i.e. positive signed distance is OUTSIDE
    array<plane<3, ScalarT>, plane_count> planes;

    /// extract inf_frustum from a view-projection-matrix (proj * view)
    /// uses reverse_z conventions (i.e. NDC z from 1..0 near..far)
    /// does not assume handedness of matrix
    /// needs functions/objects/frustum.hh
    ///    or feature/objects.hh
    ///    or tg.hh
    constexpr static inf_frustum from_view_proj_reverse_z(mat<4, 4, ScalarT> const& m);

    constexpr bool operator==(inf_frustum const& rhs) const { return planes == rhs.planes; }
    constexpr bool operator!=(inf_frustum const& rhs) const { return planes != rhs.planes; }
};

template <class I, int D, class ScalarT, class TraitsT>
constexpr void introspect(I&& i, inf_frustum<D, ScalarT, TraitsT>& v)
{
    i(v.planes, "planes");
}

} // namespace tg
