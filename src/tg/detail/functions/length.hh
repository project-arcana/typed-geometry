#pragma once

#include "../../types/vec.hh"
#include "../operators/ops_vec.hh"
#include "../scalars/scalar_math.hh"
#include "../traits.hh"

namespace tg
{
template <int D, class ScalarT>
constexpr squared_result<ScalarT> length2(vec<D, ScalarT> const& v)
{
    auto a = vec<D, squared_result<ScalarT>>(v);
    return dot(a, a);
}

template <int D, class ScalarT>
constexpr fractional_result<ScalarT> length(vec<D, ScalarT> const& v)
{
    return sqrt(length2(vec<D, fractional_result<ScalarT>>(v)));
}

template <int D, class ScalarT>
constexpr fractional_result<ScalarT> length2(segment<D, ScalarT> const& s)
{
    return length2(s.a - s.b);
}

template <int D, class ScalarT>
constexpr fractional_result<ScalarT> length(segment<D, ScalarT> const& s)
{
    return sqrt(length2(segment<D, fractional_result<ScalarT>>(s)));
}
} // namespace tg
