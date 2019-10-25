#pragma once

#include <typed-geometry/detail/special_values.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/pos.hh>

/*
 * Supported operations:
 *   mat * vec (of same dimension)
 *   mat * vec (with mat dimension + 1, e.g. treats vec3 as vec4(..., 0))
 *   mat * pos (with mat dimension + 1, e.g. treats pos3 as vec4(..., 1))
 *   mat * mat
 *   mat + mat
 *   mat - mat
 *   +mat
 *   -mat
 *   mat + scalar
 *   mat - scalar
 *   mat / scalar
 *   mat * scalar
 *
 * TODO: use if constexpr for some operations
 */

namespace tg
{
// mat * vec (of same dimension)
template <int C, class ScalarT>
[[nodiscard]] constexpr vec<1, ScalarT> operator*(mat<C, 1, ScalarT> const& m, vec<C, ScalarT> const& v)
{
    return {dot(m.row(0), v)};
}
template <int C, class ScalarT>
[[nodiscard]] constexpr vec<2, ScalarT> operator*(mat<C, 2, ScalarT> const& m, vec<C, ScalarT> const& v)
{
    return {dot(m.row(0), v), dot(m.row(1), v)};
}
template <int C, class ScalarT>
[[nodiscard]] constexpr vec<3, ScalarT> operator*(mat<C, 3, ScalarT> const& m, vec<C, ScalarT> const& v)
{
    return {dot(m.row(0), v), dot(m.row(1), v), dot(m.row(2), v)};
}
template <int C, class ScalarT>
[[nodiscard]] constexpr vec<4, ScalarT> operator*(mat<C, 4, ScalarT> const& m, vec<C, ScalarT> const& v)
{
    return {dot(m.row(0), v), dot(m.row(1), v), dot(m.row(2), v), dot(m.row(3), v)};
}

// mat * vec (with mat dimension + 1, e.g. treats vec3 as vec4(..., 0))
template <int C, class ScalarT>
[[nodiscard]] constexpr vec<1, ScalarT> operator*(mat<C, 2, ScalarT> const& m, vec<C - 1, ScalarT> const& v)
{
    auto w = dot(vec<C - 1, ScalarT>(m.row(1)), v);
    auto r = vec<C - 1, ScalarT>(dot(vec<C - 1, ScalarT>(m.row(0)), v));
    return w == ScalarT(0) || w == ScalarT(1) ? r : r / w;
}
template <int C, class ScalarT>
[[nodiscard]] constexpr vec<2, ScalarT> operator*(mat<C, 3, ScalarT> const& m, vec<C - 1, ScalarT> const& v)
{
    auto w = dot(vec<C - 1, ScalarT>(m.row(2)), v);
    auto r = vec<C - 1, ScalarT>(dot(vec<C - 1, ScalarT>(m.row(0)), v), dot(vec<C - 1, ScalarT>(m.row(1)), v));
    return w == ScalarT(0) || w == ScalarT(1) ? r : r / w;
}
template <int C, class ScalarT>
[[nodiscard]] constexpr vec<3, ScalarT> operator*(mat<C, 4, ScalarT> const& m, vec<C - 1, ScalarT> const& v)
{
    auto w = dot(vec<C - 1, ScalarT>(m.row(3)), v);
    auto r = vec<C - 1, ScalarT>(dot(vec<C - 1, ScalarT>(m.row(0)), v), dot(vec<C - 1, ScalarT>(m.row(1)), v), dot(vec<C - 1, ScalarT>(m.row(2)), v));
    return w == ScalarT(0) || w == ScalarT(1) ? r : r / w;
}

// mat * pos (with mat dimension + 1, e.g. treats pos3 as vec4(..., 1))
template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D - 1, ScalarT> operator*(mat<D, D, ScalarT> const& m, pos<D - 1, ScalarT> const& p)
{
    auto v = m * vec<D, ScalarT>(p - pos<D - 1, ScalarT>::zero, ScalarT(1));
    auto r = pos<D - 1, ScalarT>(pos<D - 1, ScalarT>::zero + vec<D - 1, ScalarT>(v));
    return v[D - 1] == ScalarT(1) ? r : r / v[D - 1];
}

// mat * mat
template <int A, int B, class ScalarT>
[[nodiscard]] constexpr mat<1, A, ScalarT> operator*(mat<B, A, ScalarT> const& a, mat<1, B, ScalarT> const& b)
{
    mat<1, A, ScalarT> m;
    m[0] = a * b[0];
    return m;
}
template <int A, int B, class ScalarT>
[[nodiscard]] constexpr mat<2, A, ScalarT> operator*(mat<B, A, ScalarT> const& a, mat<2, B, ScalarT> const& b)
{
    mat<2, A, ScalarT> m;
    m[0] = a * b[0];
    m[1] = a * b[1];
    return m;
}
template <int A, int B, class ScalarT>
[[nodiscard]] constexpr mat<3, A, ScalarT> operator*(mat<B, A, ScalarT> const& a, mat<3, B, ScalarT> const& b)
{
    mat<3, A, ScalarT> m;
    m[0] = a * b[0];
    m[1] = a * b[1];
    m[2] = a * b[2];
    return m;
}
template <int A, int B, class ScalarT>
[[nodiscard]] constexpr mat<4, A, ScalarT> operator*(mat<B, A, ScalarT> const& a, mat<4, B, ScalarT> const& b)
{
    mat<4, A, ScalarT> m;
    m[0] = a * b[0];
    m[1] = a * b[1];
    m[2] = a * b[2];
    m[3] = a * b[3];
    return m;
}

// mat + mat
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<1, R, ScalarT> operator+(mat<1, R, ScalarT> const& a, mat<1, R, ScalarT> const& b)
{
    mat<1, R, ScalarT> m;
    m[0] = a[0] + b[0];
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<2, R, ScalarT> operator+(mat<2, R, ScalarT> const& a, mat<2, R, ScalarT> const& b)
{
    mat<2, R, ScalarT> m;
    m[0] = a[0] + b[0];
    m[1] = a[1] + b[1];
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<3, R, ScalarT> operator+(mat<3, R, ScalarT> const& a, mat<3, R, ScalarT> const& b)
{
    mat<3, R, ScalarT> m;
    m[0] = a[0] + b[0];
    m[1] = a[1] + b[1];
    m[2] = a[2] + b[2];
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<4, R, ScalarT> operator+(mat<4, R, ScalarT> const& a, mat<4, R, ScalarT> const& b)
{
    mat<4, R, ScalarT> m;
    m[0] = a[0] + b[0];
    m[1] = a[1] + b[1];
    m[2] = a[2] + b[2];
    m[3] = a[3] + b[3];
    return m;
}

// mat - mat
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<1, R, ScalarT> operator-(mat<1, R, ScalarT> const& a, mat<1, R, ScalarT> const& b)
{
    mat<1, R, ScalarT> m;
    m[0] = a[0] - b[0];
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<2, R, ScalarT> operator-(mat<2, R, ScalarT> const& a, mat<2, R, ScalarT> const& b)
{
    mat<2, R, ScalarT> m;
    m[0] = a[0] - b[0];
    m[1] = a[1] - b[1];
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<3, R, ScalarT> operator-(mat<3, R, ScalarT> const& a, mat<3, R, ScalarT> const& b)
{
    mat<3, R, ScalarT> m;
    m[0] = a[0] - b[0];
    m[1] = a[1] - b[1];
    m[2] = a[2] - b[2];
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<4, R, ScalarT> operator-(mat<4, R, ScalarT> const& a, mat<4, R, ScalarT> const& b)
{
    mat<4, R, ScalarT> m;
    m[0] = a[0] - b[0];
    m[1] = a[1] - b[1];
    m[2] = a[2] - b[2];
    m[3] = a[3] - b[3];
    return m;
}

// +mat
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<1, R, ScalarT> operator+(mat<1, R, ScalarT> const& a)
{
    mat<1, R, ScalarT> m;
    m[0] = +a[0];
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<2, R, ScalarT> operator+(mat<2, R, ScalarT> const& a)
{
    mat<2, R, ScalarT> m;
    m[0] = +a[0];
    m[1] = +a[1];
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<3, R, ScalarT> operator+(mat<3, R, ScalarT> const& a)
{
    mat<3, R, ScalarT> m;
    m[0] = +a[0];
    m[1] = +a[1];
    m[2] = +a[2];
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<4, R, ScalarT> operator+(mat<4, R, ScalarT> const& a)
{
    mat<4, R, ScalarT> m;
    m[0] = +a[0];
    m[1] = +a[1];
    m[2] = +a[2];
    m[3] = +a[3];
    return m;
}

// -mat
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<1, R, ScalarT> operator-(mat<1, R, ScalarT> const& a)
{
    mat<1, R, ScalarT> m;
    m[0] = -a[0];
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<2, R, ScalarT> operator-(mat<2, R, ScalarT> const& a)
{
    mat<2, R, ScalarT> m;
    m[0] = -a[0];
    m[1] = -a[1];
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<3, R, ScalarT> operator-(mat<3, R, ScalarT> const& a)
{
    mat<3, R, ScalarT> m;
    m[0] = -a[0];
    m[1] = -a[1];
    m[2] = -a[2];
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<4, R, ScalarT> operator-(mat<4, R, ScalarT> const& a)
{
    mat<4, R, ScalarT> m;
    m[0] = -a[0];
    m[1] = -a[1];
    m[2] = -a[2];
    m[3] = -a[3];
    return m;
}

// TODO: is this really useful?
// mat + scalar
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<1, R, ScalarT> operator+(mat<1, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<1, R, ScalarT> m;
    m[0] = a[0] + b;
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<2, R, ScalarT> operator+(mat<2, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<2, R, ScalarT> m;
    m[0] = a[0] + b;
    m[1] = a[1] + b;
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<3, R, ScalarT> operator+(mat<3, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<3, R, ScalarT> m;
    m[0] = a[0] + b;
    m[1] = a[1] + b;
    m[2] = a[2] + b;
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<4, R, ScalarT> operator+(mat<4, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<4, R, ScalarT> m;
    m[0] = a[0] + b;
    m[1] = a[1] + b;
    m[2] = a[2] + b;
    m[3] = a[3] + b;
    return m;
}

// mat - scalar
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<1, R, ScalarT> operator-(mat<1, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<1, R, ScalarT> m;
    m[0] = a[0] - b;
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<2, R, ScalarT> operator-(mat<2, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<2, R, ScalarT> m;
    m[0] = a[0] - b;
    m[1] = a[1] - b;
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<3, R, ScalarT> operator-(mat<3, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<3, R, ScalarT> m;
    m[0] = a[0] - b;
    m[1] = a[1] - b;
    m[2] = a[2] - b;
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<4, R, ScalarT> operator-(mat<4, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<4, R, ScalarT> m;
    m[0] = a[0] - b;
    m[1] = a[1] - b;
    m[2] = a[2] - b;
    m[3] = a[3] - b;
    return m;
}

// mat * scalar
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<1, R, ScalarT> operator*(mat<1, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<1, R, ScalarT> m;
    m[0] = a[0] * b;
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<2, R, ScalarT> operator*(mat<2, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<2, R, ScalarT> m;
    m[0] = a[0] * b;
    m[1] = a[1] * b;
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<3, R, ScalarT> operator*(mat<3, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<3, R, ScalarT> m;
    m[0] = a[0] * b;
    m[1] = a[1] * b;
    m[2] = a[2] * b;
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<4, R, ScalarT> operator*(mat<4, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<4, R, ScalarT> m;
    m[0] = a[0] * b;
    m[1] = a[1] * b;
    m[2] = a[2] * b;
    m[3] = a[3] * b;
    return m;
}

// mat / scalar
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<1, R, ScalarT> operator/(mat<1, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<1, R, ScalarT> m;
    m[0] = a[0] / b;
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<2, R, ScalarT> operator/(mat<2, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<2, R, ScalarT> m;
    m[0] = a[0] / b;
    m[1] = a[1] / b;
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<3, R, ScalarT> operator/(mat<3, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<3, R, ScalarT> m;
    m[0] = a[0] / b;
    m[1] = a[1] / b;
    m[2] = a[2] / b;
    return m;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<4, R, ScalarT> operator/(mat<4, R, ScalarT> const& a, dont_deduce<ScalarT> b)
{
    mat<4, R, ScalarT> m;
    m[0] = a[0] / b;
    m[1] = a[1] / b;
    m[2] = a[2] / b;
    m[3] = a[3] / b;
    return m;
}

// scalar +-* mat
template <int C, int R, class ScalarT>
[[nodiscard]] constexpr mat<C, R, ScalarT> operator+(dont_deduce<ScalarT> a, mat<C, R, ScalarT> const& b)
{
    return b + a;
}
template <int C, int R, class ScalarT>
[[nodiscard]] constexpr mat<C, R, ScalarT> operator-(dont_deduce<ScalarT> a, mat<C, R, ScalarT> const& b)
{
    return -b + a;
}
template <int C, int R, class ScalarT>
[[nodiscard]] constexpr mat<C, R, ScalarT> operator*(dont_deduce<ScalarT> a, mat<C, R, ScalarT> const& b)
{
    return b * a;
}

} // namespace tg
