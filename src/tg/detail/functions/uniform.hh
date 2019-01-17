#pragma once

#include "../../types/box.hh"
#include "../../types/scalar.hh"
#include "../random.hh"
#include "../traits.hh"

#include "mix.hh"

/*
 * uniform(rng)       - fair coin flip
 * uniform(rng, a, b) - mix(a, b, uniform{0..1})
 * uniform(rng, obj)  - uniform sample from obj
 */

namespace tg
{
template <class Rng>
bool uniform(Rng& rng)
{
    return rng() & 1;
}

template <class Rng>
f32 uniform(Rng& rng, f32 a, f32 b)
{
    return mix(a, b, detail::uniform01<f32>(rng));
}
template <class Rng>
f64 uniform(Rng& rng, f64 a, f64 b)
{
    return mix(a, b, detail::uniform01<f64>(rng));
}
template <class Rng>
i32 uniform(Rng& rng, i32 a, i32 b)
{
    return std::uniform_int_distribution<i32>(a, b)(rng);
}
template <class Rng>
i64 uniform(Rng& rng, i64 a, i64 b)
{
    return std::uniform_int_distribution<i64>(a, b)(rng);
}
template <class Rng>
u32 uniform(Rng& rng, u32 a, u32 b)
{
    return std::uniform_int_distribution<i32>(a, b)(rng);
}
template <class Rng>
u64 uniform(Rng& rng, u64 a, u64 b)
{
    return std::uniform_int_distribution<i64>(a, b)(rng);
}

template <int D, class ScalarT, class Rng, class = std::enable_if_t<is_floating_point<ScalarT>>>
vec<D, ScalarT> uniform(Rng& rng, vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return mix(a, b, detail::uniform01<ScalarT>(rng));
}
template <int D, class ScalarT, class Rng, class = std::enable_if_t<is_floating_point<ScalarT>>>
pos<D, ScalarT> uniform(Rng& rng, pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return mix(a, b, detail::uniform01<ScalarT>(rng));
}
} // namespace tg
