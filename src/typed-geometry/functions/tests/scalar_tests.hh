#pragma once

#include <typed-geometry/functions/basic/scalar_math.hh>
#include <typed-geometry/types/angle.hh>
#include <typed-geometry/types/scalars/default.hh>

namespace tg
{
[[nodiscard]] constexpr bool is_approx_equal(i8 a, i8 b) { return a == b; }
[[nodiscard]] constexpr bool is_approx_equal(i16 a, i16 b) { return a == b; }
[[nodiscard]] constexpr bool is_approx_equal(i32 a, i32 b) { return a == b; }
[[nodiscard]] constexpr bool is_approx_equal(i64 a, i64 b) { return a == b; }

[[nodiscard]] constexpr bool is_approx_equal(u8 a, u8 b) { return a == b; }
[[nodiscard]] constexpr bool is_approx_equal(u16 a, u16 b) { return a == b; }
[[nodiscard]] constexpr bool is_approx_equal(u32 a, u32 b) { return a == b; }
[[nodiscard]] constexpr bool is_approx_equal(u64 a, u64 b) { return a == b; }

// TODO: f8, f16
[[nodiscard]] inline bool is_approx_equal(f32 a, f32 b)
{
    auto abs_a = abs(a);
    auto abs_b = abs(b);

    // close to eps
    if (abs_a < 1e-5f)
        return abs_b < 1e-5f;
    if (abs_b < 1e-5f)
        return abs_a < 1e-5f;

    // otherwise, different signs => unequal
    if ((a < 0) != (b < 0))
        return false;

    // otherwise have 1e-5f relative margin
    if (abs_a < abs_b)
        return abs_a > 0.9999f * abs_b;
    else
        return abs_b > 0.9999f * abs_a;
}
[[nodiscard]] inline bool is_approx_equal(f64 a, f64 b)
{
    auto abs_a = abs(a);
    auto abs_b = abs(b);

    // close to eps
    if (abs_a < 1e-14)
        return abs_b < 1e-14;
    if (abs_b < 1e-14)
        return abs_a < 1e-14;

    // otherwise, different signs => unequal
    if ((a < 0) != (b < 0))
        return false;

    // otherwise have 1e-14f relative margin
    if (abs_a < abs_b)
        return abs_a > 0.9999999999999 * abs_b;
    else
        return abs_b > 0.9999999999999 * abs_a;
}

template <class T>
[[nodiscard]] bool is_approx_equal(angle_t<T> const& a, angle_t<T> const& b)
{
    return is_approx_equal(a.radians(), b.radians());
}
}
