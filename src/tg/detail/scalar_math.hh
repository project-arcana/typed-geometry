#pragma once

#include <cmath>
#include <type_traits>

#include "../detail/traits.hh"
#include "../types/scalar.hh"

// TODO:
// proper f8, f16
// constexpr sqrt, pow, ...

namespace tg
{
constexpr i8 abs(i8 v) { return v < 0 ? -v : v; }
constexpr i16 abs(i16 v) { return v < 0 ? -v : v; }
constexpr i32 abs(i32 v) { return v < 0 ? -v : v; }
constexpr i64 abs(i64 v) { return v < 0 ? -v : v; }

constexpr u8 abs(u8 v) { return v; }
constexpr u16 abs(u16 v) { return v; }
constexpr u32 abs(u32 v) { return v; }
constexpr u64 abs(u64 v) { return v; }

constexpr f8 abs(f8 v) { return v < 0 ? -v : v; }
constexpr f16 abs(f16 v) { return v < 0 ? -v : v; }
constexpr f32 abs(f32 v) { return v < 0 ? -v : v; }
constexpr f64 abs(f64 v) { return v < 0 ? -v : v; }


inline f32 sin(f32 v) { return std::sin(v); }
inline f64 sin(f64 v) { return std::sin(v); }

inline f32 cos(f32 v) { return std::cos(v); }
inline f64 cos(f64 v) { return std::cos(v); }

inline f32 sqrt(f32 v) { return std::sqrt(v); }
inline f64 sqrt(f64 v) { return std::sqrt(v); }

inline f32 pow(f32 b, f32 e) { return std::pow(b, e); }
inline f32 pow(f32 b, i32 e) { return std::pow(b, e); }
inline f64 pow(f64 b, f64 e) { return std::pow(b, e); }
inline f64 pow(f64 b, i32 e) { return std::pow(b, e); }

template <class T, class = std::enable_if_t<has_multiplication<T>>>
constexpr T pow2(T const& v)
{
    return v * v;
}
template <class T, class = std::enable_if_t<has_multiplication<T>>>
constexpr T pow3(T const& v)
{
    return v * v * v;
}
template <class T, class = std::enable_if_t<has_multiplication<T>>>
constexpr T pow4(T const& v)
{
    return (v * v) * (v * v);
}
template <class T, class = std::enable_if_t<has_multiplication<T>>>
constexpr T pow5(T const& v)
{
    return (v * v) * (v * v) * v;
}

} // namespace tg
