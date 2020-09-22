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

namespace detail
{
array<eigen_decomposition_result<2, float>, 2> eigen_decomposition_impl(mat<2, 2, float> const& m);
array<eigen_decomposition_result<2, double>, 2> eigen_decomposition_impl(mat<2, 2, double> const& m);
array<eigen_decomposition_result<3, float>, 3> eigen_decomposition_impl(mat<3, 3, float> const& m);
array<eigen_decomposition_result<3, double>, 3> eigen_decomposition_impl(mat<3, 3, double> const& m);
array<eigen_decomposition_result<4, float>, 4> eigen_decomposition_impl(mat<4, 4, float> const& m);
array<eigen_decomposition_result<4, double>, 4> eigen_decomposition_impl(mat<4, 4, double> const& m);

array<float, 2> eigenvalues_impl(mat<2, 2, float> const& m);
array<double, 2> eigenvalues_impl(mat<2, 2, double> const& m);
array<float, 3> eigenvalues_impl(mat<3, 3, float> const& m);
array<double, 3> eigenvalues_impl(mat<3, 3, double> const& m);
array<float, 4> eigenvalues_impl(mat<4, 4, float> const& m);
array<double, 4> eigenvalues_impl(mat<4, 4, double> const& m);

array<vec<2, float>, 2> eigenvectors_impl(mat<2, 2, float> const& m);
array<vec<2, double>, 2> eigenvectors_impl(mat<2, 2, double> const& m);
array<vec<3, float>, 3> eigenvectors_impl(mat<3, 3, float> const& m);
array<vec<3, double>, 3> eigenvectors_impl(mat<3, 3, double> const& m);
array<vec<4, float>, 4> eigenvectors_impl(mat<4, 4, float> const& m);
array<vec<4, double>, 4> eigenvectors_impl(mat<4, 4, double> const& m);
}

template <class ScalarT, int D>
[[nodiscard]] array<eigen_decomposition_result<D, ScalarT>, D> eigen_decomposition_symmetric(mat<D, D, ScalarT> const& m)
{
    if constexpr (D == 1)
    {
        return {m[0], m[0]};
    }
    else if constexpr (D == 2)
    {
        static_assert(std::is_same_v<ScalarT, float> || std::is_same_v<ScalarT, double>, "currently only suports float or double");
        return detail::eigen_decomposition_impl(m);
    }
    else if constexpr (D == 3)
    {
        static_assert(std::is_same_v<ScalarT, float> || std::is_same_v<ScalarT, double>, "currently only suports float or double");
        return detail::eigen_decomposition_impl(m);
    }
    else if constexpr (D == 4)
    {
        static_assert(std::is_same_v<ScalarT, float> || std::is_same_v<ScalarT, double>, "currently only suports float or double");
        return detail::eigen_decomposition_impl(m);
    }
    else
    {
        static_assert(tg::always_false<D>, "unsupported dimension");
        return {};
    }
}

template <class ScalarT, int D>
[[nodiscard]] array<ScalarT, D> eigenvalues_symmetric(mat<D, D, ScalarT> const& m)
{
    if constexpr (D == 1)
    {
        return {m[0], m[0]};
    }
    else if constexpr (D == 2)
    {
        auto mp_half = (m[0][0] + m[1][1]) / ScalarT(2);
        auto q = m[0][0] * m[1][1] - m[1][0] * m[0][1];
        auto d = sqrt(max(ScalarT(0), mp_half * mp_half - q));
        return {mp_half + d, mp_half - d};
    }
    else if constexpr (D == 3)
    {
        static_assert(std::is_same_v<ScalarT, float> || std::is_same_v<ScalarT, double>, "currently only suports float or double");
        return detail::eigenvalues_impl(m);
    }
    else if constexpr (D == 4)
    {
        static_assert(std::is_same_v<ScalarT, float> || std::is_same_v<ScalarT, double>, "currently only suports float or double");
        return detail::eigenvalues_impl(m);
    }
    else
    {
        static_assert(tg::always_false<D>, "unsupported dimension");
        return {};
    }
}

template <class ScalarT, int D>
[[nodiscard]] array<vec<D, ScalarT>, D> eigenvectors_symmetric(mat<D, D, ScalarT> const& m)
{
    if constexpr (D == 1)
    {
        return {m[0], m[0]};
    }
    else if constexpr (D == 2)
    {
        static_assert(std::is_same_v<ScalarT, float> || std::is_same_v<ScalarT, double>, "currently only suports float or double");
        return detail::eigenvectors_impl(m);
    }
    else if constexpr (D == 3)
    {
        static_assert(std::is_same_v<ScalarT, float> || std::is_same_v<ScalarT, double>, "currently only suports float or double");
        return detail::eigenvectors_impl(m);
    }
    else if constexpr (D == 4)
    {
        static_assert(std::is_same_v<ScalarT, float> || std::is_same_v<ScalarT, double>, "currently only suports float or double");
        return detail::eigenvectors_impl(m);
    }
    else
    {
        static_assert(tg::always_false<D>, "unsupported dimension");
        return {};
    }
}

template <class ScalarT, int D>
[[nodiscard]] array<ScalarT, D> singular_values(mat<D, D, ScalarT> const& m)
{
    if constexpr (D == 1)
    {
        return {m[0]};
    }
    else if constexpr (D == 2)
    {
        auto [s0, s1] = eigenvalues(m * transpose(m));
        return {sqrt(s0), sqrt(s1)};
    }
    else if constexpr (D == 3)
    {
        auto [s0, s1, s2] = eigenvalues(m * transpose(m));
        return {sqrt(s0), sqrt(s1), sqrt(s2)};
    }
    else if constexpr (D == 4)
    {
        auto [s0, s1, s2, s3] = eigenvalues(m * transpose(m));
        return {sqrt(s0), sqrt(s1), sqrt(s2), sqrt(s3)};
    }
}

}
