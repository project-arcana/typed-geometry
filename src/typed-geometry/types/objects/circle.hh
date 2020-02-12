#pragma once

#include "sphere.hh"

namespace tg
{
template <int D, class ScalarT>
using circle = sphere<2, ScalarT, D, boundary_tag>;

// Common circle types
using circle2 = circle<2, f32>;
using fcircle2 = circle<2, f32>;
using dcircle2 = circle<2, f64>;
using icircle2 = circle<2, i32>;
using ucircle2 = circle<2, u32>;

using circle2in3 = circle<3, f32>;
using fcircle2in3 = circle<3, f32>;
using dcircle2in3 = circle<3, f64>;
using icircle2in3 = circle<3, i32>;
using ucircle2in3 = circle<3, u32>;
}
