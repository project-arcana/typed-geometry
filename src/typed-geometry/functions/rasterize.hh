#pragma once

#include <typed-geometry/common/limits.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include "aabb.hh"
#include "coordinates.hh"
#include "math.hh"

/**
 * Rasterization of objects
 *
 * Enumerates all integer points (tg::ipos2, tg::ipos3, ...) that are contained in the objects
 * (e.g. contains(obj, pos) == true for all enumerated positions)
 */

namespace tg
{
// no barycentric coords returned
// F: (tg::ipos2 p) -> void
template <class ScalarT, class F>
constexpr void fast_rasterize(triangle<2, ScalarT> const& t, F&& f, tg::vec<2, ScalarT> const& offset = tg::vec<2, ScalarT>(0))
{
    // adaptive half-space rasterization
    // see https://www.uni-obuda.hu/journal/Mileff_Nehez_Dudra_63.pdf
    auto const fbox = aabb_of(t);

    auto box = tg::aabb<2, int>(tg::ipos2(ifloor(fbox.min)), tg::ipos2(iceil(fbox.max)));

    // edge functions and their constants
    ScalarT edgeConstants[3 * 3];
    tg::pos<2, ScalarT> verts[3] = {t.pos0, t.pos1, t.pos2};

    // edges AB, BC and CA
    for (auto e = 0; e < 3; e++)
    {
        auto next = (e + 1) % 3;
        edgeConstants[e * 3 + 0] = verts[e].y - verts[next].y;
        edgeConstants[e * 3 + 1] = verts[next].x - verts[e].x;
        edgeConstants[e * 3 + 2] = verts[e].x * verts[next].y - verts[e].y * verts[next].x;
    }

    auto edgeFunction = [edgeConstants, offset](tg::ipos2 pos, int f) {
        auto first = min(f * 3, 6);
        return edgeConstants[first + 0] * (pos.x + offset.x) + edgeConstants[first + 1] * (pos.y + offset.y) + edgeConstants[first + 2];
    };

    auto renderBlock = [f, edgeFunction, edgeConstants](int x, int y, int sizeX, int sizeY) {
        if (sizeX * sizeY <= 0)
            return;

        // compute once, increment later
        auto pos = tg::ipos2(x, y);
        auto cy1 = edgeFunction(pos, 0);
        auto cy2 = edgeFunction(pos, 1);
        auto cy3 = edgeFunction(pos, 2);

        auto cx1 = cy1;
        auto cx2 = cy2;
        auto cx3 = cy3;

        for (auto py = y; py < y + sizeY; py++)
        {
            // y has changed, clip cx to cy
            cx1 = cy1;
            cx2 = cy2;
            cx3 = cy3;

            for (auto px = x; px < x + sizeX; px++)
            {
                // allows for both ccw and cc vertex order
                auto in = (cx1 < 0 && cx2 < 0 && cx3 < 0) || (cx1 >= 0 && cx2 >= 0 && cx3 >= 0);
                if (in)
                {
                    f(tg::ipos2(px, py));
                }

                // increment
                cx1 += edgeConstants[3 * 0 + 0]; // I values
                cx2 += edgeConstants[3 * 1 + 0];
                cx3 += edgeConstants[3 * 2 + 0];
            }

            // update cy values
            cy1 += edgeConstants[3 * 0 + 1]; // J values
            cy2 += edgeConstants[3 * 1 + 1];
            cy3 += edgeConstants[3 * 2 + 1];
        }
    };

    auto width = box.max.x - box.min.x;
    auto height = box.max.y - box.min.y;

    renderBlock(box.min.x, box.min.y, width, height);
}

// F: (tg::ipos2 p, float a, float b) -> void
template <class ScalarT, class F>
constexpr void rasterize(triangle<2, ScalarT> const& t, F&& f)
{
    auto const box = aabb_of(t);

    // margin so that we can safely round/clamp to integer coords
    auto const minPix = ifloor(box.min);
    auto const maxPix = iceil(box.max);

    // TODO: Bresenham on two of the triangle edges, then scanline
    for (auto y = minPix.y; y <= maxPix.y; ++y)
        for (auto x = minPix.x; x <= maxPix.x; ++x)
        {
            auto const pos = tg::pos<2, ScalarT>(ScalarT(x), ScalarT(y));
            auto const bary = coordinates(t, pos);
            auto const a = bary[0];
            auto const b = bary[1];
            auto const c = bary[2];
            if (a >= 0 && b >= 0 && c >= 0)
            {
                // inside triangle
                f(ipos2(x, y), a, b);
            }
        }
}

} // namespace tg
