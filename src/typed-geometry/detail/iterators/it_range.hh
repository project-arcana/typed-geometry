#pragma once

#include <typed-geometry/types/range.hh>

namespace tg
{
template <int D>
struct detail::range_iterator
{
    using comp_t = typename range<D>::comp_t;

    comp_t curr;
    comp_t min;
    comp_t max;

    template <int I>
    constexpr void inc()
    {
        if constexpr (D == 1)
            ++curr;
        else
        {
            ++curr[I];

            if constexpr (I < D - 1)
            {
                if (curr[I] >= max[I])
                {
                    curr[I] = min[I];
                    inc<I + 1>();
                }
            }
        }
    }

    constexpr void operator++() { inc<0>(); }

    constexpr comp_t operator*() const { return curr; }
    constexpr bool operator!=(range_sentinel) const
    {
        if constexpr (D == 1)
            return curr < max;
        else
            return curr[D - 1] < max[D - 1];
    }
};

template <int D>
constexpr detail::range_iterator<D> range<D>::begin() const
{
    return {min, min, max};
}
}