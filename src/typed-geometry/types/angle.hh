#pragma once

#include <typed-geometry/types/scalars/default.hh>

namespace tg
{
template <class T>
struct angle_t;

using angle = angle_t<f32>;
using angle32 = angle_t<f32>;
using angle64 = angle_t<f64>;

/// An opaque type representing an angle
///
/// radians(3.14..) represents a 180° angle
/// degree(45)      represents a 45° angle
template <class T>
struct angle_t
{
private:
    T angle_in_radians = T(0);

private:
    constexpr angle_t(T a) : angle_in_radians(a) {}

public:
    constexpr angle_t() = default;

    template <class U>
    constexpr explicit angle_t(angle_t<U> const& other) : angle_in_radians(T(other.radians()))
    {
    }

    constexpr static angle_t from_radians(T a) { return {a}; }
    constexpr static angle_t from_degree(T a) { return {a * T(0.01745329251)}; } // * pi / 180

    constexpr T radians() const { return angle_in_radians; }
    constexpr T degree() const { return angle_in_radians * T(57.2957795131); } // * 180 / pi

    constexpr bool operator==(angle_t<T> const& b) const { return angle_in_radians == b.angle_in_radians; }
    constexpr bool operator!=(angle_t<T> const& b) const { return angle_in_radians != b.angle_in_radians; }
    constexpr bool operator<(angle_t<T> const& b) const { return angle_in_radians < b.angle_in_radians; }
    constexpr bool operator<=(angle_t<T> const& b) const { return angle_in_radians <= b.angle_in_radians; }
    constexpr bool operator>(angle_t<T> const& b) const { return angle_in_radians > b.angle_in_radians; }
    constexpr bool operator>=(angle_t<T> const& b) const { return angle_in_radians >= b.angle_in_radians; }
};

constexpr angle_t<f32> radians(i32 a) { return angle_t<f32>::from_radians(f32(a)); }
constexpr angle_t<f32> radians(i64 a) { return angle_t<f32>::from_radians(f32(a)); }
constexpr angle_t<f32> radians(f32 a) { return angle_t<f32>::from_radians(f32(a)); }
constexpr angle_t<f64> radians(f64 a) { return angle_t<f64>::from_radians(f64(a)); }

constexpr angle_t<f32> degree(i32 a) { return angle_t<f32>::from_degree(f32(a)); }
constexpr angle_t<f32> degree(i64 a) { return angle_t<f32>::from_degree(f32(a)); }
constexpr angle_t<f32> degree(f32 a) { return angle_t<f32>::from_degree(f32(a)); }
constexpr angle_t<f64> degree(f64 a) { return angle_t<f64>::from_degree(f64(a)); }

template <class T>
constexpr T to_radians(angle_t<T> const& a)
{
    return a.radians();
}
template <class T>
constexpr T to_degree(angle_t<T> const& a)
{
    return a.degree();
}

template <class I, class ScalarT>
constexpr void introspect(I&& i, angle_t<ScalarT>& v)
{
    i(v.radians(), "rad");
}
} // namespace tg
