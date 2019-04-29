#pragma once

#include "../detail/comp_traits.hh"
#include "../detail/macros.hh"
#include "../detail/scalar_traits.hh"
#include "../detail/utility.hh"
#include "fwd.hh"
#include "scalar.hh"

/**
 * TODO:
 * maybe no bvec?
 */

namespace tg
{
template <int D, class ScalarT>
struct vec;

// Common vector types

using vec1 = vec<1, f32>;
using vec2 = vec<2, f32>;
using vec3 = vec<3, f32>;
using vec4 = vec<4, f32>;

using bvec1 = vec<1, bool>;
using bvec2 = vec<2, bool>;
using bvec3 = vec<3, bool>;
using bvec4 = vec<4, bool>;

using fvec1 = vec<1, f32>;
using fvec2 = vec<2, f32>;
using fvec3 = vec<3, f32>;
using fvec4 = vec<4, f32>;

using dvec1 = vec<1, f64>;
using dvec2 = vec<2, f64>;
using dvec3 = vec<3, f64>;
using dvec4 = vec<4, f64>;

using ivec1 = vec<1, i32>;
using ivec2 = vec<2, i32>;
using ivec3 = vec<3, i32>;
using ivec4 = vec<4, i32>;

using uvec1 = vec<1, u32>;
using uvec2 = vec<2, u32>;
using uvec3 = vec<3, u32>;
using uvec4 = vec<4, u32>;


// Sized vector types

using i8vec1 = vec<1, i8>;
using i8vec2 = vec<2, i8>;
using i8vec3 = vec<3, i8>;
using i8vec4 = vec<4, i8>;

using i16vec1 = vec<1, i16>;
using i16vec2 = vec<2, i16>;
using i16vec3 = vec<3, i16>;
using i16vec4 = vec<4, i16>;

using i32vec1 = vec<1, i32>;
using i32vec2 = vec<2, i32>;
using i32vec3 = vec<3, i32>;
using i32vec4 = vec<4, i32>;

using i64vec1 = vec<1, i64>;
using i64vec2 = vec<2, i64>;
using i64vec3 = vec<3, i64>;
using i64vec4 = vec<4, i64>;

using u8vec1 = vec<1, u8>;
using u8vec2 = vec<2, u8>;
using u8vec3 = vec<3, u8>;
using u8vec4 = vec<4, u8>;

using u16vec1 = vec<1, u16>;
using u16vec2 = vec<2, u16>;
using u16vec3 = vec<3, u16>;
using u16vec4 = vec<4, u16>;

using u32vec1 = vec<1, u32>;
using u32vec2 = vec<2, u32>;
using u32vec3 = vec<3, u32>;
using u32vec4 = vec<4, u32>;

using u64vec1 = vec<1, u64>;
using u64vec2 = vec<2, u64>;
using u64vec3 = vec<3, u64>;
using u64vec4 = vec<4, u64>;

using f8vec1 = vec<1, f8>;
using f8vec2 = vec<2, f8>;
using f8vec3 = vec<3, f8>;
using f8vec4 = vec<4, f8>;

using f16vec1 = vec<1, f16>;
using f16vec2 = vec<2, f16>;
using f16vec3 = vec<3, f16>;
using f16vec4 = vec<4, f16>;

using f32vec1 = vec<1, f32>;
using f32vec2 = vec<2, f32>;
using f32vec3 = vec<3, f32>;
using f32vec4 = vec<4, f32>;

using f64vec1 = vec<1, f64>;
using f64vec2 = vec<2, f64>;
using f64vec3 = vec<3, f64>;
using f64vec4 = vec<4, f64>;


// ======== IMPLEMENTATION ========

template <class ScalarT>
struct vec<1, ScalarT>
{
    ScalarT x = static_cast<ScalarT>(0);

    static const vec zero;
    static const vec one;
    static const vec unit_x;

    constexpr ScalarT& operator[](int i) { return (&x)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&x)[i]; }

    constexpr vec() = default;
    constexpr vec(ScalarT v) : x(v) {}

    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>
    explicit constexpr vec(Obj const& v, ScalarT fill = ScalarT(0))
    {
        auto s = detail::get_dynamic_comp_size(v);
        x = detail::comp_get(v, 0, s, fill);
    }
};

