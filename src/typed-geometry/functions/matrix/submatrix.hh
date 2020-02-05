#pragma once

#include <typed-geometry/types/mat.hh>

#include <typed-geometry/functions/vector/subvector.hh>

namespace tg
{
template <int SX, int SY, int SC, int SR, int C, int R, class ScalarT>
[[nodiscard]] constexpr mat<SC, SR, ScalarT> submatrix(mat<C, R, ScalarT> const& a)
{
    static_assert(SC <= C && SR <= R, "submatrix can only make matrices smaller");
    static_assert(SX >= 0 && SY >= 0, "submatrix must start at least at zero");
    static_assert(SX + SC <= C && SY + SR <= R, "submatrix must be contained");
    mat<SC, SR, ScalarT> m;
    for (auto c = 0; c < SC; ++c)
        m[c] = subvector<SY, SR>(a[c + SX]);
    return m;
}
} // namespace tg
