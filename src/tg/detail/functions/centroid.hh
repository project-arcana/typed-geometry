#pragma once

#include "../../types/box.hh"
#include "../../types/triangle.hh"
#include "../scalar_traits.hh"

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
    auto z = pos<D, ScalarT>::zero;
    return z + ((p.min - z) + (p.max - z)) / ScalarT(2);
}

template <int D, class ScalarT>
pos<D, fractional_result<ScalarT>> centroid(triangle<D, ScalarT> const& p)
{
    auto z = pos<D, ScalarT>::zero;
    return z + ((p.v0 - z) + (p.v1 - z) + (p.v2 - z)) / ScalarT(3);
}
} // namespace tg
