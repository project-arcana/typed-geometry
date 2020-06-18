#pragma once

#include <typed-geometry/types/span.hh>

namespace tg
{
template <class Rng, class T>
constexpr void shuffle(Rng& rng, span<T> range)
{
    for (size_t i = 1; i < range.size(); ++i)
    {
        auto j = rng() % (i + 1);
        if (i != j)
            detail::swap(range[i], range[j]);
    }
}
template <class Rng, class Range>
constexpr void shuffle(Rng& rng, Range& range)
{
    shuffle(rng, tg::span(range));
}
}
