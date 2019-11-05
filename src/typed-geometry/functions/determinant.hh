#pragma once

#include <typed-geometry/types/mat.hh>

#include "submatrix.hh"

namespace tg
{
template <class ScalarT>
[[nodiscard]] constexpr ScalarT determinant(mat<1, 1, ScalarT> const& a)
{
    return a[0].x;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT determinant(mat<2, 2, ScalarT> const& a)
{
    return a[0].x * a[1].y - a[0].y * a[1].x;
}
template <class ScalarT>
[[nodiscard]] constexpr ScalarT determinant(mat<3, 3, ScalarT> const& a)
{
    return a[0].x * (a[1].y * a[2].z - a[2].y * a[1].z) + //
           a[1].x * (a[2].y * a[0].z - a[0].y * a[2].z) + //
           a[2].x * (a[0].y * a[1].z - a[0].z * a[1].y);
}
template <class ScalarT>
constexpr ScalarT determinant(mat<4, 4, ScalarT> const& a)
{
    return a[0][3] * a[1][2] * a[2][1] * a[3][0] - a[0][2] * a[1][3] * a[2][1] * a[3][0] - a[0][3] * a[1][1] * a[2][2] * a[3][0]
           + a[0][1] * a[1][3] * a[2][2] * a[3][0] + a[0][2] * a[1][1] * a[2][3] * a[3][0] - a[0][1] * a[1][2] * a[2][3] * a[3][0]
           - a[0][3] * a[1][2] * a[2][0] * a[3][1] + a[0][2] * a[1][3] * a[2][0] * a[3][1] + a[0][3] * a[1][0] * a[2][2] * a[3][1]
           - a[0][0] * a[1][3] * a[2][2] * a[3][1] - a[0][2] * a[1][0] * a[2][3] * a[3][1] + a[0][0] * a[1][2] * a[2][3] * a[3][1]
           + a[0][3] * a[1][1] * a[2][0] * a[3][2] - a[0][1] * a[1][3] * a[2][0] * a[3][2] - a[0][3] * a[1][0] * a[2][1] * a[3][2]
           + a[0][0] * a[1][3] * a[2][1] * a[3][2] + a[0][1] * a[1][0] * a[2][3] * a[3][2] - a[0][0] * a[1][1] * a[2][3] * a[3][2]
           - a[0][2] * a[1][1] * a[2][0] * a[3][3] + a[0][1] * a[1][2] * a[2][0] * a[3][3] + a[0][2] * a[1][0] * a[2][1] * a[3][3]
           - a[0][0] * a[1][2] * a[2][1] * a[3][3] - a[0][1] * a[1][0] * a[2][2] * a[3][3] + a[0][0] * a[1][1] * a[2][2] * a[3][3];
}

template <int D, class ScalarT>
constexpr ScalarT det(mat<D, D, ScalarT> const& a)
{
    return determinant(a);
}
} // namespace tg
