#pragma once

#include <utility>

#include "../../types/line.hh"
#include "../../types/pos.hh"
#include "project.hh"

// closest_points(a, b) returns points {p_a, p_b} such that contains(a, p_a) and contains(b, p_b) and |p_a - p_b| is minimal

namespace tg
{
template <int D, class ScalarT>
constexpr std::pair<pos<D, ScalarT>, pos<D, ScalarT>> closest_points(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return {a, b};
}

template <int D, class ScalarT>
constexpr std::pair<pos<D, ScalarT>, pos<D, ScalarT>> closest_points(line<D, ScalarT> const& l, pos<D, ScalarT> const& p)
{
    return {project(p, l), p};
}

template <class ScalarT>
constexpr std::pair<pos<3, ScalarT>, pos<3, ScalarT>> closest_points(plane<ScalarT> const& pl, pos<3, ScalarT> const& p)
{
    return {project(p, pl), p};
}
} // namespace tg
