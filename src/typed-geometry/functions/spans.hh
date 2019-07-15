#pragma once

#include <typed-geometry/common/assert.hh>
#include <typed-geometry/functions/minmax.hh>
#include <typed-geometry/types/span.hh>

/*
 * This header provides summary statistics and operations on spans
 *
 * - min/max (in header minmax)
 *
 * - average (same as arithmetic_mean)
 * - mean    (same as arithmetic_mean)
 * - arithmetic_mean
 * - geometric_mean (TODO)
 * - sum
 * - product
 * - variance
 * - standard_deviation
 */

namespace tg
{
// mean and average are both aliases of arithmetic_mean
template <class ReturnT = void, class T = void>
TG_NODISCARD constexpr same_or<ReturnT, T> average(span<T> values)
{
    return arithmetic_mean(values);
}
template <class ReturnT = void, class T = void>
TG_NODISCARD constexpr same_or<ReturnT, T> mean(span<T> values)
{
    return arithmetic_mean(values);
}

template <class ReturnT = void, class T = void>
TG_NODISCARD constexpr same_or<ReturnT, T> sum(span<T> values)
{
    TG_CONTRACT(!values.empty());
    using R = same_or<ReturnT, T>;
    auto r = R(values[0]);
    for (tg::size_t i = 1; i < values.size(); ++i)
        r = r + R(values[i]);
    return r;
}

template <class ReturnT = void, class T = void>
TG_NODISCARD constexpr same_or<ReturnT, T> product(span<T> values)
{
    TG_CONTRACT(!values.empty());
    using R = same_or<ReturnT, T>;
    auto r = R(values[0]);
    for (tg::size_t i = 1; i < values.size(); ++i)
        r = r * R(values[i]);
    return r;
}

template <class ReturnT = void, class T = void>
TG_NODISCARD constexpr same_or<ReturnT, T> arithmetic_mean(span<T> values)
{
    TG_CONTRACT(!values.empty());
    return sum<ReturnT, T>(values) / values.size();
}

template <class ReturnT = void, class T = void>
TG_NODISCARD constexpr same_or<ReturnT, T> variance(span<T> values)
{
    TG_CONTRACT(!values.empty());
    auto avg = sum<ReturnT, T>(values);
    auto f = [](ReturnT v) {
        auto d = v - avg;
        return d * d;
    };
    auto r = f(R(values[0]));
    for (tg::size_t i = 1; i < values.size(); ++i)
        r = r + f(R(values[i]));
    return r / values.size();
}

template <class ReturnT = void, class T = void>
TG_NODISCARD constexpr same_or<ReturnT, T> standard_deviation(span<T> values)
{
    TG_CONTRACT(!values.empty());
    return tg::sqrt(variance<ReturnT, T>(values));
}
}
