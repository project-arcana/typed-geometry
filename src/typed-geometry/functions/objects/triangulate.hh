#pragma once

#include <typed-geometry/feature/basic.hh>
#include <typed-geometry/functions/objects/triangulation.hh>

namespace tg
{
/// calls on_triangle for each triangle of the objects triangulation
/// on_triangle: (tg::triangle) -> void
template <class Obj, class OnTriangle, std::enable_if_t<has_triangulation_of<Obj>, int> = 0>
constexpr void triangulate(Obj const& obj, OnTriangle&& on_triangle)
{
    for (auto&& t : triangulation_of(obj))
        on_triangle(t);
}

/// computes a uv triangulation of the given sphere and calls on_triangle with each triangle
/// on_triangle: (tg::triangle) -> void
/// NOTE: currently recomputes a lot of sin/cos and thus is not the fastest
/// NOTE: normal_of(t) points outwards
/// NOTE: segs_u must be >= 3, segs_v must be >= 2
template <class ScalarT, class TraitsT, class OnTriangle>
void triangulate_uv(sphere<3, ScalarT, 3, TraitsT> const& s, int segs_u, int segs_v, OnTriangle&& on_triangle)
{
    TG_ASSERT(segs_u >= 3);
    TG_ASSERT(segs_v >= 2);
    // TODO: some caching of sin/cos

    using dir_t = dir<3, ScalarT>;

    auto const dir_of = [&](int u, int v) -> dir_t {
        if (v == 0)
            return dir_t(0, 1, 0);
        else if (v == segs_v)
            return dir_t(0, -1, 0);

        auto [su, cu] = tg::sin_cos(tau<ScalarT> * (u == segs_u ? 0 : u) / ScalarT(segs_u));
        auto [sv, cv] = tg::sin_cos(pi<ScalarT> * v / ScalarT(segs_v));

        return dir_t(sv * su, cv, sv * cu);
    };
    auto const pos_of = [&](int u, int v) {
        auto d = dir_of(u, v);
        return s.center + d * s.radius;
    };

    // cap u
    {
        auto p0 = pos_of(0, 0);
        for (auto i = 0; i < segs_u; ++i)
        {
            auto p1 = pos_of(i, 1);
            auto p2 = pos_of(i + 1, 1);

            on_triangle(tg::triangle3(p0, p1, p2));
        }
    }

    // inner grid
    for (auto j = 1; j < segs_v - 1; ++j)
    {
        for (auto i = 0; i < segs_u; ++i)
        {
            auto p00 = pos_of(i + 0, j + 0);
            auto p01 = pos_of(i + 0, j + 1);
            auto p10 = pos_of(i + 1, j + 0);
            auto p11 = pos_of(i + 1, j + 1);

            on_triangle(tg::triangle3(p00, p01, p11));
            on_triangle(tg::triangle3(p00, p11, p10));
        }
    }

    // cap v
    {
        auto p0 = pos_of(0, segs_v);
        for (auto i = 0; i < segs_u; ++i)
        {
            auto p1 = pos_of(i, segs_v - 1);
            auto p2 = pos_of(i + 1, segs_v - 1);

            on_triangle(tg::triangle3(p0, p2, p1));
        }
    }
}
}
