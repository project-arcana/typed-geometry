#pragma once

#include <cmath>

#include <typed-geometry/types/angle.hh>

#include <typed-geometry/detail/errors.hh>
#include <typed-geometry/detail/macros.hh>

namespace tg
{
template <class T>
static constexpr angle_t<T> tau = angle_t<T>::from_radians(static_cast<T>(6.28318530717958647693));

template <class T>
static constexpr angle_t<T> pi = angle_t<T>::from_radians(static_cast<T>(3.14159265358979323846));

template <class T>
static constexpr T tau_scalar = static_cast<T>(6.28318530717958647693);

template <class T>
static constexpr T pi_scalar = static_cast<T>(3.14159265358979323846);

template <class T>
static constexpr T nan = NAN;

template <class T>
static constexpr T inf = INFINITY;


// Workaround for compiler discrepancies when using `static` or `inline` on variable template specializations
namespace detail
{
template <class T>
struct epsilon_t : type_error::unsupported_type<T>::value
{
};
template <>
struct epsilon_t<float>
{
    static constexpr float value = 1.19209290E-07F; // FLT_EPSILON
};
template <>
struct epsilon_t<double>
{
    static constexpr float value = 2.2204460492503131e-16; // DBL_EPSILON
};
}

template <class T>
static constexpr T epsilon = detail::epsilon_t<T>::value;
}
