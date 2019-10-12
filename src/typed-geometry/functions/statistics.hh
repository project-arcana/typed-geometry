#pragma once

#include <typed-geometry/common/assert.hh>
#include <typed-geometry/detail/utility.hh>
#include <typed-geometry/functions/minmax.hh>
#include <typed-geometry/types/span.hh>

/*
 * This header provides summary statistics and operations on ranges
 *
 * - min_element
 * - max_element
 * - average (same as arithmetic_mean)
 * - mean    (same as arithmetic_mean)
 * - arithmetic_mean
 * - geometric_mean (TODO)
 * - sum
 * - product
 * - variance
 * - standard_deviation
 * - count (TODO)
 *
 * Usage:
 *   auto s = tg::sum(a);         // a can be anything with range-based-for
 *   auto s = tg::mean<float>(a); // explicit element type (converts)
 */

namespace tg
{
namespace detail
{
template <class T = void, class RangeT, class TransformF, class ReduceF>
TG_NODISCARD constexpr auto fold_right(RangeT const& values, TransformF&& t, ReduceF&& f)
{
    TG_CONTRACT(tg::begin(values) != tg::end(values) && "values must not be empty");
    using R = same_or<T, element_type<RangeT>>;

    auto it = tg::begin(values);
    auto const e = tg::end(values);
    auto r = t(R(*it));
    it++;
    while (it != e)
    {
        r = f(r, t(R(*it)));
        it++;
    }
    return r;
}
}

template <class RangeT, class KeyT>
TG_NODISCARD constexpr auto min_by(RangeT const& values, KeyT&& key = {})
{
    TG_CONTRACT(tg::begin(values) != tg::end(values) && "values must not be empty");
    auto it = tg::begin(values);
    auto const e = tg::end(values);

    auto min_e = it;
    auto min_v = key(*it);

    it++;
    while (it != e)
    {
        auto v = key(*it);
        if (v < min_v)
        {
            min_v = v;
            min_e = it;
        }
        it++;
    }

    return *min_e;
}

template <class RangeT, class KeyT>
TG_NODISCARD constexpr auto max_by(RangeT const& values, KeyT&& key = {})
{
    TG_CONTRACT(tg::begin(values) != tg::end(values) && "values must not be empty");
    auto it = tg::begin(values);
    auto const e = tg::end(values);

    auto max_e = it;
    auto max_v = key(*it);

    it++;
    while (it != e)
    {
        auto v = key(*it);
        if (v > max_v)
        {
            max_v = v;
            max_e = it;
        }
        it++;
    }

    return *max_e;
}

template <class RangeT, class TransformT = identity_fun>
TG_NODISCARD constexpr auto min_element(RangeT const& values, TransformT&& transform = {})
{
    return detail::fold_right(values, transform, [](auto&& a, auto&& b) { return min(a, b); });
}

template <class RangeT, class TransformT = identity_fun>
TG_NODISCARD constexpr auto max_element(RangeT const& values, TransformT&& transform = {})
{
    return detail::fold_right(values, transform, [](auto&& a, auto&& b) { return max(a, b); });
}

template <class T = void, class RangeT = void, class TransformT = identity_fun>
TG_NODISCARD constexpr auto sum(RangeT const& values, TransformT&& transform = {})
{
    return detail::fold_right<T>(values, transform, [](auto&& a, auto&& b) { return a + b; });
}

template <class T = void, class RangeT = void, class TransformT = identity_fun>
TG_NODISCARD constexpr auto product(RangeT const& values, TransformT&& transform = {})
{
    return detail::fold_right<T>(values, transform, [](auto&& a, auto&& b) { return a * b; });
}

template <class T = void, class RangeT = void, class TransformT = identity_fun>
TG_NODISCARD constexpr auto arithmetic_mean(RangeT const& values, TransformT&& transform = {})
{
    auto const s = sum<T>(values, transform);
    return s / static_cast<decltype(s)>(tg::container_size(values));
}

template <class T = void, class RangeT = void, class TransformT = identity_fun>
TG_NODISCARD constexpr auto average(RangeT const& values, TransformT&& transform = {})
{
    return arithmetic_mean<T>(values, transform);
}

template <class T = void, class RangeT = void, class TransformT = identity_fun>
TG_NODISCARD constexpr auto mean(RangeT const& values, TransformT&& transform = {})
{
    return arithmetic_mean<T>(values, transform);
}

template <class T = void, class RangeT = void, class TransformT = identity_fun>
TG_NODISCARD constexpr auto variance(RangeT const& values, TransformT&& transform = {})
{
    using R = same_or<T, element_type<RangeT>>;

    auto const avg = arithmetic_mean<T>(values, transform);

    return arithmetic_mean<T>(values, [&](auto&& v) {
        auto const d = transform(R(v)) - avg;
        return d * d;
    });
}

template <class T = void, class RangeT = void, class TransformT = identity_fun>
TG_NODISCARD constexpr auto standard_deviation(RangeT const& values, TransformT&& transform = {})
{
    return sqrt(variance<T>(values, transform));
}
}
