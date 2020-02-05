#pragma once

#include <typed-geometry/detail/utility.hh>

#include "comp.hh"

namespace tg
{
/// Ranges are half-open multi-dimensional integer ranges that can be iterated over
/// (they are NOT considered objects containing positions)
template <int D>
struct range;

// Common ranges

using range1 = range<1>;
using range2 = range<2>;
using range3 = range<3>;

// ======== IMPLEMENTATION ========

namespace detail
{
template <int D>
struct range_iterator;
struct range_sentinel
{
};
}

// TODO: test which iteration method is faster (branching or modulo)
template <int D>
struct range
{
    using comp_t = conditional_type<D == 1, int, comp<D, int>>;

    comp_t min; // inclusive
    comp_t max; // exclusive

    constexpr range(int max_excl) : min(0), max(max_excl) {}
    constexpr range(int min, int max_excl) : min(min), max(max_excl) {}

    // (explicitly comp_t because int overload for range<1>)
    constexpr range(comp<D, int> max_excl) : min(0), max(max_excl) {}
    constexpr range(comp<D, int> min, comp<D, int> max_excl) : min(min), max(max_excl) {}

    // range
    constexpr detail::range_iterator<D> begin() const;
    constexpr detail::range_sentinel end() const { return {}; }

    // ops
    constexpr bool operator==(range const& rhs) const { return min == rhs.min && max == rhs.max; }
    constexpr bool operator!=(range const& rhs) const { return min != rhs.min || max != rhs.max; }
};

// deduction guides
range(int)->range<1>;
range(int, int)->range<1>;
range(comp1)->range<1>;
range(comp1, comp1)->range<1>;
range(comp2)->range<2>;
range(comp2, comp2)->range<2>;
range(comp3)->range<3>;
range(comp3, comp3)->range<3>;

// reflection
template <class I, int D>
constexpr void introspect(I&& i, range<D>& v)
{
    i(v.min, "min");
    i(v.max, "max");
}
}
