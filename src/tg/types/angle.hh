#pragma once

#include "scalar.hh"

namespace tg
{
template <class T>
struct angle;

using angle32 = angle<f32>;
using angle64 = angle<f64>;

/// An opaque type representing an angle
///
/// radians(3.14..) represents a 90° angle
/// degree(45)      represents a 45° angle
template <class T>
struct angle
{
private:
    T angle_in_radians = T(0);

private:
    constexpr angle(T a) : angle_in_radians(a) {}

public:
    constexpr angle() = default;

    constexpr static angle from_radians(T a) { return {a}; }
    constexpr static angle from_degree(T a) { return {a * T(0.01745329251)}; } // * pi / 180

    constexpr T radians() const { return angle_in_radians; }
    constexpr T degree() const { return angle_in_radians * T(57.2957795131); } // * 180 / pi

    constexpr bool operator==(angle<T> const& b) const { return angle_in_radians == b.angle_in_radians; }
    constexpr bool operator!=(angle<T> const& b) const { return angle_in_radians != b.angle_in_radians; }
    constexpr bool operator<(angle<T> const& b) const { return angle_in_radians < b.angle_in_radians; }
    constexpr bool operator<=(angle<T> const& b) const { return angle_in_radians <= b.angle_in_radians; }
    constexpr bool operator>(angle<T> const& b) const { return angle_in_radians > b.angle_in_radians; }
    constexpr bool operator>=(angle<T> const& b) const { return angle_in_radians >= b.angle_in_radians; }
};

inline angle<f32> radians(i32 a) { return angle<f32>::from_radians(f32(a)); }
inline angle<f32> radians(i64 a) { return angle<f32>::from_radians(f32(a)); }
inline angle<f32> radians(f32 a) { return angle<f32>::from_radians(f32(a)); }
inline angle<f64> radians(f64 a) { return angle<f64>::from_radians(f64(a)); }

inline angle<f32> degree(i32 a) { return angle<f32>::from_degree(f32(a)); }
inline angle<f32> degree(i64 a) { return angle<f32>::from_degree(f32(a)); }
inline angle<f32> degree(f32 a) { return angle<f32>::from_degree(f32(a)); }
inline angle<f64> degree(f64 a) { return angle<f64>::from_degree(f64(a)); }

template <class T>
T to_radians(angle<T> a)
{
    return a.radians();
}
template <class T>
T to_degree(angle<T> a)
{
    return a.degree();
}
} // namespace tg
