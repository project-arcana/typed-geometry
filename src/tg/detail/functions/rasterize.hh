#pragma once

#include "../../types/triangle.hh"
#include "aabb.hh"
#include "coordinates.hh"


/**
 * Rasterization of objects
 *
 * Enumerates all integer points (tg::ipos2, tg::ipos3, ...) that are contained in the objects
 * (e.g. contains(obj, pos) == true for all enumerated positions)
 */

namespace tg
{
// F: (tg::ipos2 p, float a, float b) -> void
template <class ScalarT, class F>
void rasterize(triangle<2, ScalarT> const& t, F&& f)
{
    auto b = aabb(t);

    // margin so that we can safely round/clamp to integer coords
    auto minPix = ipos2(b.min.x + 1, b.min.y + 1);
    auto maxPix = ipos2(b.max.x + 1, b.max.y + 1);

    // TODO: Bresenham on two of the triangle edges, then scanline
    for (auto y = minPix.y; y <= maxPix.y; ++y)
        for (auto x = minPix.x; x <= maxPix.x; ++x)
        {
            auto pos = pos2(x, y);
            auto bary = coordinates(t, pos);
            auto a = bary[0];
            auto b = bary[1];
            auto c = bary[2];
            if (a >= 0 && b >= 0 && c >= 0)
            {
                // inside triangle
                f(ipos2(pos), a, b);
            }
        }
}

} // namespace tg
