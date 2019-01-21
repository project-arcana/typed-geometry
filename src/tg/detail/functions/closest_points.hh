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
    auto pointOnLine = l.p + project(p - l.p, l.dir);
    return {pointOnLine, p};
}
} // namespace tg
