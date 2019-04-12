#pragma once

#include "hyperplane.hh"

// A plane is a 2-dimensional geometric object embedded into an n-dimensional space.
// It is defined by a distance to the origin and a normal vector
// The most meaningful version is the embedding in three dimensions

namespace tg
{
// Common plane types

using plane = hyperplane<3, f32>;
using fplane = hyperplane<3, f32>;
using dplane = hyperplane<3, f64>;
using iplane = hyperplane<3, i32>;
using uplane = hyperplane<3, u32>;


// ======== IMPLEMENTATION ========
// see hyperplane.hh

} // namespace tg
