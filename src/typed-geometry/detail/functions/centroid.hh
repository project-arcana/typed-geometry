#pragma once

#include "../../types/objects/box.hh"
#include "../../types/objects/segment.hh"
#include "../../types/objects/sphere.hh"
#include "../../types/objects/triangle.hh"
#include "../scalar_traits.hh"
#include "../special_values.hh"

// returns the arithmetic mean of all points contained in an object
// has variadic versions

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> centroid(pos<D, ScalarT> const& p)
{
    return p;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, fractional_result<ScalarT>> centroid(box<D, ScalarT> const& p)
{
    auto z = tg::zero<pos<D, ScalarT>>();
    return z + ((p.min - z) + (p.max - z)) / ScalarT(2);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, fractional_result<ScalarT>> centroid(triangle<D, ScalarT> const& p)
{
    auto z = tg::zero<pos<D, ScalarT>>();
    return z + ((p.v0 - z) + (p.v1 - z) + (p.v2 - z)) / ScalarT(3);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, fractional_result<ScalarT>> centroid(segment<D, ScalarT> const& p)
{
    return mix(p.a, p.b, fractional_result<ScalarT>(0.5));
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> centroid(sphere<D, ScalarT> const& p)
{
    return p.center;
}
} // namespace tg
