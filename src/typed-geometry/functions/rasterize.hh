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
// F: (tg::ipos2 p, float a) -> void
template <class ScalarT, class F>
constexpr void rasterize(segment<2, ScalarT> const& l, F&& f)
{
    // bresenham, see http://www.roguebasin.com/index.php?title=Bresenham%27s_Line_Algorithm
    // TODO round? or just int?
    auto x0 = iround(l.pos0.x);
    auto x1 = iround(l.pos1.x);
    auto y0 = iround(l.pos0.y);
    auto y1 = iround(l.pos1.y);

    // TODO call f at x,y or with offset of 0.5f? tolerance in tests is quite high

    auto delta_x = x1 - x0;
    // if x1 == x2, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;

    auto delta_y = y1 - y0;
    // if y1 == y2, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;

    // start
    f(tg::ipos2(x0, y0), 0);


    // TODO x equal : tie break with y
    auto a = ScalarT(0); // l.pos0.x <= l.pos1.x ? ScalarT(0.0) : ScalarT(1.0);


    if (delta_x >= delta_y)
    {
        // done
        if (x0 == x1)
            return;
        auto aStep = ScalarT(1) / abs(x1 - x0); // round(abs(l.pos0.x - l.pos1.x));

        // aStep = ScalarT(1) / abs(l.pos0.x - l.pos1.x);
        // error may go below zero
        int error(delta_y - (delta_x >> 1));

        while (x0 != x1)
        {
            // reduce error, while taking into account the corner case of error == 0
            if ((error > 0) || (!error && (ix > 0)))
            {
                error -= delta_x;
                y0 += iy;
            }
            // else do nothing

            error += delta_y;
            x0 += ix;

            // TODO value "a" might not need to be clipped as it only SLIGHTLY exceeds 1.0 in rare cases
            // (precision of tg::epsilon<float> * 3 in test cases!)
            if (x1 == x0)
                a = 1;
            else
                a += aStep;
            f(tg::ipos2(x0, y0), a);
        }
    }
    else
    {
        // done
        if (y0 == y1)
            return;
        auto aStep = ScalarT(1) / abs(y1 - y0); // round(abs(l.pos0.x - l.pos1.x));
        // error may go below zero
        int error(delta_x - (delta_y >> 1));

        while (y1 != y0)
        {
            // reduce erl.pos1.x - l.pos0.xror, while taking into account the corner case of error == 0
            if ((error > 0) || (!error && (iy > 0)))
            {
                error -= delta_y;
                x0 += ix;
            }
            // else do nothing

            error += delta_x;
            y0 += iy;

            if (y1 == y0)
                a = 1;
            else
                a += aStep;
            f(tg::ipos2(x0, y0), a);
        }
    }
    // TODO add option to not calculate parametric value?
    // TODO add limits?
}

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
