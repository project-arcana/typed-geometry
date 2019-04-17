#pragma once

#include <typed-geometry/types/mat.hh>

#include "submatrix.hh"

namespace tg
{
template <class ScalarT>
TG_NODISCARD constexpr ScalarT determinant(mat<1, 1, ScalarT> const& a)
{
    return a[0].x;
}
template <class ScalarT>
TG_NODISCARD constexpr ScalarT determinant(mat<2, 2, ScalarT> const& a)
{
    return a[0].x * a[1].y - a[0].y * a[1].x;
}
template <class ScalarT>
TG_NODISCARD constexpr ScalarT determinant(mat<3, 3, ScalarT> const& a)
{
    return a[0].x * (a[1].y * a[2].z - a[2].y * a[1].z) + //
           a[1].x * (a[2].y * a[0].z - a[0].y * a[2].z) + //
           a[2].x * (a[0].y * a[1].z - a[0].z * a[1].y);
}
// template <class ScalarT>
// constexpr ScalarT determinant(mat<4, 4, ScalarT> const& a)
// {
//     // TODO
// }
} // namespace tg
