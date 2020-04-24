#pragma once

#include <typed-geometry/detail/utility.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/types/mat.hh>

namespace tg
{
template <int C, int R, class ScalarT, class = enable_if<min(C, R) == 1>>
[[nodiscard]] constexpr vec<1, ScalarT> diag(mat<C, R, ScalarT> const& m)
{
    return {m[0][0]};
}
template <int C, int R, class ScalarT, class = enable_if<min(C, R) == 2>>
[[nodiscard]] constexpr vec<2, ScalarT> diag(mat<C, R, ScalarT> const& m)
{
    return {m[0][0], m[1][1]};
}
template <int C, int R, class ScalarT, class = enable_if<min(C, R) == 3>>
[[nodiscard]] constexpr vec<3, ScalarT> diag(mat<C, R, ScalarT> const& m)
{
    return {m[0][0], m[1][1], m[2][2]};
}
template <int C, int R, class ScalarT, class = enable_if<min(C, R) == 4>>
[[nodiscard]] constexpr vec<4, ScalarT> diag(mat<C, R, ScalarT> const& m)
{
    return {m[0][0], m[1][1], m[2][2], m[3][3]};
}
} // namespace tg
