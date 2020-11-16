#pragma once

#include "cylinder.hh"
#include "inf_cylinder.hh"

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


template <int D, class ScalarT>
using inf_tube = inf_cylinder_boundary<D, ScalarT>;

// Common inf_tube types
using inf_tube3 = inf_tube<3, f32>;
using finf_tube3 = inf_tube<3, f32>;
using dinf_tube3 = inf_tube<3, f64>;
using iinf_tube3 = inf_tube<3, i32>;
using uinf_tube3 = inf_tube<3, u32>;
}
