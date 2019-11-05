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
    // thus currently skipped
    if (false)
    {
        box.min = tg::ipos2(max(ifloor(fbox.min), limitMin));
        box.max = tg::ipos2(min(iceil(fbox.max), limitMax));
    }

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
    auto renderBlock = [f, edgeFunction, edgeConstants](int x, int y, int size, bool contained) {
        // all pixels in this block are contained, simply render
        if (contained)
        {
            for (auto py = y; py < y + size; py++)
                for (auto px = x; px < x + size; px++)
                    f(tg::ipos2(px, py));
        }
        else
        {
            // compute once, then increment as we go zig-zag
            auto pos = tg::ipos2(x, y);
            auto cy1 = edgeFunction(pos, 0);
            auto cy2 = edgeFunction(pos, 1);
            auto cy3 = edgeFunction(pos, 2);

            auto cx1 = cy1;
            auto cx2 = cy2;
            auto cx3 = cy3;


            // TODO incremental half space with edge functions
            for (auto py = y; py < y + size; py++)
            {
                // update cx values to new row too
                cx1 = cy1;
                cx2 = cy2;
                cx3 = cy3;

                for (auto px = x; px < x + size; px++)
                {
                    // TODO not sure about sign here, this should do it though
                    // TODO in paper just > not >=
                    if ((cx1 >= 0 && cx2 >= 0 && cx3 >= 0) || (cx1 < 0 && cx2 < 0 && cx3 < 0))
                    {
                    }
                    // increment
                    // TODO minus sign?
                    cx1 += edgeConstants[3 * 0]; // I values
                    cx2 += edgeConstants[3 * 1];
                    cx3 += edgeConstants[3 * 2];
                }

                // update cy values
                cy1 += edgeConstants[3 * 0 + 1]; // J values
                cy2 += edgeConstants[3 * 1 + 1];
                cy3 += edgeConstants[3 * 2 + 1];
            }
        }
    };

    // TODO rn: true, always use block rasterization
    if (true || (orientation > 0.4 && orientation < 1.6f))
    {
        // find topmost vertex to get left and right point
        // sort vertices vertically
        if (verts[0].y > verts[1].y)
            std::swap(verts[0], verts[1]);
        if (verts[1].y > verts[2].y)
        {
            std::swap(verts[1], verts[2]);
            if (verts[0].y > verts[1].y)
                std::swap(verts[0], verts[1]);
        }

        // TODO paper does not specify what left and right point exactly is supposed to be initially
        // assuming is simply the x coord, but ifloor/iceil?

        // first element is top
        auto leftPoint = ifloor(verts[0].x);
        auto rightPoint = iceil(verts[0].x);

        int const blockSize = 8; // 8x8 supposedly is a good block size
        auto q = blockSize;
        // TODO maybe use normal arithmetic? or proper box type?
        auto const halfY = (box.min.y + ((box.max.y - box.min.y) >> 1)) & ~(blockSize - 1);

        for (auto run = 0; run < 2; run++)
        {
            // run from top, then bottom
            if (run > 0)
            {
                // use bottom vertex
                leftPoint = ifloor(verts[2].x);
                rightPoint = iceil(verts[2].x);
            }
            auto j = box.min.y;
            auto increment = blockSize;

            if (run > 0)
            {
                j = box.max.y & ~(blockSize - 1);
                increment *= -1;
            }

            for (; run == 0 ? j < halfY + blockSize : j > halfY; j += increment)
            {
                // TODO not sure if bit operation is correct
                auto midpoint = (leftPoint + ((rightPoint - leftPoint) >> 1)) & ~(blockSize - 1);
                auto x = midpoint;

                for (auto k = 0; k < 2; k++)
                {
                    // from midpoint, walk to left/right border in steps of boxsize
                    for (; q > 0 ? x < box.max.x : x > box.min.x - q; x += q)
                    {
                        // are all/none/some corners of current box contained?
                        const int xVals[2] = {x, x + blockSize - 1};
                        const int yVals[2] = {j, j + blockSize - 1};

                        auto count = 0;
                        for (auto vx = 0; vx < 2; vx++)
                            for (auto vy = 0; vy < 2; vy++)
                            {
                                auto corner = tg::ipos2(xVals[vx], yVals[vy]);
                                if (edgeFunction(corner, 0) >= 0 && edgeFunction(corner, 1) >= 0 && edgeFunction(corner, 2) >= 0)
                                {
                                    count++;
                                }
                            }
                        if (count == 0)
                        {
                            // TODO pseudo code in paper says "continue", but shouldn't we break here?
                            // "When the edge of a triangle is reached in a row, we can move straight to the next row because only empty blocks can be
                            // found from this position."
                            break;
                        }
                        else
                        {
                            if (count == 4)
                            {
                                // all corners inside triangle
                                // render block with topleft corner at i, j and size q*q
                                renderBlock(x, j, q, true); // true: just render
                            }
                            else
                            {
                                // some contained, fall back to incremental half-space rasterization
                                renderBlock(x, j, q, false); // false: check for each pixel
                            }
                        }
                    }
                    // swap direction
                    q = -q;

                    // TODO paper mentions that check is necessary whether we started *outside* the triangle before updating the bounds
                    if (k == 0)
                        rightPoint = x - blockSize;
                    else
                        leftPoint = x + blockSize;
                    x = midpoint - blockSize;
                }
            }
        }
    }
}

/*
template  <class ScalarT>
constexpr  ScalarT perp_dot(tg::vec<2, ScalarT> a, tg::vec<2, ScalarT> b){
    // 2d equivalent of 3d cross-product
    return dot(perpendicular(a), b);
}
// F: (tg::ipos2 p) -> void
template <class ScalarT, class F>
constexpr void simple_fill(triangle<2, ScalarT> const& t,
                         F&& f,
                         const tg::ipos2& limitMin = tg::ipos2(tg::detail::limits<int>().min()),
                         const tg::ipos2& limitMax = tg::ipos2(tg::detail::limits<int>().max()))
{
    // half-space rasterization
    // adapted from page 7 https://www.uni-obuda.hu/journal/Mileff_Nehez_Dudra_63.pdf

    auto const box = aabb_of(t);

    // margin so that we can safely round/clamp to integer coords
    // limitMin/Max to only rasterize on desired image size
    auto const minPix = max(ifloor(box.min), limitMin);
    auto const maxPix = min(iceil(box.max), limitMax);

    // TODO the paper seems to be wrong here

    auto ab = t.pos1 - t.pos0; // from a to b
    auto bc = t.pos2 - t.pos1;
    auto ca = t.pos0 - t.pos2;

    for (auto y = minPix.y; y <= maxPix.y; ++y)
        for (auto x = minPix.x; x <= maxPix.x; ++x)
        {
            auto const pos = tg::pos<2, ScalarT>(ScalarT(x), ScalarT(y));
            auto const c0 = perp_dot(ab, pos - t.pos0);
            auto const c1 = perp_dot(bc, pos - t.pos1);
            auto const c2 = perp_dot(ca, pos - t.pos2);

            // perp_dot(a,b) > 0 -> b is ccw from a
            if ((c0 >= 0 && c1 >= 0 && c2 >= 0) || (c0 < 0 && c1 < 0 && c2 < 0))
            {
                // inside triangle
                f(ipos2(x, y));
            }
        }
}
*/

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
