#pragma once

#include <typed-geometry/detail/macros.hh>

namespace tg
{
/// forward-mode automatic differentiation using dual numbers
/// see https://en.wikipedia.org/wiki/Automatic_differentiation#Automatic_differentiation_using_dual_numbers
/// use #include <typed-geometry/feature/fwd_diff.hh> for all features
template <class T>
struct fwd_diff;

using fd32 = fwd_diff<float>;
using fd64 = fwd_diff<double>;

template <class T>
struct fwd_diff
{
    T value = {};
    T derivative = {};

    constexpr fwd_diff() = default;
    explicit constexpr fwd_diff(T value) : value(value), derivative(0) {}
    constexpr fwd_diff(T value, T derivative) : value(value), derivative(derivative) {}

    TG_NODISCARD static constexpr fwd_diff input(T value) { return {value, T(1)}; }
    TG_NODISCARD static constexpr fwd_diff constant(T value) { return {value, T(0)}; }

    TG_NODISCARD constexpr bool operator==(fwd_diff const& rhs) const { return value == rhs.value && derivative == rhs.derivative; }
    TG_NODISCARD constexpr bool operator!=(fwd_diff const& rhs) const { return value != rhs.value || derivative != rhs.derivative; }
    TG_NODISCARD constexpr bool operator==(T const& rhs) const { return value == rhs; }
    TG_NODISCARD constexpr bool operator!=(T const& rhs) const { return value != rhs; }
};

template <class T>
TG_NODISCARD constexpr fwd_diff<T> fwd_diff_input(T value)
{
    return fwd_diff<T>::input(value);
}
}
