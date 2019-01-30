#pragma once

#include "../../types/mat.hh"
#include "../scalars/scalar_math.hh"
#include "../utility.hh"

namespace tg
{
template <int C, int R, class ScalarT, class = enable_if<min(C, R) == 1>>
constexpr vec<1, ScalarT> diag(mat<C, R, ScalarT> const& m)
{
    return {m[0][0]};
}
template <int C, int R, class ScalarT, class = enable_if<min(C, R) == 2>>
constexpr vec<2, ScalarT> diag(mat<C, R, ScalarT> const& m)
{
    return {m[0][0], m[1][1]};
}
template <int C, int R, class ScalarT, class = enable_if<min(C, R) == 3>>
constexpr vec<3, ScalarT> diag(mat<C, R, ScalarT> const& m)
{
    return {m[0][0], m[1][1], m[2][2]};
}
template <int C, int R, class ScalarT, class = enable_if<min(C, R) == 4>>
constexpr vec<4, ScalarT> diag(mat<C, R, ScalarT> const& m)
{
    return {m[0][0], m[1][1], m[2][2], m[3][3]};
}
} // namespace tg
