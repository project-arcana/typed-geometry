#pragma once

#include "../../types/objects/triangle.hh"
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
// F: (tg::ipos2 p, float a, float b) -> void
template <class ScalarT, class F>
void rasterize(triangle<2, ScalarT> const& t, F&& f)
{
    auto const b = aabb(t);

    // margin so that we can safely round/clamp to integer coords
    auto const minPix = ifloor(b.min);
    auto const maxPix = iceil(b.max);

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
