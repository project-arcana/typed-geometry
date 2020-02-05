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

    [[nodiscard]] static constexpr fwd_diff input(T value) { return {value, T(1)}; }
    [[nodiscard]] static constexpr fwd_diff constant(T value) { return {value, T(0)}; }

    [[nodiscard]] constexpr bool operator==(fwd_diff const& rhs) const { return value == rhs.value && derivative == rhs.derivative; }
    [[nodiscard]] constexpr bool operator!=(fwd_diff const& rhs) const { return value != rhs.value || derivative != rhs.derivative; }
    [[nodiscard]] constexpr bool operator==(T const& rhs) const { return value == rhs; }
    [[nodiscard]] constexpr bool operator!=(T const& rhs) const { return value != rhs; }
};

template <class T>
[[nodiscard]] constexpr fwd_diff<T> fwd_diff_input(T value)
{
    return fwd_diff<T>::input(value);
}

template <class I, class T>
constexpr void introspect(I&& i, fwd_diff<T>& v)
{
    i(v.value, "value");
    i(v.derivative, "derivative");
}
}
