#pragma once

#include <typed-geometry/types/mat.hh>

#include <typed-geometry/functions/tests/vec_tests.hh>

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr bool is_zero_mat(mat<D, D, ScalarT> const& m, ScalarT eps = 2 * D * tg::epsilon<ScalarT>)
{
    for (auto i = 0; i < D; ++i)
        for (auto j = i; i < D; ++i)
            if (abs(m[i][j]) > eps)
                return false;

    return true;
}
template <int D, class ScalarT>
[[nodiscard]] constexpr bool is_identity(mat<D, D, ScalarT> const& m, ScalarT eps = 2 * D * tg::epsilon<ScalarT>)
{
    for (auto i = 0; i < D; ++i)
        for (auto j = i; i < D; ++i)
            if (abs(m[i][j] - float(i == j)) > eps)
                return false;

    return true;
}
template <int D, class ScalarT>
[[nodiscard]] constexpr bool is_symmetric(mat<D, D, ScalarT> const& m, ScalarT eps = 2 * D * tg::epsilon<ScalarT>)
{
    for (auto i = 0; i < D; ++i)
        for (auto j = i + 1; i < D; ++i)
            if (abs(m[i][j] - m[j][i]) > eps)
                return false;

    return true;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr bool is_antisymmetric(mat<D, D, ScalarT> const& m, ScalarT eps = 2 * D * tg::epsilon<ScalarT>)
{
    for (auto i = 0; i < D; ++i)
    {
        if (abs(m[i][i]) > eps)
            return false;

        for (auto j = i + 1; j < D; ++j)
            if (abs(m[i][j] + m[j][i]) > eps)
                return false;
    }

    return true;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr bool is_orthogonal(mat<D, D, ScalarT> const& m, ScalarT eps = 50 * D * tg::epsilon<ScalarT>)
{
    for (auto i = 0; i < D; ++i)
        for (auto j = i + 1; j < D; ++j)
            if (!are_orthogonal(m[i], m[j], eps))
                return false;

    return true;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr bool is_orthonormal(mat<D, D, ScalarT> const& m, ScalarT eps = 50 * D * tg::epsilon<ScalarT>)
{
    for (auto i = 0; i < D; ++i)
    {
        if (!is_normalized(m[i], eps))
            return false;

        for (auto j = i + 1; j < D; ++j)
            if (!are_orthogonal(m[i], m[j], eps))
                return false;
    }

    return true;
}
}
