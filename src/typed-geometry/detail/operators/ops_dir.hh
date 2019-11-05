#pragma once

#include <typed-geometry/detail/macros.hh>
#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
// comparison with vec
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(dir, vec, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(dir, vec, bool, operator!=, ||, !=);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(vec, dir, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(vec, dir, bool, operator!=, ||, !=);

// dir +- dir = vec
TG_IMPL_DEFINE_BINARY_OP(dir, dir, vec, +);
TG_IMPL_DEFINE_BINARY_OP(dir, dir, vec, -);

// +dir, -dir
TG_IMPL_DEFINE_UNARY_OP(dir, +);
TG_IMPL_DEFINE_UNARY_OP(dir, -);

// decays to vector
template <int D, class ScalarT, class T>
[[nodiscard]] constexpr auto operator*(dir<D, ScalarT> const& lhs, T const& rhs) -> decltype(vec<D, ScalarT>(lhs) * rhs)
{
    return vec<D, ScalarT>(lhs) * rhs;
}
template <int D, class ScalarT, class T>
[[nodiscard]] constexpr auto operator/(dir<D, ScalarT> const& lhs, T const& rhs) -> decltype(vec<D, ScalarT>(lhs) / rhs)
{
    return vec<D, ScalarT>(lhs) / rhs;
}
template <int D, class ScalarT, class T>
[[nodiscard]] constexpr auto operator+(dir<D, ScalarT> const& lhs, T const& rhs) -> decltype(vec<D, ScalarT>(lhs) + rhs)
{
    return vec<D, ScalarT>(lhs) + rhs;
}
template <int D, class ScalarT, class T>
[[nodiscard]] constexpr auto operator-(dir<D, ScalarT> const& lhs, T const& rhs) -> decltype(vec<D, ScalarT>(lhs) - rhs)
{
    return vec<D, ScalarT>(lhs) - rhs;
}

template <int D, class ScalarT, class T>
[[nodiscard]] constexpr auto operator*(T const& lhs, dir<D, ScalarT> const& rhs) -> decltype(lhs * vec<D, ScalarT>(rhs))
{
    return lhs * vec<D, ScalarT>(rhs);
}
template <int D, class ScalarT, class T>
[[nodiscard]] constexpr auto operator/(T const& lhs, dir<D, ScalarT> const& rhs) -> decltype(lhs / vec<D, ScalarT>(rhs))
{
    return lhs / vec<D, ScalarT>(rhs);
}
template <int D, class ScalarT, class T>
[[nodiscard]] constexpr auto operator+(T const& lhs, dir<D, ScalarT> const& rhs) -> decltype(lhs + vec<D, ScalarT>(rhs))
{
    return lhs + vec<D, ScalarT>(rhs);
}
template <int D, class ScalarT, class T>
[[nodiscard]] constexpr auto operator-(T const& lhs, dir<D, ScalarT> const& rhs) -> decltype(lhs - vec<D, ScalarT>(rhs))
{
    return lhs - vec<D, ScalarT>(rhs);
}

} // namespace tg
