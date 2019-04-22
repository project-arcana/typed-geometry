#pragma once

#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include "cross.hh"
#include "normal.hh"

namespace tg
{
// For a given triangle with 3D positions pos and 2D tex coords uv,
// return the world space tangent of the triangle
template <class ScalarT>
TG_NODISCARD constexpr dir<3, ScalarT> tangent(triangle<3, ScalarT> const& pos, triangle<2, ScalarT> const& uv)
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

    return normalize(tangent);
}

// Returns the TBN matrix, i.e. the tangent frame for the given triangle position and uv coords (not necessarily orthogonal)
template <class ScalarT>
TG_NODISCARD constexpr mat<3, 3, ScalarT> tbn_matrix(triangle<3, ScalarT> const& pos, triangle<2, ScalarT> const& uv)
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
    auto t = dir * (p20 * v10 - p10 * v20);
    auto b = dir * (p10 * u20 - p20 * u10);

    t = normalize_safe(t);
    b = normalize_safe(b);
    auto n = normalize_safe(cross(t, b));

    return {{t, b, n}};
}
}
