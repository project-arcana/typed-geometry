#pragma once

namespace tg
{
template <class T>
constexpr T tau = static_cast<T>(6.28318530717958647693);

template <class T>
constexpr T pi = static_cast<T>(3.14159265358979323846);

template <class T>
constexpr T nan = static_cast<T>(0.0 / 0.0);

// TODO: proper min vs. lowest
// template <class T>
// constexpr T min = std::numeric_limits<T>::min();

// template <class T>
// constexpr T max = std::numeric_limits<T>::max();

} // namespace tg
