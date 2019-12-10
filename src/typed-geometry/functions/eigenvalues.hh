#pragma once

#include <typed-geometry/types/comp.hh>
#include <typed-geometry/types/mat.hh>

#include <typed-geometry/common/scalar_math.hh>

#include <typed-geometry/functions/transpose.hh>

namespace tg
{
template <class T>
[[nodiscard]] constexpr comp<2, T> eigenvalues(mat<2, 2, T> const& m)
{
    auto mp_half = (m[0][0] + m[1][1]) / T(2);
    auto q = m[0][0] * m[1][1] - m[1][0] * m[0][1];
    auto d = sqrt(max(T(0), mp_half * mp_half - q));
    return {mp_half + d, mp_half - d};
}

template <class T>
[[nodiscard]] constexpr comp<2, T> singular_values(mat<2, 2, T> const& m)
{
    auto [s0, s1] = eigenvalues(m * transpose(m));
    return {sqrt(s0), sqrt(s1)};
}
}
