#pragma once

#include <typed-geometry/types/angle.hh>

namespace tg
{
namespace literals
{
constexpr angle32 operator""_deg(u64 v) { return angle32::from_degree(f32(v)); }
constexpr angle64 operator""_deg(long double v) { return angle64::from_degree(f64(v)); }
constexpr angle32 operator""_degf(long double v) { return angle32::from_degree(f32(v)); }

constexpr angle32 operator""_rad(u64 v) { return angle32::from_radians(f32(v)); }
constexpr angle64 operator""_rad(long double v) { return angle64::from_radians(f64(v)); }
constexpr angle32 operator""_radf(long double v) { return angle32::from_radians(f32(v)); }
} // namespace literals
} // namespace tg

#ifdef TG_EXPORT_LITERALS
using namespace tg::literals;
#endif
