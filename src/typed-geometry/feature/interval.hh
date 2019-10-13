#pragma once

#include <typed-geometry/common/constants.hh>
#include <typed-geometry/common/scalar_math.hh>
#include <typed-geometry/detail/utility.hh>
#include <typed-geometry/functions/minmax.hh>
#include <typed-geometry/types/scalars/interval.hh>

namespace tg
{
// ================================= Test =================================

template <class T>
TG_NODISCARD constexpr bool contains_zero(interval<T> const& a)
{
    return a.min <= T(0) && T(0) <= a.max;
}
template <class T>
TG_NODISCARD constexpr bool can_be_positive(interval<T> const& a)
{
    return a.max > T(0);
}
template <class T>
TG_NODISCARD constexpr bool is_fully_positive(interval<T> const& a)
{
    return a.min > T(0);
}
template <class T>
TG_NODISCARD constexpr bool can_be_negative(interval<T> const& a)
{
    return a.min < T(0);
}
template <class T>
TG_NODISCARD constexpr bool is_fully_negative(interval<T> const& a)
{
    return a.max < T(0);
}
template <class T>
TG_NODISCARD constexpr bool is_complete(interval<T> const& a)
{
    return a.min == -tg::inf<T> && a.max == tg::inf<T>;
}


// ================================= Helper =================================

template <class T>
TG_NODISCARD constexpr interval<T> merge(interval<T> const& a, interval<T> const& b)
{
    return {min(a.min, b.min), max(a.max, b.max)};
}


// ================================= Operators =================================

// auto-diff
template <class T>
TG_NODISCARD constexpr interval<T> operator+(interval<T> const& a, interval<T> const& b)
{
    return {a.min + b.min, a.max + b.max};
}
template <class T>
TG_NODISCARD constexpr interval<T> operator-(interval<T> const& a, interval<T> const& b)
{
    return {a.min - b.max, a.max - b.min};
}
template <class T>
TG_NODISCARD constexpr interval<T> operator*(interval<T> const& a, interval<T> const& b)
{
    auto v0 = a.min * b.min;
    auto v1 = a.max * b.min;
    auto v2 = a.min * b.max;
    auto v3 = a.max * b.max;

    // can only be NaN for 0 * (+-inf)
    // (in which case 0 is the appropriate result)
    if (is_nan(v0))
        v0 = T(0);
    if (is_nan(v1))
        v1 = T(0);
    if (is_nan(v2))
        v2 = T(0);
    if (is_nan(v3))
        v3 = T(0);

    return {min(v0, v1, v2, v3), max(v0, v1, v2, v3)};
}
template <class T>
TG_NODISCARD constexpr interval<T> operator/(interval<T> a, interval<T> b)
{
    if (a.min == T(0) && a.max == T(0)) // 0 / x -> 0
        return a;

    if (b.min < T(0) && b.max > T(0)) // proper zero-crossing in b? -> -inf..inf
        return {-tg::inf<T>, tg::inf<T>};

    if (is_complete(a)) // a == -inf..inf? -> -inf..inf
        return a;

    if (b.min < T(0)) // ensure positive b
    {
        a = -a;
        b = -b;
    }
    b.min += T(0); // make sure it's +0
    TG_INTERNAL_ASSERT(b.max >= T(0));

    // special case: b.min == 0
    if (b.min == T(0))
    {
        if (contains_zero(a)) // a != 0 so this is a proper zero crossing
            return {-tg::inf<T>, tg::inf<T>};
        else if (is_fully_positive(a))
            return {a.min / b.max, tg::inf<T>};
        else // fully_negative
            return {-tg::inf<T>, a.max / b.max};
    }
    // special case: b.max == inf
    else if (b.max == tg::inf<T>)
    {
        // b.min > 0
        auto const v0 = T(0);
        auto const v1 = a.min / b.min;
        auto const v2 = a.max / b.min;
        return {min(v0, v1), max(v0, v2)};
    }
    else // b.min > 0, b.max < inf
    {
        auto v0 = a.min / b.min;
        auto v1 = a.max / b.min;
        auto v2 = a.min / b.max;
        auto v3 = a.max / b.max;

        return {min(v0, v1, v2, v3), max(v0, v1, v2, v3)};
    }
}
template <class T>
TG_NODISCARD constexpr interval<T> operator+(interval<T> const& a)
{
    return {a.min, a.max};
}
template <class T>
TG_NODISCARD constexpr interval<T> operator-(interval<T> const& a)
{
    return {-a.max, -a.min};
}

// scalars
template <class T>
TG_NODISCARD constexpr interval<T> operator+(interval<T> const& a, dont_deduce<T> const& b)
{
    return {a.min + b, a.max + b};
}
template <class T>
TG_NODISCARD constexpr interval<T> operator+(dont_deduce<T> const& a, interval<T> const& b)
{
    return {a + b.min, a + b.max};
}
template <class T>
TG_NODISCARD constexpr interval<T> operator-(interval<T> const& a, dont_deduce<T> const& b)
{
    return {a.min - b, a.max - b};
}
template <class T>
TG_NODISCARD constexpr interval<T> operator-(dont_deduce<T> const& a, interval<T> const& b)
{
    return {a - b.max, a - b.min};
}
template <class T>
TG_NODISCARD constexpr interval<T> operator*(interval<T> const& a, dont_deduce<T> const& b)
{
    if (b > T(0))
        return {a.min * b, a.max * b};
    else if (b < T(0))
        return {a.max * b, a.min * b};
    else
        return {T(0), T(0)};
}
template <class T>
TG_NODISCARD constexpr interval<T> operator*(dont_deduce<T> const& a, interval<T> const& b)
{
    if (a > T(0))
        return {a * b.min, a * b.max};
    else if (a < T(0))
        return {a * b.max, a * b.min};
    else
        return {T(0), T(0)};
}
template <class T>
TG_NODISCARD constexpr interval<T> operator/(interval<T> const& a, dont_deduce<T> const& b)
{
    // b == 0 is undefined behavior
    auto inv_b = 1 / b;
    if (b >= T(0))
        return {a.min * inv_b, a.max * inv_b};
    else
        return {a.max * inv_b, a.min * inv_b};
}
template <class T>
TG_NODISCARD constexpr interval<T> operator/(dont_deduce<T> a, interval<T> b)
{
    if (a == T(0))
        return {T(0), T(0)};

    if (b.min < T(0) && b.max > T(0))
        return {-tg::inf<T>, tg::inf<T>};

    if (b.min < T(0)) // ensure positive b
    {
        a = -a;
        b = -b;
    }
    b.min += T(0); // make sure it's +0

    // cannot be NaN because a != 0
    auto const v0 = a / b.min;
    auto const v1 = a / b.max;

    if (v0 < v1)
        return {v0, v1};
    else
        return {v1, v0};
}


// ================================= Math =================================

template <class T>
TG_NODISCARD constexpr interval<T> abs(interval<T> const& v)
{
    if (contains_zero(v))
        return {T(0), max(-v.min, v.max)};
    else if (is_fully_negative(v))
        return {-v.max, -v.min};
    else
        return v;
}

template <class T>
TG_NODISCARD constexpr interval<T> sqrt(interval<T> const& v)
{
    return {sqrt(max(v.min, T(0))), sqrt(max(v.max, T(0)))};
}

template <class T>
TG_NODISCARD constexpr interval<T> exp(interval<T> const& v)
{
    return {exp(v.min), exp(v.max)};
}

template <class T>
TG_NODISCARD constexpr interval<T> log(interval<T> const& v)
{
    return {log(max(T(0), v.min)), log(max(T(0), v.max))};
}

}
