#pragma once

#include "hyperplane.hh"
#include "pos.hh"
#include "scalar.hh"
#include "vec.hh"

// A plane is a 2-dimensional geometric object embedded into an n-dimensional space.
// It is defined by a distance to the origin and a normal vector
// The most meaningful version is the embedding in three dimensions

namespace tg
{
template <class ScalarT>
using plane3 = hyperplane<3, ScalarT>;

// Common plane types

using plane = plane3<f32>;
using fplane = plane3<f32>;
using dplane = plane3<f64>;
using iplane = plane3<i32>;
using uplane = plane3<u32>;


// ======== IMPLEMENTATION ========
// see hyperplane.hh

} // namespace tg
