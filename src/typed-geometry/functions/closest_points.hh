#pragma once

#include <typed-geometry/detail/utility.hh>
#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/plane.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/pos.hh>

#include "coordinates.hh"
#include "mix.hh"
#include "project.hh"

// closest_points(a, b) returns points {p_a, p_b} such that contains(a, p_a) and contains(b, p_b) and |p_a - p_b| is minimal

namespace tg
{
// Base case for point/point:
template <int D, class ScalarT>
TG_NODISCARD constexpr pair<pos<D, ScalarT>, pos<D, ScalarT>> closest_points(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return {a, b};
}

// Default implementation for point/object and object/point:
template <int D, class ScalarT, class ObjectT>
TG_NODISCARD constexpr auto closest_points(ObjectT const& o, pos<D, ScalarT> const& p) -> pair<decltype(project(p, o)), pos<D, ScalarT>>
{
    return {project(p, o), p};
}
template <int D, class ScalarT, class ObjectT>
TG_NODISCARD constexpr auto closest_points(pos<D, ScalarT> const& p, ObjectT const& o) -> pair<pos<D, ScalarT>, decltype(project(p, o))>
{
    return {p, project(p, o)};
}


// =========== Object Implementations ===========

// TODO
} // namespace tg
