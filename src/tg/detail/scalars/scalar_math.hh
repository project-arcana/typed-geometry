#pragma once

#include <cmath>

#include "../../detail/scalar_traits.hh"
#include "../../types/scalar.hh"
#include "../utility.hh"

// TODO:
// - proper f8, f16
// - constexpr abs, sqrt, pow, ...
// - asm sqrt
// - floor, ceil, round

namespace tg
{
inline i8 abs(i8 v) { return v < 0 ? -v : v; }
inline i16 abs(i16 v) { return v < 0 ? -v : v; }
inline i32 abs(i32 v) { return v < 0 ? -v : v; }
inline i64 abs(i64 v) { return v < 0 ? -v : v; }

inline u8 abs(u8 v) { return v; }
inline u16 abs(u16 v) { return v; }
inline u32 abs(u32 v) { return v; }
inline u64 abs(u64 v) { return v; }

inline f8 abs(f8 v) { return v; }
inline f16 abs(f16 v) { return std::abs(v); }
inline f32 abs(f32 v) { return std::abs(v); }
inline f64 abs(f64 v) { return std::abs(v); }

inline f32 sin(f32 v) { return std::sin(v); }
inline f64 sin(f64 v) { return std::sin(v); }

inline f32 cos(f32 v) { return std::cos(v); }
inline f64 cos(f64 v) { return std::cos(v); }

inline f32 sqrt(f32 v) { return std::sqrt(v); }
inline f64 sqrt(f64 v) { return std::sqrt(v); }

inline f32 cbrt(f32 v) { return std::cbrt(v); }
inline f64 cbrt(f64 v) { return std::cbrt(v); }

inline f32 pow(f32 b, f32 e) { return std::pow(b, e); }
inline f32 pow(f32 b, i32 e) { return f32(std::pow(b, e)); }
inline f64 pow(f64 b, f64 e) { return std::pow(b, e); }
inline f64 pow(f64 b, i32 e) { return std::pow(b, e); }

inline f32 floor(f32 v) { return std::floor(v); }
inline f64 floor(f64 v) { return std::floor(v); }
inline i32 ifloor(f32 v) { return v >= 0 || i32(v) == v ? i32(v) : i32(v) - 1; }
inline i64 ifloor(f64 v) { return v >= 0 || i64(v) == v ? i64(v) : i64(v) - 1; }

inline f32 ceil(f32 v) { return std::ceil(v); }
inline f64 ceil(f64 v) { return std::ceil(v); }
inline i32 iceil(f32 v) { return v <= 0 || i32(v) == v ? i32(v) : i32(v) + 1; }
inline i64 iceil(f64 v) { return v <= 0 || i64(v) == v ? i64(v) : i64(v) + 1; }

inline f32 round(f32 v) { return std::round(v); }
inline f64 round(f64 v) { return std::round(v); }
inline i32 iround(f32 v) { return v >= 0 ? i32(v + 0.5f) : i32(v - 0.5f); }
inline i64 iround(f64 v) { return v >= 0 ? i64(v + 0.5) : i64(v - 0.5); }

inline f32 fract(f32 v) { return v - floor(v); }
inline f64 fract(f64 v) { return v - floor(v); }

template <class T, class = enable_if<is_scalar<T>>>
constexpr T min(T const& a, T const& b)
{
    return a < b ? a : b;
}
template <class T, class = enable_if<is_scalar<T>>>
constexpr T max(T const& a, T const& b)
{
    return a < b ? b : a;
}

template <class T, class = enable_if<has_multiplication<T>>>
constexpr T pow2(T const& v)
{
    return v * v;
}
template <class T, class = enable_if<has_multiplication<T>>>
constexpr T pow3(T const& v)
{
    return v * v * v;
}
template <class T, class = enable_if<has_multiplication<T>>>
constexpr T pow4(T const& v)
{
    auto v2 = v * v;
    return v2 * v2;
}
template <class T, class = enable_if<has_multiplication<T>>>
constexpr T pow5(T const& v)
{
    auto v2 = v * v;
    return v2 * v2 * v;
}

template <class T, class = enable_if<is_scalar<T>>>
constexpr T clamp(T const& a, T const& min_value, T const& max_value)
{
    return min(max(a, min_value), max_value);
}

} // namespace tg
