#pragma once

#include <tg/typed-geometry-lean.hh>

#include "../traits.hh"

// returns the arithmetic mean of all points contained in an object
// has variadic versions

namespace tg
{
template <int D, class ScalarT>
pos<D, ScalarT> centroid(pos<D, ScalarT> const& p)
{
    return p;
}

template <int D, class ScalarT>
pos<D, fractional_result<ScalarT>> centroid(box<D, ScalarT> const& p)
{
    return p.min + (p.max - p.min) * 0.5f;
}

template <int D, class ScalarT>
pos<D, fractional_result<ScalarT>> centroid(triangle<D, ScalarT> const& p)
{
    return p.v0 + (p.v1 - p.v0) * (1 / 3.0f) + (p.v2 - p.v0) * (1 / 3.0f);
}
} // namespace tg
