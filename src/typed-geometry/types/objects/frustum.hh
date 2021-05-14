#pragma once

#include <typed-geometry/feature/assert.hh>

#include <typed-geometry/types/scalars/default.hh>
#include "../pos.hh"
#include "../vec.hh"
#include "aabb.hh"
#include "plane.hh"

// A representation of a view frustum
// NOTE: the storage is not minimal. halfspaces AND endpoints are stored

namespace tg
{
template <int D, class ScalarT, class TraitsT = default_object_tag>
struct frustum;

// Common frustum types

using frustum3 = frustum<3, f32>;
using ffrustum3 = frustum<3, f32>;
using dfrustum3 = frustum<3, f64>;

// ======== IMPLEMENTATION ========

template <class ScalarT, class TraitsT>
struct frustum<3, ScalarT, TraitsT>
{
    enum
    {
        plane_idx_neg_x = 0,
        plane_idx_neg_y,
        plane_idx_neg_z,
        plane_idx_pos_x,
        plane_idx_pos_y,
        plane_idx_pos_z,
        plane_count
    };

    enum
    {
        vertex_idx_000 = 0,
        vertex_idx_001,
        vertex_idx_010,
        vertex_idx_011,
        vertex_idx_100,
        vertex_idx_101,
        vertex_idx_110,
        vertex_idx_111,
        vertex_count,
    };

    /// frustum represented by 6 planes
    /// NOTE: planes point outwards, i.e. positive signed distance is OUTSIDE
    array<plane<3, ScalarT>, plane_count> planes;

    /// frustum represented by 8 corner vertices
    array<pos<3, ScalarT>, vertex_count> vertices;

    /// extract frustum from a view-projection-matrix (proj * view)
    /// uses -1..1 NDC coordinates by default (but is configurable)
    /// does not assume handedness of matrix
    /// needs functions/objects/frustum.hh
    ///    or feature/objects.hh
    ///    or tg.hh
    constexpr static frustum from_view_proj(mat<4, 4, ScalarT> const& m, aabb<3, ScalarT> const& ndc = aabb<3, ScalarT>(ScalarT(-1), ScalarT(1)));

    constexpr bool operator==(frustum const& rhs) const { return planes == rhs.planes; }
    constexpr bool operator!=(frustum const& rhs) const { return planes != rhs.planes; }
};

template <class I, int D, class ScalarT, class TraitsT>
constexpr void introspect(I&& i, frustum<D, ScalarT, TraitsT>& v)
{
    i(v.planes, "planes");
}

} // namespace tg
