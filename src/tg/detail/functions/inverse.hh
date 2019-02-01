#pragma once

#include "../../types/mat.hh"
#include "../../types/vec.hh"
#include "../scalar_traits.hh"
#include "determinant.hh"

namespace tg
{
template <class ScalarT>
mat<1, 1, fractional_result<ScalarT>> inverse(mat<1, 1, ScalarT> const& m)
{
    return {ScalarT(1.0) / m[0].x};
}
template <class ScalarT>
mat<2, 2, fractional_result<ScalarT>> inverse(mat<2, 2, ScalarT> const& m)
{
    auto invdet = ScalarT(1.0) / determinant(m);

    mat<2, 2, fractional_result<ScalarT>> res;
    res[0][0] = +m[1][1] * invdet;
    res[0][1] = -m[0][1] * invdet;
    res[1][0] = -m[1][0] * invdet;
    res[1][1] = +m[0][0] * invdet;
    return res;
}
template <class ScalarT>
mat<3, 3, fractional_result<ScalarT>> inverse(mat<3, 3, ScalarT> const& m)
{
    auto invdet = ScalarT(1.0) / determinant(m);

    mat<3, 3, fractional_result<ScalarT>> res;
    res[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]) * invdet;
    res[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * invdet;
    res[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * invdet;
    res[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * invdet;
    res[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * invdet;
    res[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * invdet;
    res[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]) * invdet;
    res[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * invdet;
    res[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * invdet;
    return res;
}
// template <class ScalarT>
// mat<4, 4, fractional_result<ScalarT>> inverse(mat<4, 4, ScalarT> const& m)
// {
//     auto invdet = ScalarT(1.0) / determinant(m);
//
//     mat<4, 4, fractional_result<ScalarT>> res;
//     ... TODO ...
//     return res;
// }
} // namespace tg
