#pragma once

#include "cylinder.hh"

namespace tg
{
template <int D, class ScalarT>
using tube = cylinder_boundary_no_caps<D, ScalarT>;

// Common tube types
using tube3 = tube<3, f32>;
using ftube3 = tube<3, f32>;
using dtube3 = tube<3, f64>;
using itube3 = tube<3, i32>;
using utube3 = tube<3, u32>;
}
