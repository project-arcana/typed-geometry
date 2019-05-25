#pragma once

#include <cmath>

#include <typed-geometry/types/angle.hh>

#include <typed-geometry/detail/errors.hh>
#include <typed-geometry/detail/macros.hh>

namespace tg
{
template <class T>
constexpr angle_t<T> tau = angle_t<T>::from_radians(static_cast<T>(6.28318530717958647693));

template <class T>
constexpr angle_t<T> pi = angle_t<T>::from_radians(static_cast<T>(3.14159265358979323846));

template <class T>
constexpr T tau_scalar = static_cast<T>(6.28318530717958647693);

template <class T>
constexpr T pi_scalar = static_cast<T>(3.14159265358979323846);

template <class T>
constexpr T nan = NAN;

template <class T>
constexpr T inf = INFINITY;

template <class T>
constexpr T epsilon = type_error::unsupported_type<T>::value;
template <>
constexpr float epsilon<float> = 1.19209290E-07F; // FLT_EPSILON
template <>
constexpr double epsilon<double> = 2.2204460492503131e-16; // DBL_EPSILON

// TODO: proper min vs. lowest
// template <class T>
// constexpr T min = std::numeric_limits<T>::min();

// template <class T>
// constexpr T max = std::numeric_limits<T>::max();

} // namespace tg
