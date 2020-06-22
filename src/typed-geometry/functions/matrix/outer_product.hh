#pragma once

#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <class ScalarT>
[[nodiscard]] constexpr mat<1, 1, ScalarT> outer_product(vec<1, ScalarT> const& lhs, vec<1, ScalarT> const& rhs)
{
    mat<1, 1, ScalarT> res;
    res[0] = lhs * rhs[0];
    return res;
}
template <class ScalarT>
[[nodiscard]] constexpr mat<2, 2, ScalarT> outer_product(vec<2, ScalarT> const& lhs, vec<2, ScalarT> const& rhs)
{
    mat<2, 2, ScalarT> res;
    res[0] = lhs * rhs[0];
    res[1] = lhs * rhs[1];
    return res;
}
template <class ScalarT>
[[nodiscard]] constexpr mat<3, 3, ScalarT> outer_product(vec<3, ScalarT> const& lhs, vec<3, ScalarT> const& rhs)
{
    mat<3, 3, ScalarT> res;
    res[0] = lhs * rhs[0];
    res[1] = lhs * rhs[1];
    res[2] = lhs * rhs[2];
    return res;
}
template <class ScalarT>
[[nodiscard]] constexpr mat<4, 4, ScalarT> outer_product(vec<4, ScalarT> const& lhs, vec<4, ScalarT> const& rhs)
{
    mat<4, 4, ScalarT> res;
    res[0] = lhs * rhs[0];
    res[1] = lhs * rhs[1];
    res[2] = lhs * rhs[2];
    res[3] = lhs * rhs[3];
    return res;
}

template <class T>
tg::mat<1, 1, T> self_outer_product(tg::vec<1, T> const& v)
{
    tg::mat<1, 1, T> M;
    M[0][0] = v[0] * v[0];
    return M;
}
template <class T>
tg::mat<2, 2, T> self_outer_product(tg::vec<2, T> const& v)
{
    auto const a = v.x;
    auto const b = v.y;

    tg::mat<2, 2, T> M;

    M[0][0] = a * a;
    M[1][1] = b * b;

    M[1][0] = M[0][1] = a * b;

    return M;
}
template <class T>
tg::mat<3, 3, T> self_outer_product(tg::vec<3, T> const& v)
{
    auto const a = v.x;
    auto const b = v.y;
    auto const c = v.z;

    tg::mat<3, 3, T> M;

    M[0][0] = a * a;
    M[1][1] = b * b;
    M[2][2] = c * c;

    M[1][0] = M[0][1] = a * b;
    M[2][0] = M[0][2] = a * c;
    M[2][1] = M[1][2] = b * c;

    return M;
}
template <class T>
tg::mat<4, 4, T> self_outer_product(tg::vec<4, T> const& v)
{
    auto const a = v.x;
    auto const b = v.y;
    auto const c = v.z;
    auto const d = v.w;

    tg::mat<4, 4, T> M;

    M[0][0] = a * a;
    M[1][1] = b * b;
    M[2][2] = c * c;
    M[3][3] = d * d;

    M[1][0] = M[0][1] = a * b;
    M[2][0] = M[0][2] = a * c;
    M[2][1] = M[1][2] = b * c;
    M[3][0] = M[0][3] = a * d;
    M[3][1] = M[1][3] = b * d;
    M[3][2] = M[2][3] = c * d;

    return M;
}
} // namespace tg
