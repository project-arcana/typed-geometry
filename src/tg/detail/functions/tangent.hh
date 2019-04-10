#pragma once

#include "../../types/objects/triangle.hh"
#include "../../types/mat.hh"
#include "normal.hh"
#include "cross.hh"

namespace tg
{
// For a given triangle with 3D positions pos and 2D tex coords uv,
// return the world space tangent of the triangle
template <class ScalarT>
constexpr vec<3, ScalarT> tangent(triangle<3, ScalarT> const& pos, triangle<2, ScalarT> const& uv)
{
    auto p10 = pos.v1 - pos.v0;
    auto p20 = pos.v2 - pos.v0;

    auto uv10 = uv.v1 - uv.v0;
    auto uv20 = uv.v2 - uv.v0;

    auto u10 = uv10.x;
    auto u20 = uv20.x;
    auto v10 = uv10.y;
    auto v20 = uv20.y;

    auto dir = u20 * v10 - u10 * v20;
    auto tangent = dir * (p20 * v10 - p10 * v20);

    // TODO: eps reasonable?
    constexpr ScalarT eps = ScalarT(1e-6);
    return normalize_safe(tangent, eps);
}

// Returns the TBN matrix, i.e. the tangent frame for the given triangle position and uv coords
template <class ScalarT>
constexpr mat<3,3, ScalarT> tangent_space(triangle<3, ScalarT> const& pos, triangle<2, ScalarT> const& uv)
{
    // TODO: optimized version instead?
    auto t = tangent(pos, uv);
    auto n = normal(pos);
    auto b = cross(n, t);

    return {t, b, n};
}
}
