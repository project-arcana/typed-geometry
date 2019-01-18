#pragma once
#include "../../types/pos.hh"
#include "../../types/vec.hh"
#include "../operators/ops_pos.hh"
#include "../operators/ops_vec.hh"
#include "../traits.hh"
#include "../scalars/scalar_math.hh"

namespace tg
{
template <int D, class ScalarT>
constexpr squared_result<ScalarT> distance2(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return length2(a - b);
}

template <int D, class ScalarT>
constexpr squared_result<ScalarT> distance(pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return sqrt(distance2(a, b));
}
} // namespace tg
