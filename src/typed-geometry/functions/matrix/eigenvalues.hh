#pragma once

#include <typed-geometry/types/array.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/functions/matrix/transpose.hh>

namespace tg
{
template <int D, class ScalarT>
struct eigen_decomposition_result
{
    vec<D, ScalarT> eigenvector;
    ScalarT eigenvalue;
};

template <class ScalarT>
[[nodiscard]] constexpr array<eigen_decomposition_result<2, ScalarT>, 2> eigen_decomposition(mat<2, 2, ScalarT> const& m)
{
    // todo
}

template <class ScalarT>
[[nodiscard]] constexpr array<eigen_decomposition_result<3, ScalarT>, 3> eigen_decomposition(mat<3, 3, ScalarT> const& m);

template <class ScalarT>
[[nodiscard]] constexpr array<eigen_decomposition_result<4, ScalarT>, 4> eigen_decomposition(mat<4, 4, ScalarT> const& m);

template <>
[[nodiscard]] inline constexpr array<eigen_decomposition_result<3, float>, 3> eigen_decomposition(mat<3, 3, float> const& m);

template <>
[[nodiscard]] inline constexpr array<eigen_decomposition_result<4, float>, 4> eigen_decomposition(mat<4, 4, float> const& m);

template <>
[[nodiscard]] inline constexpr array<eigen_decomposition_result<3, double>, 3> eigen_decomposition(mat<3, 3, double> const& m);

template <>
[[nodiscard]] inline constexpr array<eigen_decomposition_result<4, double>, 4> eigen_decomposition(mat<4, 4, double> const& m);

template <class ScalarT>
[[nodiscard]] constexpr array<ScalarT, 2> eigenvalues(mat<2, 2, ScalarT> const& m)
{
    auto mp_half = (m[0][0] + m[1][1]) / ScalarT(2);
    auto q = m[0][0] * m[1][1] - m[1][0] * m[0][1];
    auto d = sqrt(max(ScalarT(0), mp_half * mp_half - q));
    return {mp_half + d, mp_half - d};
}


template <class ScalarT>
[[nodiscard]] constexpr array<ScalarT, 3> eigenvalues(mat<3, 3, ScalarT> const& m)
{
    // todo
}

template <class ScalarT>
[[nodiscard]] constexpr array<ScalarT, 4> eigenvalues(mat<4, 4, ScalarT> const& m)
{
    // todo
}

template <class ScalarT>
[[nodiscard]] constexpr array<vec<2, ScalarT>, 2> eigenvectors(mat<2, 2, ScalarT> const& m)
{
    // todo
}

template <class ScalarT>
[[nodiscard]] constexpr array<vec<3, ScalarT>, 3> eigenvectors(mat<3, 3, ScalarT> const& m)
{
    // todo
}

template <class ScalarT>
[[nodiscard]] constexpr array<vec<4, ScalarT>, 4> eigenvectors(mat<4, 4, ScalarT> const& m)
{
    // todo
}

template <class ScalarT>
[[nodiscard]] constexpr array<ScalarT, 2> singular_values(mat<2, 2, ScalarT> const& m)
{
    auto [s0, s1] = eigenvalues(m * transpose(m));
    return {sqrt(s0), sqrt(s1)};
}

template <class ScalarT>
[[nodiscard]] constexpr array<ScalarT, 3> singular_values(mat<3, 3, ScalarT> const& m)
{
    // todo
}

template <class ScalarT>
[[nodiscard]] constexpr array<ScalarT, 4> singular_values(mat<4, 4, ScalarT> const& m)
{
    // todo
}


}
