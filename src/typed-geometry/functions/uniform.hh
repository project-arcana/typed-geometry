#pragma once

#include <typed-geometry/common/random.hh>
#include <typed-geometry/detail/scalar_traits.hh>

#include <typed-geometry/types/color.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/range.hh>
#include <typed-geometry/types/scalars/scalars.hh>

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/ball.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/circle.hh>
#include <typed-geometry/types/objects/disk.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include "math.hh"
#include "minmax.hh"
#include "mix.hh"

#include <initializer_list>

/*
 * uniform(rng)       - fair coin flip
 * uniform(rng, a, b) - uniform between a..b (inclusive!)
 * uniform(rng, obj)  - uniform sample from obj
 *
 * Note:
 *  to "uniformly sample" vec3(0)..vec3(1) independent per component, use uniform(rng, tg::aabb3(0, 1))
 *  to "uniformly sample" vec3(0)..vec3(1) as a segment, use uniform(rng, tg::segment3({0,0,0}, {1,1,1}))
 *
 * uniform_vec(rng, ...) same as uniform but returns uniform(rng, ...) as a vector
 */

// TODO: uniform int/uint distribution might need some improvement but is currently faster than the stdlib versions

namespace tg
{
namespace detail
{
template <class T>
struct sampler
{
    static_assert(sizeof(T) >= 0, "sampling of this type not supported without parameters");
};
}

template <class T, class Rng>
TG_NODISCARD constexpr T uniform(Rng& rng)
{
    return detail::sampler<T>::uniform(rng);
}

template <class Rng>
TG_NODISCARD constexpr f32 uniform(Rng& rng, f32 a, f32 b)
{
    return mix(a, b, detail::uniform01<f32>(rng));
}
template <class Rng>
TG_NODISCARD constexpr f64 uniform(Rng& rng, f64 a, f64 b)
{
    return mix(a, b, detail::uniform01<f64>(rng));
}
template <class Rng>
TG_NODISCARD constexpr i32 uniform(Rng& rng, i32 a, i32 b_inc)
{
    i32 r = 0;
    auto fa = f32(a);
    auto fb = f32(b_inc) + 1;
    do
    {
        r = tg::ifloor(uniform(rng, fa, fb));
    } while (r > b_inc);
    return r;
}
template <class Rng>
TG_NODISCARD constexpr i64 uniform(Rng& rng, i64 a, i64 b_inc)
{
    i64 r = 0;
    auto fa = f64(a);
    auto fb = f64(b_inc) + 1;
    do
    {
        r = tg::ifloor(uniform(rng, fa, fb));
    } while (r > b_inc);
    return r;
}
template <class Rng>
TG_NODISCARD constexpr u32 uniform(Rng& rng, u32 a, u32 b_inc)
{
    u32 r = 0;
    auto fa = f32(a);
    auto fb = f32(b_inc) + 1;
    do
    {
        r = u32(tg::ifloor(uniform(rng, fa, fb)));
    } while (r > b_inc);
    return r;
}
template <class Rng>
TG_NODISCARD constexpr u64 uniform(Rng& rng, u64 a, u64 b_inc)
{
    u64 r = 0;
    auto fa = f64(a);
    auto fb = f64(b_inc) + 1;
    do
    {
        r = u64(tg::ifloor(uniform(rng, fa, fb)));
    } while (r > b_inc);
    return r;
}

template <class T, class Rng>
TG_NODISCARD constexpr angle_t<T> uniform(Rng& rng, angle_t<T> a, angle_t<T> b)
{
    return mix(a, b, detail::uniform01<T>(rng));
}

template <class Rng, class T>
TG_NODISCARD constexpr T uniform(Rng& rng, std::initializer_list<T> list)
{
    TG_CONTRACT(list.size() > 0 && "cannot pick from an empty list");
    return list.begin()[uniform(rng, u64(0), u64(list.size() - 1))];
}

template <class Rng, class Container>
TG_NODISCARD constexpr auto uniform(Rng& rng, Container const& c) -> decltype(c[c.size()])
{
    TG_CONTRACT(c.size() > 0 && "cannot pick from an empty container");
    return c[uniform(rng, u64(0), u64(c.size() - 1))];
}

template <class Rng>
TG_NODISCARD constexpr int uniform(Rng& rng, range1 const& b)
{
    return uniform(rng, b.min, b.max - 1);
}

template <class Rng>
TG_NODISCARD constexpr comp<2, int> uniform(Rng& rng, range2 const& b)
{
    return {uniform(rng, b.min.comp0, b.max.comp0 - 1), //
            uniform(rng, b.min.comp1, b.max.comp1 - 1)};
}

template <class Rng>
TG_NODISCARD constexpr comp<3, int> uniform(Rng& rng, range3 const& b)
{
    return {uniform(rng, b.min.comp0, b.max.comp0 - 1), //
            uniform(rng, b.min.comp1, b.max.comp1 - 1), //
            uniform(rng, b.min.comp2, b.max.comp2 - 1)};
}

template <class ScalarT, class Rng>
TG_NODISCARD constexpr pos<1, ScalarT> uniform(Rng& rng, aabb<1, ScalarT> const& b)
{
    return {uniform(rng, b.min.x, b.max.x)};
}
template <class ScalarT, class Rng>
TG_NODISCARD constexpr pos<2, ScalarT> uniform(Rng& rng, aabb<2, ScalarT> const& b)
{
    return {uniform(rng, b.min.x, b.max.x), //
            uniform(rng, b.min.y, b.max.y)};
}
template <class ScalarT, class Rng>
TG_NODISCARD constexpr pos<3, ScalarT> uniform(Rng& rng, aabb<3, ScalarT> const& b)
{
    return {uniform(rng, b.min.x, b.max.x), //
            uniform(rng, b.min.y, b.max.y), //
            uniform(rng, b.min.z, b.max.z)};
}
template <class ScalarT, class Rng>
TG_NODISCARD constexpr pos<4, ScalarT> uniform(Rng& rng, aabb<4, ScalarT> const& b)
{
    return {uniform(rng, b.min.x, b.max.x), //
            uniform(rng, b.min.y, b.max.y), //
            uniform(rng, b.min.z, b.max.z), //
            uniform(rng, b.min.w, b.max.w)};
}

template <int D, class ScalarT, class Rng>
TG_NODISCARD constexpr pos<D, ScalarT> uniform(Rng& rng, segment<D, ScalarT> const& s)
{
    return mix(s.pos0, s.pos1, detail::uniform01<ScalarT>(rng));
}

template <int D, class ScalarT, class Rng>
TG_NODISCARD constexpr pos<D, ScalarT> uniform(Rng& rng, box<D, ScalarT> const& b)
{
    return b.center + b.half_extents * uniform_vec(rng, aabb<D, ScalarT>::minus_one_to_one);
}

template <class ScalarT, class Rng>
TG_NODISCARD constexpr pos<2, ScalarT> uniform(Rng& rng, circle<2, ScalarT> const& c)
{
    return c.center + c.radius * uniform<dir<2, ScalarT>>(rng);
}
template <class ScalarT, class Rng>
TG_NODISCARD constexpr pos<3, ScalarT> uniform(Rng& rng, circle<3, ScalarT> const& c)
{
    auto direction = uniform<dir<2, ScalarT>>(rng);
    auto x = any_normal(c.normal);
    auto y = cross(c.normal, x);
    return c.center + c.radius * (direction.x * x + direction.y * y);
}

template <class ScalarT, class Rng>
TG_NODISCARD constexpr pos<2, ScalarT> uniform(Rng& rng, disk<2, ScalarT> const& d)
{
    return uniform(rng, ball<2, ScalarT>(d.center, d.radius));
}
template <class ScalarT, class Rng>
TG_NODISCARD constexpr pos<3, ScalarT> uniform(Rng& rng, disk<3, ScalarT> const& d)
{
    auto direction = uniform(rng, ball<2, ScalarT>(pos<2, ScalarT>::zero, d.radius));
    auto x = any_normal(d.normal);
    auto y = cross(d.normal, x);
    return d.center + direction.x * x + direction.y * y;
}

template <int D, class ScalarT, class Rng>
TG_NODISCARD constexpr pos<D, ScalarT> uniform(Rng& rng, sphere<D, ScalarT> const& s)
{
    auto ub = tg::aabb<D, ScalarT>::minus_one_to_one;
    while (true)
    {
        auto p = uniform_vec(rng, ub);
        auto l = length_sqr(p);
        if (l > ScalarT(0) && l <= ScalarT(1))
            return s.center + p * (s.radius / sqrt(l));
    }
}

template <int D, class ScalarT, class Rng>
TG_NODISCARD constexpr pos<D, ScalarT> uniform(Rng& rng, ball<D, ScalarT> const& b)
{
    auto ub = tg::aabb<D, ScalarT>::minus_one_to_one;
    while (true)
    {
        auto p = uniform_vec(rng, ub);
        auto l = length_sqr(p);
        if (l <= ScalarT(1))
            return b.center + p * b.radius;
    }
}

template <int D, class ScalarT, class Rng, class = enable_if<is_floating_point<ScalarT>>>
[[deprecated("potentially misleading operation. use uniform_vec(rng, tg::aabb3(..)) or uniform_vec(rng, tg::segment3(..)) depending on your intended "
             "semantics")]] TG_NODISCARD constexpr vec<D, ScalarT>
uniform(Rng& rng, vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return mix(a, b, detail::uniform01<ScalarT>(rng));
}
template <int D, class ScalarT, class Rng, class = enable_if<is_floating_point<ScalarT>>>
[[deprecated("potentially misleading operation. use uniform(rng, tg::aabb3(..)) or uniform(rng, tg::segment3(..)) depending on your intended "
             "semantics")]] TG_NODISCARD constexpr pos<D, ScalarT>
uniform(Rng& rng, pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return mix(a, b, detail::uniform01<ScalarT>(rng));
}

template <int D, class ScalarT, class Rng, class = enable_if<is_floating_point<ScalarT>>>
TG_NODISCARD constexpr pos<D, ScalarT> uniform(Rng& rng, triangle<D, ScalarT> const& t)
{
    auto e0 = t.pos1 - t.pos0;
    auto e1 = t.pos2 - t.pos0;
    auto u0 = uniform(rng, ScalarT(0), ScalarT(1));
    auto u1 = uniform(rng, ScalarT(0), ScalarT(1));
    if (u0 + u1 > ScalarT(1))
    {
        u0 = ScalarT(1) - u0;
        u1 = ScalarT(1) - u1;
    }
    return t.pos0 + u0 * e0 + u1 * e1;
}

template <class Rng, class... Args>
TG_NODISCARD constexpr auto uniform_vec(Rng& rng, Args const&... args) -> decltype(uniform(rng, args...) - decltype(uniform(rng, args...))::zero)
{
    return uniform(rng, args...) - decltype(uniform(rng, args...))::zero;
}


namespace detail
{
template <>
struct sampler<bool>
{
    template <class Rng>
    constexpr static bool uniform(Rng& rng)
    {
        return rng() & 1;
    }
};
template <class T>
struct sampler<angle_t<T>>
{
    template <class Rng>
    constexpr static angle_t<T> uniform(Rng& rng)
    {
        return tg::uniform(rng, tg::radians(T(0)), 2 * tg::pi<T>);
    }
};
template <class T>
struct sampler<color<3, T>>
{
    template <class Rng>
    constexpr static color<3, T> uniform(Rng& rng)
    {
        return tg::color3(tg::uniform(rng, T(0), T(1)), tg::uniform(rng, T(0), T(1)), tg::uniform(rng, T(0), T(1)));
    }
};
template <class T>
struct sampler<color<4, T>>
{
    template <class Rng>
    constexpr static color<4, T> uniform(Rng& rng)
    {
        return tg::color3(tg::uniform(rng, T(0), T(1)), tg::uniform(rng, T(0), T(1)), tg::uniform(rng, T(0), T(1)), tg::uniform(rng, T(0), T(1)));
    }
};
template <int D, class ScalarT>
struct sampler<dir<D, ScalarT>>
{
    template <class Rng>
    constexpr static dir<D, ScalarT> uniform(Rng& rng)
    {
        auto ub = tg::aabb<D, ScalarT>::minus_one_to_one;
        while (true)
        {
            auto p = uniform_vec(rng, ub);
            auto l = length_sqr(p);
            if (l > ScalarT(0) && l <= ScalarT(1))
                return tg::dir<D, ScalarT>(p / sqrt(l));
        }
    }
};
}

} // namespace tg
