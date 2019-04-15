#pragma once

#include <cmath>

#include "../../detail/scalar_traits.hh"
#include "../../types/angle.hh"
#include "../../types/scalar.hh"
#include "../constants.hh"
#include "../utility.hh"

// TODO:
// - proper f8, f16
// - constexpr abs, sqrt, pow, ...
// - asm sqrt
// - floor, ceil, round

namespace tg
{
// ==================================================================
// Classification

TG_NODISCARD inline bool is_nan(f32 x)
{
    return std::isnan(x);
}
TG_NODISCARD inline bool is_nan(f64 x)
{
    return std::isnan(x);
}

TG_NODISCARD inline bool is_zero(f32 x)
{
    return x == 0;
}
TG_NODISCARD inline bool is_zero(f64 x)
{
    return x == 0;
}

TG_NODISCARD inline bool is_inf(f32 x)
{
    return std::isinf(x);
}
TG_NODISCARD inline bool is_inf(f64 x)
{
    return std::isinf(x);
}

TG_NODISCARD inline bool is_finite(f32 x)
{
    return std::isfinite(x);
}
TG_NODISCARD inline bool is_finite(f64 x)
{
    return std::isfinite(x);
}

TG_NODISCARD inline bool is_normal(f32 x)
{
    return std::isnormal(x);
}
TG_NODISCARD inline bool is_normal(f64 x)
{
    return std::isnormal(x);
}

TG_NODISCARD inline bool is_subnormal(f32 x)
{
    return is_finite(x) && !is_normal(x);
}
TG_NODISCARD inline bool is_subnormal(f64 x)
{
    return is_finite(x) && !is_normal(x);
}

enum class fp_class
{
    infinite,
    nan,
    normal,
    subnormal,
    zero
};

TG_NODISCARD inline fp_class fp_classify(f32 x)
{
    if (is_nan(x))
        return fp_class::nan;
    else if (is_inf(x))
        return fp_class::infinite;
    else if (is_zero(x))
        return fp_class::zero;
    else if (is_normal(x))
        return fp_class::normal;
    else
        return fp_class::subnormal;
}
TG_NODISCARD inline fp_class fp_classify(f64 x)
{
    if (is_nan(x))
        return fp_class::nan;
    else if (is_inf(x))
        return fp_class::infinite;
    else if (is_zero(x))
        return fp_class::zero;
    else if (is_normal(x))
        return fp_class::normal;
    else
        return fp_class::subnormal;
}

// ==================================================================
// Basics

TG_NODISCARD inline i8 abs(i8 v)
{
    return v < 0 ? -v : v;
}
TG_NODISCARD inline i16 abs(i16 v)
{
    return v < 0 ? -v : v;
}
TG_NODISCARD inline i32 abs(i32 v)
{
    return v < 0 ? -v : v;
}
TG_NODISCARD inline i64 abs(i64 v)
{
    return v < 0 ? -v : v;
}

TG_NODISCARD inline u8 abs(u8 v)
{
    return v;
}
TG_NODISCARD inline u16 abs(u16 v)
{
    return v;
}
TG_NODISCARD inline u32 abs(u32 v)
{
    return v;
}
TG_NODISCARD inline u64 abs(u64 v)
{
    return v;
}

TG_NODISCARD inline f8 abs(f8 v)
{
    return v;
}
TG_NODISCARD inline f16 abs(f16 v)
{
    return std::abs(v);
}
TG_NODISCARD inline f32 abs(f32 v)
{
    return std::abs(v);
}
TG_NODISCARD inline f64 abs(f64 v)
{
    return std::abs(v);
}

template <class T>
TG_NODISCARD angle<T> abs(angle<T> a)
{
    return radians(abs(a.radians()));
}

TG_NODISCARD inline f32 floor(f32 v)
{
    return std::floor(v);
}
TG_NODISCARD inline f64 floor(f64 v)
{
    return std::floor(v);
}
TG_NODISCARD inline i32 ifloor(f32 v)
{
    return v >= 0 || i32(v) == v ? i32(v) : i32(v) - 1;
}
TG_NODISCARD inline i64 ifloor(f64 v)
{
    return v >= 0 || i64(v) == v ? i64(v) : i64(v) - 1;
}

TG_NODISCARD inline f32 ceil(f32 v)
{
    return std::ceil(v);
}
TG_NODISCARD inline f64 ceil(f64 v)
{
    return std::ceil(v);
}
TG_NODISCARD inline i32 iceil(f32 v)
{
    return v <= 0 || i32(v) == v ? i32(v) : i32(v) + 1;
}
TG_NODISCARD inline i64 iceil(f64 v)
{
    return v <= 0 || i64(v) == v ? i64(v) : i64(v) + 1;
}

TG_NODISCARD inline f32 round(f32 v)
{
    return std::round(v);
}
TG_NODISCARD inline f64 round(f64 v)
{
    return std::round(v);
}
TG_NODISCARD inline i32 iround(f32 v)
{
    return v >= 0 ? i32(v + 0.5f) : i32(v - 0.5f);
}
TG_NODISCARD inline i64 iround(f64 v)
{
    return v >= 0 ? i64(v + 0.5) : i64(v - 0.5);
}

TG_NODISCARD inline f32 fract(f32 v)
{
    return v - floor(v);
}
TG_NODISCARD inline f64 fract(f64 v)
{
    return v - floor(v);
}

template <class T, class = enable_if<is_scalar<T>>>
TG_NODISCARD constexpr T min(T const& a, T const& b)
{
    return a < b ? a : b;
}
template <class T, class = enable_if<is_scalar<T>>>
TG_NODISCARD constexpr T max(T const& a, T const& b)
{
    return a < b ? b : a;
}

template <class T, class = enable_if<is_scalar<T>>>
TG_NODISCARD constexpr T clamp(T const& a, T const& min_value, T const& max_value)
{
    return min(max(a, min_value), max_value);
}

// ==================================================================
// Powers

TG_NODISCARD inline f32 pow(f32 b, f32 e)
{
    return std::pow(b, e);
}
TG_NODISCARD inline f32 pow(f32 b, i32 e)
{
    return f32(std::pow(b, e));
}
TG_NODISCARD inline f64 pow(f64 b, f64 e)
{
    return std::pow(b, e);
}
TG_NODISCARD inline f64 pow(f64 b, i32 e)
{
    return std::pow(b, e);
}

template <class T, class = enable_if<has_multiplication<T>>>
TG_NODISCARD constexpr T pow2(T const& v)
{
    return v * v;
}
template <class T, class = enable_if<has_multiplication<T>>>
TG_NODISCARD constexpr T pow3(T const& v)
{
    return v * v * v;
}
template <class T, class = enable_if<has_multiplication<T>>>
TG_NODISCARD constexpr T pow4(T const& v)
{
    auto v2 = v * v;
    return v2 * v2;
}
template <class T, class = enable_if<has_multiplication<T>>>
TG_NODISCARD constexpr T pow5(T const& v)
{
    auto v2 = v * v;
    return v2 * v2 * v;
}

TG_NODISCARD inline f32 sqrt(f32 v)
{
    return std::sqrt(v);
}
TG_NODISCARD inline f64 sqrt(f64 v)
{
    return std::sqrt(v);
}

TG_NODISCARD inline f32 cbrt(f32 v)
{
    return std::cbrt(v);
}
TG_NODISCARD inline f64 cbrt(f64 v)
{
    return std::cbrt(v);
}

// ==================================================================
// Exponentials

TG_NODISCARD inline f32 exp(f32 v)
{
    return std::exp(v);
}
TG_NODISCARD inline f64 exp(f64 v)
{
    return std::exp(v);
}

TG_NODISCARD inline f32 exp2(f32 v)
{
    return std::exp2(v);
}
TG_NODISCARD inline f64 exp2(f64 v)
{
    return std::exp2(v);
}

TG_NODISCARD inline f32 log(f32 v)
{
    return std::log(v);
}
TG_NODISCARD inline f64 log(f64 v)
{
    return std::log(v);
}

TG_NODISCARD inline f32 log2(f32 v)
{
    return std::log2(v);
}
TG_NODISCARD inline f64 log2(f64 v)
{
    return std::log2(v);
}

TG_NODISCARD inline f32 log10(f32 v)
{
    return std::log10(v);
}
TG_NODISCARD inline f64 log10(f64 v)
{
    return std::log10(v);
}

// ==================================================================
// Trigonometry

TG_NODISCARD inline f32 sin(angle<f32> v)
{
    return std::sin(v.radians());
}
TG_NODISCARD inline f64 sin(angle<f64> v)
{
    return std::sin(v.radians());
}
TG_NODISCARD inline f32 cos(angle<f32> v)
{
    return std::cos(v.radians());
}
TG_NODISCARD inline f64 cos(angle<f64> v)
{
    return std::cos(v.radians());
}
TG_NODISCARD inline f32 tan(angle<f32> v)
{
    return std::tan(v.radians());
}
TG_NODISCARD inline f64 tan(angle<f64> v)
{
    return std::tan(v.radians());
}

TG_NODISCARD inline angle<f32> asin(f32 v)
{
    return radians(std::asin(v));
}
TG_NODISCARD inline angle<f64> asin(f64 v)
{
    return radians(std::asin(v));
}
TG_NODISCARD inline angle<f32> acos(f32 v)
{
    return radians(std::acos(v));
}
TG_NODISCARD inline angle<f64> acos(f64 v)
{
    return radians(std::acos(v));
}
TG_NODISCARD inline angle<f32> atan(f32 v)
{
    return radians(std::atan(v));
}
TG_NODISCARD inline angle<f64> atan(f64 v)
{
    return radians(std::atan(v));
}

TG_NODISCARD inline angle<f32> atan2(f32 y, f32 x)
{
    return radians(std::atan2(y, x));
}
TG_NODISCARD inline angle<f64> atan2(f64 y, f64 x)
{
    return radians(std::atan2(y, x));
}

TG_NODISCARD inline f32 sinh(f32 v)
{
    return std::sinh(v);
}
TG_NODISCARD inline f64 sinh(f64 v)
{
    return std::sinh(v);
}
TG_NODISCARD inline f32 cosh(f32 v)
{
    return std::cosh(v);
}
TG_NODISCARD inline f64 cosh(f64 v)
{
    return std::cosh(v);
}
TG_NODISCARD inline f32 tanh(f32 v)
{
    return std::tanh(v);
}
TG_NODISCARD inline f64 tanh(f64 v)
{
    return std::tanh(v);
}

TG_NODISCARD inline f32 asinh(f32 v)
{
    return std::asinh(v);
}
TG_NODISCARD inline f64 asinh(f64 v)
{
    return std::asinh(v);
}
TG_NODISCARD inline f32 acosh(f32 v)
{
    return std::acosh(v);
}
TG_NODISCARD inline f64 acosh(f64 v)
{
    return std::acosh(v);
}
TG_NODISCARD inline f32 atanh(f32 v)
{
    return std::atanh(v);
}
TG_NODISCARD inline f64 atanh(f64 v)
{
    return std::atanh(v);
}

} // namespace tg
