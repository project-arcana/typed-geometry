#pragma once

#include "uniform.hh"

namespace tg
{
template <class A, class B, class Rng>
[[nodiscard]] constexpr auto random_choice(Rng& rng, A const& a, B const& b) -> decltype(uniform<bool>(rng) ? a : b)
{
    return uniform<bool>(rng) ? a : b;
}

template <class Rng, class Container>
[[nodiscard]] constexpr auto random_choice(Rng& rng, Container& c) -> decltype(c[c.size()])
{
    TG_CONTRACT(c.size() > 0 && "cannot pick from an empty container");
    return c[uniform(rng, tg::u64(0), tg::u64(c.size() - 1))];
}

}
