#pragma once

#include "../../types/objects/box.hh"
#include "../../types/pos.hh"
#include "../../types/size.hh"
#include "../../types/objects/triangle.hh"
#include "../../types/vec.hh"
#include "../operators/ops_vec.hh"
#include "../scalar_traits.hh"
#include "../scalars/scalar_math.hh"

namespace tg
{
template <class ScalarT>
constexpr squared_result<ScalarT> area(size<2, ScalarT> const& s)
{
    return s.width * s.height;
}

template <class ScalarT>
constexpr squared_result<ScalarT> area(box<2, ScalarT> const& b)
{
    return area(size<2, ScalarT>(b.max - b.min));
}

template <class ScalarT>
constexpr fractional_result<ScalarT> signed_area(triangle<2, ScalarT> const& b)
{
    return cross(b.v1 - b.v0, b.v2 - b.v0) * fractional_result<ScalarT>(0.5);
}

template <class ScalarT>
constexpr fractional_result<ScalarT> area(triangle<2, ScalarT> const& b)
{
    return abs(signed_area(b));
}

template <class ScalarT>
constexpr fractional_result<ScalarT> area(triangle<3, ScalarT> const& b)
{
    return length(cross(b.v1 - b.v0, b.v2 - b.v0)) * fractional_result<ScalarT>(0.5);
}
} // namespace tg