template <class ScalarT>
struct vec<2, ScalarT>
{
    ScalarT x = static_cast<ScalarT>(0);
    ScalarT y = static_cast<ScalarT>(0);

    static const vec zero;
    static const vec one;
    static const vec unit_x;
    static const vec unit_y;

    constexpr ScalarT& operator[](int i) { return (&x)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&x)[i]; }

    constexpr vec() = default;
    constexpr explicit vec(ScalarT v) : x(v), y(v) {}
    constexpr vec(ScalarT x, ScalarT y) : x(x), y(y) {}

    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>
    explicit constexpr vec(Obj const& v, ScalarT fill = ScalarT(0))
    {
        auto s = detail::get_dynamic_comp_size(v);
        x = detail::comp_get(v, 0, s, fill);
        y = detail::comp_get(v, 1, s, fill);
    }
};

template <class ScalarT>
struct vec<3, ScalarT>
{
    ScalarT x = static_cast<ScalarT>(0);
    ScalarT y = static_cast<ScalarT>(0);
    ScalarT z = static_cast<ScalarT>(0);

    static const vec zero;
    static const vec one;
    static const vec unit_x;
    static const vec unit_y;
    static const vec unit_z;

    constexpr ScalarT& operator[](int i) { return (&x)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&x)[i]; }

    constexpr vec() = default;
    constexpr explicit vec(ScalarT v) : x(v), y(v), z(v) {}
    constexpr vec(ScalarT x, ScalarT y, ScalarT z) : x(x), y(y), z(z) {}

    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>
    explicit constexpr vec(Obj const& v, ScalarT fill = ScalarT(0))
    {
        auto s = detail::get_dynamic_comp_size(v);
        x = detail::comp_get(v, 0, s, fill);
        y = detail::comp_get(v, 1, s, fill);
        z = detail::comp_get(v, 2, s, fill);
    }
};

template <class ScalarT>
struct vec<4, ScalarT>
{
    ScalarT x = static_cast<ScalarT>(0);
    ScalarT y = static_cast<ScalarT>(0);
    ScalarT z = static_cast<ScalarT>(0);
    ScalarT w = static_cast<ScalarT>(0);

    static const vec zero;
    static const vec one;
    static const vec unit_x;
    static const vec unit_y;
    static const vec unit_z;
    static const vec unit_w;

    constexpr ScalarT& operator[](int i) { return (&x)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&x)[i]; }

    constexpr vec() = default;
    constexpr explicit vec(ScalarT v) : x(v), y(v), z(v), w(v) {}
    constexpr vec(ScalarT x, ScalarT y, ScalarT z, ScalarT w) : x(x), y(y), z(z), w(w) {}

    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>
    explicit constexpr vec(Obj const& v, ScalarT fill = ScalarT(0))
    {
        auto s = detail::get_dynamic_comp_size(v);
        x = detail::comp_get(v, 0, s, fill);
        y = detail::comp_get(v, 1, s, fill);
        z = detail::comp_get(v, 2, s, fill);
        w = detail::comp_get(v, 3, s, fill);
    }
};

// comparison operators
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(vec, vec, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(vec, vec, bool, operator!=, ||, !=);

// deduction guides
#ifdef TG_SUPPORT_CXX17
template <class A, class = enable_if<is_scalar<A>>>
vec(A const& x)->vec<1, A>;
template <class A, class B>
vec(A const& x, B const& y)->vec<2, promoted_scalar<A, B>>;
template <class A, class B, class C>
vec(A const& x, B const& y, C const& z)->vec<3, promoted_scalar<A, promoted_scalar<B, C>>>;
template <class A, class B, class C, class D>
vec(A const& x, B const& y, C const& z, D const& w)->vec<4, promoted_scalar<promoted_scalar<A, B>, promoted_scalar<C, D>>>;

template <int D, class T>
vec(pos<D, T> const&)->vec<D, T>;
template <int D, class T>
vec(size<D, T> const&)->vec<D, T>;
template <int D, class T>
vec(dir<D, T> const&)->vec<D, T>;
#endif

} // namespace tg
