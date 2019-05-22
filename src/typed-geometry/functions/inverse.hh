#pragma once

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/vec.hh>
#include "determinant.hh"

namespace tg
{
template <class ScalarT>
TG_NODISCARD constexpr mat<1, 1, fractional_result<ScalarT>> inverse(mat<1, 1, ScalarT> const& m)
{
    return {{ScalarT(1.0) / m[0].x}};
}
template <class ScalarT>
TG_NODISCARD constexpr mat<2, 2, fractional_result<ScalarT>> inverse(mat<2, 2, ScalarT> const& m)
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
TG_NODISCARD constexpr mat<3, 3, fractional_result<ScalarT>> inverse(mat<3, 3, ScalarT> const& m)
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
template <class ScalarT>
constexpr mat<4, 4, fractional_result<ScalarT>> inverse(mat<4, 4, ScalarT> const& val)
{
    auto invdet = ScalarT(1.0) / determinant(val);

    mat<4, 4, fractional_result<ScalarT>> res;

    // https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    ScalarT inv[16];
    ScalarT const* m = &val[0][0];
    inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
    inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
    inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
    inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
    inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
    inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
    inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
    inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
    inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
    inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
    inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
    inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
    inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
    inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
    inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
    inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

    ScalarT* resP = &res[0][0];
    for (auto i = 0; i < 16; i++)
        resP[i] = inv[i] * invdet;

    return res;
}
} // namespace tg
