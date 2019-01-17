#pragma once

#include "../../types/triangle.hh"

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
    // TODO
}

} // namespace tg
