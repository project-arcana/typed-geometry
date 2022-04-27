#pragma once

#include <typed-geometry/feature/matrix.hh>
#include <typed-geometry/feature/vector.hh>
#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/frustum.hh>
#include <typed-geometry/types/objects/inf_frustum.hh>

namespace tg
{
template <class ScalarT, class TraitsT>
constexpr frustum<3, ScalarT, TraitsT> frustum<3, ScalarT, TraitsT>::from_view_proj(mat<4, 4, ScalarT> const& m, aabb<3, ScalarT> const& ndc)
{
    // TODO: see if insight from http://www8.cs.umu.se/kurser/5DV051/HT12/lab/plane_extraction.pdf can be used

    CC_ASSERT(tg::determinant(m) != 0 && "cannot create frustum from singular matrices");
    CC_ASSERT((vec<3, ScalarT>(m.row(2)) != vec<3, ScalarT>::zero) && "cannot create frustum from reverse_z matrix. did you mean to use inf_frustum?");

    using frustum_t = frustum<3, ScalarT, TraitsT>;
    using comp_t = tg::comp<3, ScalarT>;
    using plane_t = tg::plane<3, ScalarT>;

    auto const invM = inverse(m);

    frustum_t f;

    f.vertices[frustum_t::vertex_idx_000] = invM * ndc[comp_t(0, 0, 0)];
    f.vertices[frustum_t::vertex_idx_001] = invM * ndc[comp_t(0, 0, 1)];
    f.vertices[frustum_t::vertex_idx_010] = invM * ndc[comp_t(0, 1, 0)];
    f.vertices[frustum_t::vertex_idx_011] = invM * ndc[comp_t(0, 1, 1)];
    f.vertices[frustum_t::vertex_idx_100] = invM * ndc[comp_t(1, 0, 0)];
    f.vertices[frustum_t::vertex_idx_101] = invM * ndc[comp_t(1, 0, 1)];
    f.vertices[frustum_t::vertex_idx_110] = invM * ndc[comp_t(1, 1, 0)];
    f.vertices[frustum_t::vertex_idx_111] = invM * ndc[comp_t(1, 1, 1)];

    auto const build_plane = [&](int i0, int i1, int i2)
    {
        auto const p0 = f.vertices[i0];
        auto const p1 = f.vertices[i1];
        auto const p2 = f.vertices[i2];
        return plane_t(normalize(cross(p1 - p0, p2 - p0)), p0);
    };

    f.planes[frustum_t::plane_idx_neg_x] = build_plane(frustum_t::vertex_idx_000, frustum_t::vertex_idx_010, frustum_t::vertex_idx_001);
    f.planes[frustum_t::plane_idx_neg_y] = build_plane(frustum_t::vertex_idx_000, frustum_t::vertex_idx_001, frustum_t::vertex_idx_100);
    f.planes[frustum_t::plane_idx_neg_z] = build_plane(frustum_t::vertex_idx_000, frustum_t::vertex_idx_100, frustum_t::vertex_idx_010);
    f.planes[frustum_t::plane_idx_pos_x] = build_plane(frustum_t::vertex_idx_100, frustum_t::vertex_idx_101, frustum_t::vertex_idx_110);
    f.planes[frustum_t::plane_idx_pos_y] = build_plane(frustum_t::vertex_idx_010, frustum_t::vertex_idx_110, frustum_t::vertex_idx_011);
    f.planes[frustum_t::plane_idx_pos_z] = build_plane(frustum_t::vertex_idx_001, frustum_t::vertex_idx_011, frustum_t::vertex_idx_101);

    // proper sign
    auto const center = invM * ndc[comp_t(0.5f, 0.5f, 0.5f)];
    if (signed_distance(center, f.planes[0]) > ScalarT(0))
    {
        f.planes[frustum_t::plane_idx_neg_x] = -f.planes[frustum_t::plane_idx_neg_x];
        f.planes[frustum_t::plane_idx_neg_y] = -f.planes[frustum_t::plane_idx_neg_y];
        f.planes[frustum_t::plane_idx_neg_z] = -f.planes[frustum_t::plane_idx_neg_z];
        f.planes[frustum_t::plane_idx_pos_x] = -f.planes[frustum_t::plane_idx_pos_x];
        f.planes[frustum_t::plane_idx_pos_y] = -f.planes[frustum_t::plane_idx_pos_y];
        f.planes[frustum_t::plane_idx_pos_z] = -f.planes[frustum_t::plane_idx_pos_z];
    }

    return f;
}

template <class ScalarT, class TraitsT>
constexpr inf_frustum<3, ScalarT, TraitsT> inf_frustum<3, ScalarT, TraitsT>::from_view_proj_reverse_z(mat<4, 4, ScalarT> const& m)
{
    // TODO: see if insight from http://www8.cs.umu.se/kurser/5DV051/HT12/lab/plane_extraction.pdf can be used

    CC_ASSERT(tg::determinant(m) != 0 && "cannot create frustum from singular matrices");
    CC_ASSERT((vec<3, ScalarT>(m.row(2)) == vec<3, ScalarT>::zero) && "requires reverse_z matrix. did you mean to use frustum?");

    auto const p1 = m.row(0);
    auto const p2 = m.row(1);
    auto const p3 = m.row(2);
    auto const p4 = m.row(3);

    tg::vec4 planes[5];
    // Gribb-Hartmann method
    // https://fgiesen.wordpress.com/2012/08/31/frustum-planes-from-the-projection-matrix/
    planes[0] = p4 + p1; // left
    planes[1] = p4 - p1; // right
    planes[2] = p4 + p2; // bottom
    planes[3] = p4 - p2; // top
    planes[4] = p4 - p3; // near

    // normalize planes
    inf_frustum<3, ScalarT, TraitsT> f;
    for (auto i = 0; i < 5; ++i)
    {
        tg::vec4& plane = planes[i];
        float const normalLengthInv = 1 / tg::sqrt(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
        plane *= normalLengthInv;
        f.planes[i] = tg::plane3(-tg::dir3(plane), plane.w);
    }

    return f;
}
}
