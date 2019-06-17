#pragma once

#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/common/scalar_math.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr ScalarT length_sqr(vec<D, ScalarT> const& v)
{
    auto a = vec<D, ScalarT>(v);
    return dot(a, a);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr fractional_result<ScalarT> length(vec<D, ScalarT> const& v)
{
    return tg::sqrt(length_sqr(vec<D, fractional_result<ScalarT>>(v)));
}

template <int D, class ScalarT>
TG_NODISCARD constexpr fractional_result<ScalarT> length_sqr(segment<D, ScalarT> const& s)
{
    return length_sqr(s.pos0 - s.pos1);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr fractional_result<ScalarT> length(segment<D, ScalarT> const& s)
{
    return tg::sqrt(length_sqr(segment<D, fractional_result<ScalarT>>(s)));
}
} // namespace tg
