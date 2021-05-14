#pragma once

#include <cstddef>
#include <initializer_list>

#include <typed-geometry/detail/scalar_traits.hh>

#include <typed-geometry/types/color.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/range.hh>
#include <typed-geometry/types/scalars/scalars.hh>

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/capsule.hh>
#include <typed-geometry/types/objects/cone.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/ellipse.hh>
#include <typed-geometry/types/objects/hemisphere.hh>
#include <typed-geometry/types/objects/pyramid.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include <typed-geometry/functions/basic/minmax.hh>
#include <typed-geometry/functions/basic/mix.hh>
#include <typed-geometry/functions/vector/math.hh>

#include "random.hh"

/*
 * uniform<T>(rng)    - uniform sample for T's where no parameter is needed
 * uniform(rng)       - returns an object that implicitly converts to T (and calls uniform<T>(rng))
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

// NOTE: this class should not be saved
template <class Rng>
struct uniform_sampler
{
    explicit uniform_sampler(Rng& rng) : rng(rng) {}

    template <class T>
    operator T() &&
    {
        return sampler<T>::uniform(rng);
    }
    template <class T>
    operator T() &
    {
        static_assert(always_false<T>, "the result of uniform(rng) must not be stored!");
    }

private:
    Rng& rng;
};
}

template <class T, class Rng>
[[nodiscard]] constexpr T uniform(Rng& rng)
{
    return detail::sampler<T>::uniform(rng);
}

template <class Rng>
[[nodiscard]] constexpr detail::uniform_sampler<Rng> uniform(Rng& rng)
{
    return detail::uniform_sampler<Rng>{rng};
}

// ======== Uniform between two numbers ========

template <class Rng>
[[nodiscard]] constexpr f32 uniform(Rng& rng, f32 a, f32 b)
{
    return mix(a, b, detail::uniform01<f32>(rng));
}
template <class Rng>
[[nodiscard]] constexpr f64 uniform(Rng& rng, f64 a, f64 b)
{
    return mix(a, b, detail::uniform01<f64>(rng));
}
template <class Rng>
[[nodiscard]] constexpr i32 uniform(Rng& rng, i32 a, i32 b_inc)
{
    TG_CONTRACT(a <= b_inc);
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
[[nodiscard]] constexpr long uniform(Rng& rng, long a, long b_inc)
{
    TG_CONTRACT(a <= b_inc);
    long r = 0;
    auto fa = f64(a);
    auto fb = f64(b_inc) + 1;
    do
    {
        r = (long)tg::ifloor(uniform(rng, fa, fb));
    } while (r > b_inc);
    return r;
}
template <class Rng>
[[nodiscard]] constexpr i64 uniform(Rng& rng, long long a, long long b_inc)
{
    TG_CONTRACT(a <= b_inc);
    long long r = 0;
    auto fa = f64(a);
    auto fb = f64(b_inc) + 1;
    do
    {
        r = (long long)tg::ifloor(uniform(rng, fa, fb));
    } while (r > b_inc);
    return r;
}
template <class Rng>
[[nodiscard]] constexpr u32 uniform(Rng& rng, u32 a, u32 b_inc)
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
[[nodiscard]] constexpr unsigned long uniform(Rng& rng, unsigned long a, unsigned long b_inc)
{
    TG_CONTRACT(a <= b_inc);
    unsigned long r = 0;
    auto fa = f64(a);
    auto fb = f64(b_inc) + 1;
    do
    {
        r = (unsigned long)(tg::ifloor(uniform(rng, fa, fb)));
    } while (r > b_inc);
    return r;
}
template <class Rng>
[[nodiscard]] constexpr unsigned long long uniform(Rng& rng, unsigned long long a, unsigned long long b_inc)
{
    TG_CONTRACT(a <= b_inc);
    unsigned long long r = 0;
    auto fa = f64(a);
    auto fb = f64(b_inc) + 1;
    do
    {
        r = (unsigned long long)(tg::ifloor(uniform(rng, fa, fb)));
    } while (r > b_inc);
    return r;
}
template <class Rng>
[[nodiscard]] constexpr char uniform(Rng& rng, char a, char b_inc)
{
    return char(uniform(rng, i32(a), i32(b_inc)));
}
template <class Rng>
[[nodiscard]] constexpr i8 uniform(Rng& rng, i8 a, i8 b_inc)
{
    return i8(uniform(rng, i32(a), i32(b_inc)));
}
template <class Rng>
[[nodiscard]] constexpr i16 uniform(Rng& rng, i16 a, i16 b_inc)
{
    return i16(uniform(rng, i32(a), i32(b_inc)));
}
template <class Rng>
[[nodiscard]] constexpr u8 uniform(Rng& rng, u8 a, u8 b_inc)
{
    return u8(uniform(rng, i32(a), i32(b_inc)));
}
template <class Rng>
[[nodiscard]] constexpr u16 uniform(Rng& rng, u16 a, u16 b_inc)
{
    return u16(uniform(rng, i32(a), i32(b_inc)));
}

template <class T, class Rng>
[[nodiscard]] constexpr angle_t<T> uniform(Rng& rng, angle_t<T> a, angle_t<T> b)
{
    return mix(a, b, detail::uniform01<T>(rng));
}

// ======== Uniform selection from a list ========

template <class Rng, class T>
[[nodiscard]] constexpr T uniform(Rng& rng, std::initializer_list<T> list)
{
    TG_CONTRACT(list.size() > 0 && "cannot pick from an empty list");
    return list.begin()[uniform(rng, u64(0), u64(list.size() - 1))];
}

template <class Rng, class Container>
[[nodiscard]] constexpr auto uniform(Rng& rng, Container& c) -> decltype(c[c.size()])
{
    TG_CONTRACT(c.size() > 0 && "cannot pick from an empty container");
    return c[uniform(rng, u64(0), u64(c.size() - 1))];
}

// ======== Uniform point on multiple objects ========

template <class Rng, class ObjectA, class... ObjectRest>
constexpr auto uniform_by_length(Rng& rng, ObjectA const& obj, ObjectRest const&... rest) -> typename object_traits<ObjectA>::pos_t
{
    using ScalarT = typename object_traits<ObjectA>::scalar_t;

    static_assert(object_traits<ObjectA>::object_dimension == 1, "objects must be 1D (but might be embedded in another dimension)");
    static_assert((... && (object_traits<ObjectRest>::object_dimension == 1)), "objects must be 1D (but might be embedded in another dimension)");
    static_assert((... && (object_traits<ObjectA>::domain_dimension == object_traits<ObjectRest>::domain_dimension)), "objects must be in the same "
                                                                                                                      "domain");
    static_assert((... && std::is_same_v<typename object_traits<ObjectRest>::scalar_t, ScalarT>), "objects must have same scalar type");
    static_assert(!std::is_integral_v<ScalarT>, "sampling from integer objects not supported (yet)");

    ScalarT lengthSums[1 + sizeof...(rest)];
    auto lengthSum = ScalarT(0);
    { // compute cumulative areas
        auto i = 0;
        lengthSums[i++] = lengthSum += length(obj);
        ((lengthSums[i++] = lengthSum += length(rest)), ...);
    }

    auto part = uniform(rng, ScalarT(0), lengthSum);

    // special case for the first object
    if (part <= lengthSums[0])
        return uniform(rng, obj);

    typename object_traits<ObjectA>::pos_t result;
    { // otherwise, sample from rest
        auto i = 1;
        // this code finds the interval where part is within lengthSums, samples from the associated object (stored in result), then stops by short circuiting
        auto ok = ((part <= lengthSums[i++] ? (result = uniform(rng, rest), true) : false) || ...);
        TG_ASSERT(ok);
    }
    return result;
}

template <class Rng, class ObjectA, class... ObjectRest>
constexpr auto uniform_by_area(Rng& rng, ObjectA const& obj, ObjectRest const&... rest) -> typename object_traits<ObjectA>::pos_t
{
    using ScalarT = typename object_traits<ObjectA>::scalar_t;

    static_assert(object_traits<ObjectA>::object_dimension == 2, "objects must be 2D (but might be embedded in another dimension)");
    static_assert((... && (object_traits<ObjectRest>::object_dimension == 2)), "objects must be 2D (but might be embedded in another dimension)");
    static_assert((... && (object_traits<ObjectA>::domain_dimension == object_traits<ObjectRest>::domain_dimension)), "objects must be in the same "
                                                                                                                      "domain");
    static_assert((... && std::is_same_v<typename object_traits<ObjectRest>::scalar_t, ScalarT>), "objects must have same scalar type");
    static_assert(!std::is_integral_v<ScalarT>, "sampling from integer objects not supported (yet)");

    ScalarT areaSums[1 + sizeof...(rest)];
    auto areaSum = ScalarT(0);
    { // compute cumulative areas
        auto i = 0;
        areaSums[i++] = areaSum += area_of(obj);
        ((areaSums[i++] = areaSum += area_of(rest)), ...);
    }

    auto part = uniform(rng, ScalarT(0), areaSum);

    // special case for the first object
    if (part <= areaSums[0])
        return uniform(rng, obj);

    typename object_traits<ObjectA>::pos_t result;
    { // otherwise, sample from rest
        auto i = 1;
        // this code finds the interval where part is within areaSums, samples from the associated object (stored in result), then stops by short circuiting
        auto ok = ((part <= areaSums[i++] ? (result = uniform(rng, rest), true) : false) || ...);
        TG_ASSERT(ok);
    }
    return result;
}

template <class Rng, class ObjectA, class... ObjectRest>
constexpr auto uniform_by_volume(Rng& rng, ObjectA const& obj, ObjectRest const&... rest) -> typename object_traits<ObjectA>::pos_t
{
    using ScalarT = typename object_traits<ObjectA>::scalar_t;

    static_assert(object_traits<ObjectA>::object_dimension == 3, "objects must be 3D (but might be embedded in another dimension)");
    static_assert((... && (object_traits<ObjectRest>::object_dimension == 3)), "objects must be 3D (but might be embedded in another dimension)");
    static_assert((... && (object_traits<ObjectA>::domain_dimension == object_traits<ObjectRest>::domain_dimension)), "objects must be in the same "
                                                                                                                      "domain");
    static_assert((... && std::is_same_v<typename object_traits<ObjectRest>::scalar_t, ScalarT>), "objects must have same scalar type");
    static_assert(!std::is_integral_v<ScalarT>, "sampling from integer objects not supported (yet)");

    ScalarT volumeSums[1 + sizeof...(rest)];
    auto volumeSum = ScalarT(0);
    { // compute cumulative volumes
        auto i = 0;
        volumeSums[i++] = volumeSum += volume_of(obj);
        ((volumeSums[i++] = volumeSum += volume_of(rest)), ...);
    }

    auto part = uniform(rng, ScalarT(0), volumeSum);

    // special case for the first object
    if (part <= volumeSums[0])
        return uniform(rng, obj);

    typename object_traits<ObjectA>::pos_t result;
    { // otherwise, sample from rest
        auto i = 1;
        // this code finds the interval where part is within volumeSums, samples from the associated object (stored in result), then stops by short circuiting
        auto ok = ((part <= volumeSums[i++] ? (result = uniform(rng, rest), true) : false) || ...);
        TG_ASSERT(ok);
    }
    return result;
}

// ======== Uniform point on an object ========

template <int D, class ScalarT, class Rng>
[[nodiscard]] constexpr pos<D, ScalarT> uniform(Rng&, pos<D, ScalarT> const& p)
{
    return p; // not really a random position, but this makes some generic implementations easier
}

template <class Rng>
[[nodiscard]] constexpr int uniform(Rng& rng, range1 const& b)
{
    return uniform(rng, b.min, b.max - 1);
}

template <class Rng>
[[nodiscard]] constexpr comp<2, int> uniform(Rng& rng, range2 const& b)
{
    return {uniform(rng, b.min.comp0, b.max.comp0 - 1), //
            uniform(rng, b.min.comp1, b.max.comp1 - 1)};
}

template <class Rng>
[[nodiscard]] constexpr comp<3, int> uniform(Rng& rng, range3 const& b)
{
    return {uniform(rng, b.min.comp0, b.max.comp0 - 1), //
            uniform(rng, b.min.comp1, b.max.comp1 - 1), //
            uniform(rng, b.min.comp2, b.max.comp2 - 1)};
}

template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<1, ScalarT> uniform(Rng& rng, aabb<1, ScalarT> const& b)
{
    return {uniform(rng, b.min.x, b.max.x)};
}
template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<2, ScalarT> uniform(Rng& rng, aabb<2, ScalarT> const& b)
{
    return {uniform(rng, b.min.x, b.max.x), //
            uniform(rng, b.min.y, b.max.y)};
}
template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<3, ScalarT> uniform(Rng& rng, aabb<3, ScalarT> const& b)
{
    return {uniform(rng, b.min.x, b.max.x), //
            uniform(rng, b.min.y, b.max.y), //
            uniform(rng, b.min.z, b.max.z)};
}
template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<4, ScalarT> uniform(Rng& rng, aabb<4, ScalarT> const& b)
{
    return {uniform(rng, b.min.x, b.max.x), //
            uniform(rng, b.min.y, b.max.y), //
            uniform(rng, b.min.z, b.max.z), //
            uniform(rng, b.min.w, b.max.w)};
}
template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<1, ScalarT> uniform(Rng& rng, aabb_boundary<1, ScalarT> const& b)
{
    return uniform<bool>(rng) ? b.min.x : b.max.x;
}
template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<2, ScalarT> uniform(Rng& rng, aabb_boundary<2, ScalarT> const& b)
{
    auto extends = b.max - b.min;
    if (uniform(rng, ScalarT(0), extends.x + extends.y) < extends.x)
        return {uniform(rng, b.min.x, b.max.x), //
                uniform<bool>(rng) ? b.min.y : b.max.y};

    return {uniform<bool>(rng) ? b.min.x : b.max.x, //
            uniform(rng, b.min.y, b.max.y)};
}
template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<3, ScalarT> uniform(Rng& rng, aabb_boundary<3, ScalarT> const& b)
{
    auto extends = b.max - b.min;
    auto areaX = extends.y * extends.z;
    auto areaY = extends.x * extends.z;
    auto areaZ = extends.x * extends.y;

    auto res = uniform(rng, solid_of(b)); // Sample a random point inside the aabb
    // Project to one of the sides, proportional to their area
    auto part = uniform(rng, ScalarT(0), areaX + areaY + areaZ);
    int i = part < areaX ? 0 : part < areaX + areaY ? 1 : 2;
    res[i] = uniform<bool>(rng) ? b.min[i] : b.max[i];
    return res;
}
template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<4, ScalarT> uniform(Rng& rng, aabb_boundary<4, ScalarT> const& b)
{
    auto extends = b.max - b.min;
    auto volX = extends.y * extends.z * extends.w;
    auto volY = extends.x * extends.z * extends.w;
    auto volZ = extends.x * extends.y * extends.w;
    auto volW = extends.x * extends.y * extends.z;

    auto res = uniform(rng, solid_of(b)); // Sample a random point inside the aabb
    // Project to one of the borders, proportional to their volume
    auto part = uniform(rng, ScalarT(0), volX + volY + volZ + volW);
    int i = part < volX + volY ? (part < volX ? 0 : 1) : (part < volX + volY + volZ ? 2 : 3);
    res[i] = uniform<bool>(rng) ? b.min[i] : b.max[i];
    return res;
}

template <int D, class ScalarT, class Rng>
[[nodiscard]] constexpr pos<D, ScalarT> uniform(Rng& rng, segment<D, ScalarT> const& s)
{
    return mix(s.pos0, s.pos1, detail::uniform01<ScalarT>(rng));
}

template <int D, class ScalarT, class Rng, class = enable_if<is_floating_point<ScalarT>>>
[[nodiscard]] constexpr pos<D, ScalarT> uniform(Rng& rng, triangle<D, ScalarT> const& t)
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

template <int ObjectD, class ScalarT, int DomainD, class TraitsT, class Rng>
[[nodiscard]] constexpr pos<DomainD, ScalarT> uniform(Rng& rng, box<ObjectD, ScalarT, DomainD, TraitsT> const& b)
{
    return b.center + b.half_extents * uniform_vec(rng, aabb<ObjectD, ScalarT, TraitsT>::minus_one_to_one);
}

template <int ObjectD, class ScalarT, int DomainD, class Rng>
[[nodiscard]] constexpr pos<DomainD, ScalarT> uniform(Rng& rng, ellipse<ObjectD, ScalarT, DomainD> const& e)
{
    // Note: The transformation preserves the uniform distribution in the interior. This is not the case for the boundary_tag variant.
    return e.center + e.semi_axes * uniform_vec(rng, sphere<ObjectD, ScalarT>::unit);
}

template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<1, ScalarT> uniform(Rng& rng, ellipse_boundary<1, ScalarT> const& e)
{
    return e.center + (uniform<bool>(rng) ? e.semi_axes[0][0] : -e.semi_axes[0][0]);
}
template <int ObjectD, class ScalarT, int DomainD, class Rng>
[[nodiscard]] constexpr pos<DomainD, ScalarT> uniform(Rng& rng, ellipse_boundary<ObjectD, ScalarT, DomainD> const& e)
{
    // based on https://math.stackexchange.com/a/982833
    auto ls = vec<ObjectD, ScalarT>();
    auto coeffs = comp<ObjectD, ScalarT>();
    for (auto i = 0; i < ObjectD; ++i)
        ls[i] = length(e.semi_axes[i]);

    if constexpr (ObjectD == 2)
        coeffs = {ls.x, ls.y};
    else if constexpr (ObjectD == 3)
        coeffs = {ls.y * ls.z, ls.x * ls.z, ls.x * ls.y};
    else if constexpr (ObjectD == 4)
        coeffs = {ls.y * ls.z * ls.w, ls.x * ls.z * ls.w, ls.x * ls.y * ls.w, ls.x * ls.y * ls.z};
    else
        static_assert(always_false<ObjectD>, "dimension not supported");

    auto probMax = max_element(coeffs);

    // sample transformed sphere_boundary with rejection of some samples for uniform distribution
    while (true)
    {
        auto v = uniform_vec(rng, sphere_boundary<ObjectD, ScalarT>::unit);
        auto prob = length(coeffs * v); // == sqrt(pow2(coeffs.x * v.x) + pow2(coeffs.y * v.y) + ...)
        if (detail::uniform01<ScalarT>(rng) <= prob / probMax)
            return e.center + e.semi_axes * v;
    }
}

template <int D, class ScalarT, class Rng>
[[nodiscard]] constexpr pos<D, ScalarT> uniform(Rng& rng, sphere_boundary<D, ScalarT> const& s)
{
    auto ub = aabb<D, ScalarT>::minus_one_to_one;
    while (true)
    {
        auto p = uniform_vec(rng, ub);
        auto l = length_sqr(p);
        if (l > ScalarT(0) && l <= ScalarT(1))
            return s.center + p * (s.radius / sqrt(l));
    }
}

template <int D, class ScalarT, class Rng>
[[nodiscard]] constexpr pos<D, ScalarT> uniform(Rng& rng, sphere<D, ScalarT> const& b)
{
    auto ub = aabb<D, ScalarT>::minus_one_to_one;
    while (true)
    {
        auto p = uniform_vec(rng, ub);
        auto l = length_sqr(p);
        if (l <= ScalarT(1))
            return b.center + p * b.radius;
    }
}

template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<2, ScalarT> uniform(Rng& rng, sphere_boundary<2, ScalarT> const& c)
{
    return c.center + c.radius * uniform<dir<2, ScalarT>>(rng);
}
template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<3, ScalarT> uniform(Rng& rng, sphere_boundary<2, ScalarT, 3> const& c)
{
    auto direction = uniform<dir<2, ScalarT>>(rng);
    auto x = any_normal(c.normal);
    auto y = cross(c.normal, x);
    return c.center + c.radius * (direction.x * x + direction.y * y);
}

template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<3, ScalarT> uniform(Rng& rng, sphere<2, ScalarT, 3> const& d)
{
    auto direction = uniform(rng, sphere<2, ScalarT>(pos<2, ScalarT>::zero, d.radius));
    auto x = any_normal(d.normal);
    auto y = cross(d.normal, x);
    return d.center + direction.x * x + direction.y * y;
}

template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<2, ScalarT> uniform(Rng& rng, sphere<1, ScalarT, 2> const& s)
{
    auto v = perpendicular(s.normal) * s.radius;
    return mix(s.center - v, s.center + v, detail::uniform01<ScalarT>(rng));
}
template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<2, ScalarT> uniform(Rng& rng, sphere_boundary<1, ScalarT, 2> const& s)
{
    auto v = perpendicular(s.normal) * s.radius;
    return uniform(rng) ? s.center + v : s.center - v;
}

template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<3, ScalarT> uniform(Rng& rng, cylinder<3, ScalarT> const& t)
{
    auto d = sphere<2, ScalarT, 3>(pos<3, ScalarT>::zero, t.radius, normalize(t.axis.pos1 - t.axis.pos0));
    return uniform(rng, t.axis) + vec<3, ScalarT>(uniform(rng, d));
}
template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<3, ScalarT> uniform(Rng& rng, cylinder_boundary_no_caps<3, ScalarT> const& t)
{
    auto c = sphere_boundary<2, ScalarT, 3>(pos<3, ScalarT>::zero, t.radius, normalize(t.axis.pos1 - t.axis.pos0));
    return uniform(rng, t.axis) + vec<3, ScalarT>(uniform(rng, c));
}

template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<3, ScalarT> uniform(Rng& rng, cylinder_boundary<3, ScalarT> const& c)
{
    auto x = c.axis.pos1 - c.axis.pos0;
    auto h = length(x);
    auto sideArea = ScalarT(2) * c.radius * h; // * Pi, but that does not matter here
    auto capArea = c.radius * c.radius;        // * Pi
    auto totalArea = ScalarT(2) * capArea + sideArea;
    auto part = detail::uniform01<ScalarT>(rng) * totalArea;
    if (part < sideArea) // Uniform sampling on cylinder side
        return uniform(rng, boundary_no_caps_of(c));

    // Otherwise sampling on one of the caps
    auto capDisk = sphere<2, ScalarT, 3>(part < sideArea + capArea ? c.axis.pos0 : c.axis.pos1, c.radius, normalize(x));
    return uniform(rng, capDisk);
}

template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<3, ScalarT> uniform(Rng& rng, capsule<3, ScalarT> const& c)
{
    auto x = c.axis.pos1 - c.axis.pos0;
    auto h = length(x);
    auto tubeVolume = c.radius * c.radius * h;                                 // * Pi, but that does not matter here
    auto capVolume = ScalarT(2) / ScalarT(3) * c.radius * c.radius * c.radius; // * Pi
    auto totalVolume = ScalarT(2) * capVolume + tubeVolume;
    auto part = detail::uniform01<ScalarT>(rng) * totalVolume;
    if (part < tubeVolume) // Uniform sampling in capsule tube part
        return uniform(rng, cylinder<3, ScalarT>(c.axis, c.radius));

    // Otherwise sampling in one of the caps
    auto capHemi = hemisphere<3, ScalarT>();
    capHemi.radius = c.radius;
    capHemi.center = part < tubeVolume + capVolume ? c.axis.pos0 : c.axis.pos1;
    capHemi.normal = part < tubeVolume + capVolume ? -normalize(x) : normalize(x);
    return uniform(rng, capHemi);
}

template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<3, ScalarT> uniform(Rng& rng, capsule_boundary<3, ScalarT> const& c)
{
    auto x = c.axis.pos1 - c.axis.pos0;
    auto h = length(x);
    auto sideArea = ScalarT(2) * c.radius * h; // * Pi, but that does not matter here
    auto capArea = c.radius * c.radius;        // * Pi
    auto totalArea = ScalarT(2) * capArea + sideArea;
    auto part = detail::uniform01<ScalarT>(rng) * totalArea;
    if (part < sideArea) // Uniform sampling on capsule side
        return uniform(rng, cylinder_boundary_no_caps<3, ScalarT>(c.axis, c.radius));

    // Otherwise sampling on one of the caps
    auto capHemi = hemisphere_boundary_no_caps<3, ScalarT>();
    capHemi.radius = c.radius;
    capHemi.center = part < sideArea + capArea ? c.axis.pos0 : c.axis.pos1;
    capHemi.normal = part < sideArea + capArea ? -normalize(x) : normalize(x);
    return uniform(rng, capHemi);
}

template <int D, class ScalarT, class Rng>
[[nodiscard]] constexpr pos<D, ScalarT> uniform(Rng& rng, hemisphere<D, ScalarT> const& h)
{
    auto p = uniform(rng, sphere<D, ScalarT>(h.center, h.radius));
    auto v = p - h.center;
    if (dot(v, h.normal) >= ScalarT(0))
        return p;
    else
        return h.center - v;
}
template <int D, class ScalarT, class Rng>
[[nodiscard]] constexpr pos<D, ScalarT> uniform(Rng& rng, hemisphere_boundary_no_caps<D, ScalarT> const& h)
{
    auto p = uniform(rng, sphere_boundary<D, ScalarT>(h.center, h.radius));
    auto v = p - h.center;
    if (dot(v, h.normal) >= ScalarT(0))
        return p;
    else
        return h.center - v;
}
template <int D, class ScalarT, class Rng>
[[nodiscard]] constexpr pos<D, ScalarT> uniform(Rng& rng, hemisphere_boundary<D, ScalarT> const& h)
{
    ScalarT ratio;
    if constexpr (D == 1)
        ratio = ScalarT(0.5);
    if constexpr (D == 2)
        ratio = ScalarT(2) / (ScalarT(2) + pi_scalar<ScalarT>); // round length = pi * r, flat length = 2 * r => ratio pi : 2
    if constexpr (D == 3)
        ratio = ScalarT(1) / ScalarT(3); // round area = 2 * pi * r^2, flat area = pi * r^2 => ratio 2 : 1

    if (detail::uniform01<ScalarT>(rng) >= ratio)
        return uniform(rng, boundary_no_caps_of(h));

    return uniform(rng, caps_of(h));
}

template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<3, ScalarT> uniform(Rng& rng, cone_boundary<3, ScalarT> const& c)
{
    // base area = pi * r^^2, lateral area (mantle) = pi * r * sqrt(r^2 + h^2) => ratio r : sqrt(r^2 + h^2)
    auto r = c.base.radius;
    ScalarT ratio = r / (r + sqrt(pow2(r) + pow2(c.height)));
    if (detail::uniform01<ScalarT>(rng) >= ratio)
        return uniform(rng, boundary_no_caps_of(c));

    return uniform(rng, caps_of(c));
}

template <class ScalarT, class Rng>
[[nodiscard]] constexpr pos<3, ScalarT> uniform(Rng& rng, cone_boundary_no_caps<3, ScalarT> const& c)
{
    auto ub = aabb<2, ScalarT>::minus_one_to_one;
    while (true)
    {
        auto p = uniform_vec(rng, ub);
        auto l = length_sqr(p);
        if (l <= ScalarT(1))
        {
            p *= c.base.radius;
            auto x = any_normal(c.base.normal);
            auto y = cross(c.base.normal, x);
            return c.base.center + p.x * x + p.y * y + (ScalarT(1) - sqrt(l)) * c.base.normal * c.height;
        }
    }
}

// All solid pyramid variants
template <class BaseT, class Rng>
[[nodiscard]] constexpr pos<3, typename BaseT::scalar_t> uniform(Rng& rng, pyramid<BaseT> const& p)
{
    using ScalarT = typename BaseT::scalar_t;
    const auto n = normal_of(p.base);
    const auto h = ScalarT(1) - cbrt(detail::uniform01<ScalarT>(rng));
    const auto pBase = uniform(rng, p.base);
    return mix(pBase, centroid_of(p.base), h) + h * p.height * n;
}

// All boundary and boundary_no_caps pyramid variants except cones
template <class BaseT, class Rng>
[[nodiscard]] constexpr pos<3, typename BaseT::scalar_t> uniform(Rng& rng, pyramid_boundary<BaseT> const& py)
{
    const auto triangles = faces_of(boundary_no_caps_of(py));
    if constexpr (triangles.size() == 3)
        return uniform_by_area(rng, py.base, triangles[0], triangles[1], triangles[2]);
    else // verts.size() == 4
        return uniform_by_area(rng, py.base, triangles[0], triangles[1], triangles[2], triangles[3]);
}
template <class BaseT, class Rng>
[[nodiscard]] constexpr pos<3, typename BaseT::scalar_t> uniform(Rng& rng, pyramid_boundary_no_caps<BaseT> const& py)
{
    const auto triangles = faces_of(py);
    if constexpr (triangles.size() == 3)
        return uniform_by_area(rng, triangles[0], triangles[1], triangles[2]);
    else // verts.size() == 4
        return uniform_by_area(rng, triangles[0], triangles[1], triangles[2], triangles[3]);
}

template <int D, class ScalarT, class Rng, class = enable_if<is_floating_point<ScalarT>>>
[[deprecated("potentially misleading operation. use uniform_vec(rng, tg::aabb3(..)) or uniform_vec(rng, tg::segment3(..)) depending on your intended "
             "semantics")]] [[nodiscard]] constexpr vec<D, ScalarT>
uniform(Rng& rng, vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return mix(a, b, detail::uniform01<ScalarT>(rng));
}
template <int D, class ScalarT, class Rng, class = enable_if<is_floating_point<ScalarT>>>
[[deprecated("potentially misleading operation. use uniform(rng, tg::aabb3(..)) or uniform(rng, tg::segment3(..)) depending on your intended "
             "semantics")]] [[nodiscard]] constexpr pos<D, ScalarT>
uniform(Rng& rng, pos<D, ScalarT> const& a, pos<D, ScalarT> const& b)
{
    return mix(a, b, detail::uniform01<ScalarT>(rng));
}

template <class Rng, class... Args>
[[nodiscard]] constexpr auto uniform_vec(Rng& rng, Args const&... args) -> decltype(uniform(rng, args...) - decltype(uniform(rng, args...))::zero)
{
    return uniform(rng, args...) - decltype(uniform(rng, args...))::zero;
}

/// returns uniformly sampled barycentric coordinates
/// i.e. uniform(rng, triangle) has the same distribution as
///      triangle[uniform_barycoords(rng)]
template <class ScalarT = float, class Rng>
[[nodiscard]] constexpr comp<3, ScalarT> uniform_barycoords(Rng& rng)
{
    auto const a = detail::uniform01<ScalarT>(rng);
    auto const b = detail::uniform01<ScalarT>(rng);
    if (a + b <= ScalarT(1))
        return {a, b, 1 - a - b};
    else
        return {1 - a, 1 - b, a + b - 1};
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
template <>
struct sampler<std::byte>
{
    template <class Rng>
    constexpr static std::byte uniform(Rng& rng)
    {
        return std::byte(rng() & 0xFF);
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
        return color<3, T>(tg::uniform(rng, T(0), T(1)), tg::uniform(rng, T(0), T(1)), tg::uniform(rng, T(0), T(1)));
    }
};
template <class T>
struct sampler<color<4, T>>
{
    template <class Rng>
    constexpr static color<4, T> uniform(Rng& rng)
    {
        return color<4, T>(tg::uniform(rng, T(0), T(1)), tg::uniform(rng, T(0), T(1)), tg::uniform(rng, T(0), T(1)), tg::uniform(rng, T(0), T(1)));
    }
};
template <int D, class ScalarT>
struct sampler<dir<D, ScalarT>>
{
    template <class Rng>
    constexpr static dir<D, ScalarT> uniform(Rng& rng)
    {
        auto ub = aabb<D, ScalarT>::minus_one_to_one;
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
