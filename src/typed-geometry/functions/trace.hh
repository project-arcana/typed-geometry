#pragma once

#include <typed-geometry/types/mat.hh>

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr ScalarT trace(mat<D, D, ScalarT> const& A)
{
    if constexpr (D == 1)
        return A[0][0];
    else if constexpr (D == 2)
        return A[0][0] + A[1][1];
    else if constexpr (D == 3)
        return A[0][0] + A[1][1] + A[2][2];
    else if constexpr (D == 4)
        return A[0][0] + A[1][1] + A[2][2] + A[3][3];
    else
        static_assert(D <= 4, "only up to 3D supported");
}

// trace(A * B)
template <int N, int M, class ScalarT>
[[nodiscard]] constexpr ScalarT trace_of_product(mat<N, M, ScalarT> const& A, mat<M, N, ScalarT> const& B)
{
    auto r = ScalarT(0);
    for (auto i = 0; i < N; ++i)
        for (auto j = 0; j < N; ++j)
            r += A[i][j] * B[j][i];
    return r;
}
}
