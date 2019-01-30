#pragma once

#include "../../types/mat.hh"
#include "../../types/pos.hh"
#include "../special_values.hh"

namespace tg
{
// mat * vec (of same dimension)
template <int C, class ScalarT>
constexpr vec<1, ScalarT> operator*(mat<C, 1, ScalarT> const& m, vec<C, ScalarT> const& v)
{
    return {dot(m.row(0), v)};
}
template <int C, class ScalarT>
constexpr vec<2, ScalarT> operator*(mat<C, 2, ScalarT> const& m, vec<C, ScalarT> const& v)
{
    return {dot(m.row(0), v), dot(m.row(1), v)};
}
template <int C, class ScalarT>
constexpr vec<3, ScalarT> operator*(mat<C, 3, ScalarT> const& m, vec<C, ScalarT> const& v)
{
    return {dot(m.row(0), v), dot(m.row(1), v), dot(m.row(2), v)};
}
template <int C, class ScalarT>
constexpr vec<4, ScalarT> operator*(mat<C, 4, ScalarT> const& m, vec<C, ScalarT> const& v)
{
    return {dot(m.row(0), v), dot(m.row(1), v), dot(m.row(2), v), dot(m.row(3), v)};
}

// mat * vec (with mat dimension + 1, e.g. treats vec3 as vec4(..., 0))
template <int C, class ScalarT>
constexpr vec<1, ScalarT> operator*(mat<C, 1, ScalarT> const& m, vec<C - 1, ScalarT> const& v)
{
    return {dot(vec<C - 1, ScalarT>(m.row(0)), v)};
}
template <int C, class ScalarT>
constexpr vec<2, ScalarT> operator*(mat<C, 2, ScalarT> const& m, vec<C - 1, ScalarT> const& v)
{
    return {dot(vec<C - 1, ScalarT>(m.row(0)), v), dot(vec<C - 1, ScalarT>(m.row(1)), v)};
}
template <int C, class ScalarT>
constexpr vec<3, ScalarT> operator*(mat<C, 3, ScalarT> const& m, vec<C - 1, ScalarT> const& v)
{
    return {dot(vec<C - 1, ScalarT>(m.row(0)), v), dot(vec<C - 1, ScalarT>(m.row(1)), v), dot(vec<C - 1, ScalarT>(m.row(2)), v)};
}
template <int C, class ScalarT>
constexpr vec<4, ScalarT> operator*(mat<C, 4, ScalarT> const& m, vec<C - 1, ScalarT> const& v)
{
    return {dot(vec<C - 1, ScalarT>(m.row(0)), v), dot(vec<C - 1, ScalarT>(m.row(1)), v), dot(vec<C - 1, ScalarT>(m.row(2)), v),
            dot(vec<C - 1, ScalarT>(m.row(3)), v)};
}

// mat * pos (with mat dimension + 1, e.g. treats pos3 as vec4(..., 1))
template <int D, class ScalarT>
constexpr pos<D - 1, ScalarT> operator*(mat<D, 2, ScalarT> const& m, pos<D - 1, ScalarT> const& p)
{
    auto v = m * vec<D, ScalarT>(p - zero<pos<D - 1, ScalarT>>(), ScalarT(1));
    auto r = pos<D - 1, ScalarT>(v + zero<pos<D - 1, ScalarT>>());
    return r / v[D];
}

// mat * mat
template <int A, int B, class ScalarT>
constexpr mat<1, A, ScalarT> operator*(mat<B, A, ScalarT> const& a, mat<1, B, ScalarT> const& b)
{
    mat<1, A, ScalarT> m;
    m[0] = a * b[0];
    return m;
}
template <int A, int B, class ScalarT>
constexpr mat<2, A, ScalarT> operator*(mat<B, A, ScalarT> const& a, mat<2, B, ScalarT> const& b)
{
    mat<2, A, ScalarT> m;
    m[0] = a * b[0];
    m[1] = a * b[1];
    return m;
}
template <int A, int B, class ScalarT>
constexpr mat<3, A, ScalarT> operator*(mat<B, A, ScalarT> const& a, mat<3, B, ScalarT> const& b)
{
    mat<3, A, ScalarT> m;
    m[0] = a * b[0];
    m[1] = a * b[1];
    m[2] = a * b[2];
    return m;
}
template <int A, int B, class ScalarT>
constexpr mat<4, A, ScalarT> operator*(mat<B, A, ScalarT> const& a, mat<4, B, ScalarT> const& b)
{
    mat<4, A, ScalarT> m;
    m[0] = a * b[0];
    m[1] = a * b[1];
    m[2] = a * b[2];
    m[3] = a * b[3];
    return m;
}

} // namespace tg
