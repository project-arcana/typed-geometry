#pragma once

#include <limits>

#include "types.hh"

namespace tg
{
template <class T>
constexpr T pi = static_cast<T>(3.14159265358979323846);

// TODO: proper min vs. lowest
template <class T>
constexpr T min = std::numeric_limits<T>::min();

template <class T>
constexpr T max = std::numeric_limits<T>::max();
}
