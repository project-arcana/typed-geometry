#pragma once

#include <typed-geometry/common/scalar_math.hh>
#include <typed-geometry/types/quat.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
// ================================= Operators =================================

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> operator+(quat<ScalarT> const& a, quat<ScalarT> const& b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> operator-(quat<ScalarT> const& a, quat<ScalarT> const& b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> operator*(quat<ScalarT> const& a, quat<ScalarT> const& b)
{
    return {
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y, //
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x, //
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w, //
        a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w  //
    };
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> operator*(quat<ScalarT> const& a, dont_deduce<ScalarT> b)
{
    return {a.x * b, a.y * b, a.z * b, a.w * b};
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> operator*(dont_deduce<ScalarT> a, quat<ScalarT> const& b)
{
    return {a * b.x, a * b.y, a * b.z, a * b.w};
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> operator/(quat<ScalarT> const& a, dont_deduce<ScalarT> b)
{
    return a * (ScalarT(1) / b);
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> operator/(dont_deduce<ScalarT> a, quat<ScalarT> const& b)
{
    return {a / b.x, a / b.y, a / b.z, a / b.w};
}

// ================================= Functions =================================

template <class ScalarT>
TG_NODISCARD constexpr ScalarT length_sqr(quat<ScalarT> const& q)
{
    return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT length(quat<ScalarT> const& q)
{
    return sqrt(length_sqr(q));
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> normalize(quat<ScalarT> const& q)
{
    return q / length(q);
}

template <class ScalarT>
TG_NODISCARD constexpr quat<ScalarT> normalize_safe(quat<ScalarT> const& q, dont_deduce<ScalarT> eps = ScalarT(0))
{
    auto const l = length(q);
    return l <= eps ? quat<ScalarT>::zero : q / l;
}

template <class ScalarT>
TG_NODISCARD constexpr ScalarT real(quat<ScalarT> const& q)
{
    return q.w;
}

template <class ScalarT>
TG_NODISCARD constexpr vec<3, ScalarT> imaginary(quat<ScalarT> const& q)
{
    return {q.x, q.y, q.z};
}
}
