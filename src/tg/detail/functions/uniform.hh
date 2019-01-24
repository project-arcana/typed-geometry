#pragma once

#include "../../types/box.hh"
#include "../../types/pos.hh"
#include "../../types/scalar.hh"
#include "../../types/triangle.hh"
#include "../random.hh"
#include "../traits.hh"

#include "mix.hh"
#include "round.hh"

/*
 * uniform(rng)       - fair coin flip
 * uniform(rng, a, b) - mix(a, b, uniform{0..1})
 * uniform(rng, obj)  - uniform sample from obj
 */

// TODO: uniform int/uint distribution might need some improvement but is currently faster than the stdlib versions

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
    i32 r;
    auto fa = f32(a);
    auto fb = f32(b) + 1;
    do
    {
        r = tg::ifloor(uniform(rng, fa, fb));
    } while (r > b);
    return r;
}
template <class Rng>
i64 uniform(Rng& rng, i64 a, i64 b)
{
    i64 r;
    auto fa = f64(a);
    auto fb = f64(b) + 1;
    do
    {
        r = tg::ifloor(uniform(rng, fa, fb));
    } while (r > b);
    return r;
}
template <class Rng>
u32 uniform(Rng& rng, u32 a, u32 b)
{
    u32 r;
    auto fa = f32(a);
    auto fb = f32(b) + 1;
    do
    {
        r = u32(tg::ifloor(uniform(rng, fa, fb)));
    } while (r > b);
    return r;
}
template <class Rng>
u64 uniform(Rng& rng, u64 a, u64 b)
{
    u64 r;
    auto fa = f64(a);
    auto fb = f64(b) + 1;
    do
    {
        r = tg::ifloor(uniform(rng, fa, fb));
    } while (r > b);
    return r;
}

template <class ScalarT, class Rng>
pos<1, ScalarT> uniform(Rng& rng, box<1, ScalarT> const& b)
{
    return {uniform(rng, b.min.x, b.max.x)};
}
template <class ScalarT, class Rng>
pos<2, ScalarT> uniform(Rng& rng, box<2, ScalarT> const& b)
{
    return {uniform(rng, b.min.x, b.max.x), //
            uniform(rng, b.min.y, b.max.y)};
}
template <class ScalarT, class Rng>
pos<3, ScalarT> uniform(Rng& rng, box<3, ScalarT> const& b)
{
    return {uniform(rng, b.min.x, b.max.x), //
            uniform(rng, b.min.y, b.max.y), //
            uniform(rng, b.min.z, b.max.z)};
}
template <class ScalarT, class Rng>
pos<4, ScalarT> uniform(Rng& rng, box<4, ScalarT> const& b)
{
    return {uniform(rng, b.min.x, b.max.x), //
            uniform(rng, b.min.y, b.max.y), //
            uniform(rng, b.min.z, b.max.z), //
            uniform(rng, b.min.w, b.max.w)};
}

template <int D, class ScalarT, class Rng, class = enable_if<is_floating_point<ScalarT>>>
vec<D, ScalarT> uniform(Rng& rng, vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return mix(a, b, detail::uniform01<ScalarT>(rng));
}
template <int D, class ScalarT, class Rng, class = enable_if<is_floating_point<ScalarT>>>
pos<D, ScalarT> uniform(Rng& rng, pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return mix(a, b, detail::uniform01<ScalarT>(rng));
}

template <int D, class ScalarT, class Rng, class = enable_if<is_floating_point<ScalarT>>>
pos<D, ScalarT> uniform(Rng& rng, triangle<D, ScalarT> const& t)
{
    auto e0 = t.v1 - t.v0;
    auto e1 = t.v2 - t.v0;
    auto u0 = uniform(rng, ScalarT(0), ScalarT(1));
    auto u1 = uniform(rng, ScalarT(0), ScalarT(1));
    if (u0 + u1 > ScalarT(1))
    {
        u0 = ScalarT(1) - u0;
        u1 = ScalarT(1) - u1;
    }
    return t.v0 + u0 * e0 + u1 * e1;
}
} // namespace tg
