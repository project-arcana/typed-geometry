#pragma once

#include <typed-geometry/common/limits.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include "aabb.hh"
#include "coordinates.hh"
#include "math.hh"
#include "perpendicular.hh"

// TODO how to swap in tg?
#include <algorithm>

/**
 * Rasterization of objects
 *
 * Enumerates all integer points (tg::ipos2, tg::ipos3, ...) that are contained in the objects
 * (e.g. contains(obj, pos) == true for all enumerated positions)
 */

namespace tg
{
// F: (tg::ipos2 p) -> void
template <class ScalarT, class F>
constexpr void fast_rasterize(triangle<2, ScalarT> const& t,
                              F&& f,
                              const tg::ipos2& limitMin = tg::ipos2(tg::detail::limits<int>().min()),
                              const tg::ipos2& limitMax = tg::ipos2(tg::detail::limits<int>().max()))
{
    // adaptive half-space rasterization
    // see https://www.uni-obuda.hu/journal/Mileff_Nehez_Dudra_63.pdf
    auto const fbox = aabb_of(t);
    auto const orientation = (fbox.max.x - fbox.min.x) / float(fbox.max.y - fbox.min.y);

    auto box = tg::aabb<2, int>(tg::ipos2(fbox.min), tg::ipos2(fbox.max));

    // TODO do before orientation computation?
    // margin so that we can safely round/clamp to integer coords
    // clip to render target

    // TODO left and right point computed later would be off?
    box.min = tg::ipos2(max(ifloor(fbox.min), limitMin));
    box.max = tg::ipos2(min(iceil(fbox.max), limitMax));

    // TODO assuming ccw! (irrelevant?)
    tg::pos<2, ScalarT> verts[3] = {t.pos0, t.pos1, t.pos2};

    // edge functions and constants
    // 3 constants per edge
    ScalarT edgeConstants[3 * 3];

    // AB, BC, CA
    for (auto e = 0; e < 3; e++)
    {
        auto next = (e + 1) % 3;
        edgeConstants[e * 3 + 0] = verts[e].y - verts[next].y;
        edgeConstants[e * 3 + 1] = verts[next].x - verts[e].x;
        edgeConstants[e * 3 + 2] = verts[e].x * verts[next].y - verts[e].y * verts[next].x;
    }
    auto edgeFunction = [edgeConstants](const tg::ipos2& P, int f = 0) {
        auto first = min(f * 3, 6);
        return edgeConstants[first + 0] * P.x + edgeConstants[first + 1] * P.y + edgeConstants[first + 2];
    };

    // TODO how to capture f? &f?
    auto renderBlock = [f, edgeFunction, edgeConstants](int x, int y, int sizeX = 8, int sizeY = 8, bool contained = false) {
        // all pixels in this block are contained, simply render
        if (contained)
        {
            // TODO <= ?
            for (auto py = y; py < y + sizeY; py++)
                for (auto px = x; px < x + sizeX; px++)
                    f(tg::ipos2(px, py));
        }
        else
        {
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
                // update cx values to new row too
                cx1 = cy1;
                cx2 = cy2;
                cx3 = cy3;

                for (auto px = x; px < x + sizeX; px++)
                {
                    // note: using > and < to comply with tg::contains. equal to 0 if on edge/corner.
                    if ((cx1 > 0 && cx2 > 0 && cx3 > 0) || (cx1 < 0 && cx2 < 0 && cx3 < 0))
                    {
                        f(tg::ipos2(px, py));
                    }
                    // increment
                    // TODO in paper they use a minus sign for no specified reason
                    cx1 += edgeConstants[3 * 0 + 0]; // I values
                    cx2 += edgeConstants[3 * 1 + 0];
                    cx3 += edgeConstants[3 * 2 + 0];
                }

                // update cy values
                cy1 += edgeConstants[3 * 0 + 1]; // J values
                cy2 += edgeConstants[3 * 1 + 1];
                cy3 += edgeConstants[3 * 2 + 1];
            }
        }
    };

    bool pixelSets = orientation > 0.4 && orientation < 1.6f;

    auto width = box.max.x - box.min.x;
    auto height = box.max.y - box.min.y;

    if (!pixelSets)
    {
        // render pixel by pixel
        renderBlock(box.min.x, box.min.y, width, height);
    }
    else
    {
        // TODO this might lead to skipping very small triangles, or does the midpoint method fix that?
        auto const blockSize = 8; // 8x8 supposedly is a good block size
        auto q = blockSize;       // step, will be flipped

        auto const halfY = (box.min.y + ((box.max.y - box.min.y) >> 1)) & ~(blockSize - 1);

        // sort vertices vertically (first is top)
        if (verts[0].y > verts[1].y)
            std::swap(verts[0], verts[1]);
        if (verts[1].y > verts[2].y)
        {
            std::swap(verts[1], verts[2]);
            if (verts[0].y > verts[1].y)
                std::swap(verts[0], verts[1]);
        }

        // we start at the top vertex
        auto leftPoint = ifloor(verts[0].x);
        auto rightPoint = iceil(verts[0].x);


        for (auto upwards = 0; upwards < 2; upwards++)
        {
            if (upwards)
            {
                // start at bottom midpoint to go upwards
                leftPoint = ifloor(verts[2].x);
                rightPoint = iceil(verts[2].x);
            }

            // TODO at some point not sure whether = should be involved, i.e. <= instead of <
            auto j = box.min.y;
            if (upwards)
                j = box.max.y &~(blockSize-1);
            for (; upwards ? j > halfY : j < halfY + q; upwards ? j -= q : j += q)
            {
                for (auto x = box.min.x; x <= box.max.x; x += q)
                {
                    // are all/none/some corners of current box contained?
                    // TODO why -1?
                    const int xVals[2] = {x, x + blockSize};
                    const int yVals[2] = {j, j + blockSize};
                    auto count = 0;
                    for (auto vx = 0; vx < 2; vx++)
                    {
                        for (auto vy = 0; vy < 2; vy++)
                        {
                            auto corner = tg::ipos2(xVals[vx], yVals[vy]);
                            auto F0 = edgeFunction(corner, 0);
                            auto F1 = edgeFunction(corner, 1);
                            auto F2 = edgeFunction(corner, 2);
                            if ((F0 > 0 && F1 > 0 && F2 > 0) || (F0 < 0 && F1 < 0 && F2 < 0))
                                count++;
                        }
                    }

                    // box is not on triangle at all
                    if (count == 0)
                    {
                        continue;
                    }
                    else
                    {
                        // box completely contained, render all contained pixels
                        if (count == 4)
                        {
                            renderBlock(x, j, q, q, true); // true: just render
                        }
                        // box partly contained, fallback to pixel rendering
                        else
                        {
                            renderBlock(x, j, q, q);
                        }
                    }
                }
            }
        }
    }
}

// F: (tg::ipos2 p, float a, float b) -> void
template <class ScalarT, class F>
constexpr void rasterize(triangle<2, ScalarT> const& t,
                         F&& f,
                         const tg::ipos2& limitMin = tg::ipos2(tg::detail::limits<int>().min()),
                         const tg::ipos2& limitMax = tg::ipos2(tg::detail::limits<int>().max()))
{
    auto const box = aabb_of(t);

    // margin so that we can safely round/clamp to integer coords
    // limitMin/Max to only rasterize on desired image size
    auto const minPix = max(ifloor(box.min), limitMin);
    auto const maxPix = min(iceil(box.max), limitMax);

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
