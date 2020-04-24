#pragma once

#include <typed-geometry/detail/macros.hh>
#include <typed-geometry/feature/assert.hh>
#include <typed-geometry/functions/basic/constants.hh>

namespace tg
{
/// interval arithmetic
/// use #include <typed-geometry/feature/interval.hh> for all features
/// NOTE:
///   * intervals cannot be compared
///   * intervals represent a closed interval [min, max]
///   * unbounded intervals can use +inf and -inf (though all actual values are finite)
template <class T>
struct interval;

template <class T>
struct interval
{
    T min = {};
    T max = {};

    static const interval complete; // -inf..inf

    constexpr interval() = default;
    explicit constexpr interval(T v) : min(v), max(v) {}
    constexpr interval(T min, T max) : min(min), max(max) { TG_CONTRACT(min <= max); }

    [[nodiscard]] constexpr bool operator==(interval const& rhs) const { return min == rhs.min && max == rhs.max; }
    [[nodiscard]] constexpr bool operator!=(interval const& rhs) const { return min != rhs.min || max != rhs.max; }
};

template <class T>
const interval<T> interval<T>::complete = {-tg::inf<T>, tg::inf<T>};

template <class I, class T>
constexpr void introspect(I&& i, interval<T>& v)
{
    i(v.min, "min");
    i(v.max, "max");
}
}
