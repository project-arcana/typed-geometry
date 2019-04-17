#pragma once

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <class ScalarT>
TG_NODISCARD constexpr vec<3, squared_result<ScalarT>> cross(vec<3, ScalarT> const& a, vec<3, ScalarT> const& b)
{
    vec<3, ScalarT> r;
    r.x = a.y * squared_result<ScalarT>(b.z) - a.z * squared_result<ScalarT>(b.y);
    r.y = a.z * squared_result<ScalarT>(b.x) - a.x * squared_result<ScalarT>(b.z);
    r.z = a.x * squared_result<ScalarT>(b.y) - a.y * squared_result<ScalarT>(b.x);
    return r;
}

template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> cross(vec<2, ScalarT> const& a, vec<2, ScalarT> const& b)
{
    return a.x * squared_result<ScalarT>(b.y) - a.y * squared_result<ScalarT>(b.x);
}

} // namespace tg
