#pragma once

#include "../detail/macros.hh"
#include "../detail/utility.hh"
#include "scalar.hh"

/**
 * TODO:
 * maybe move scalar_t and shape into traits
 * maybe move zero/one/etc into constants
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

template <int D, class ScalarT>
struct vec
{
};

template <class ScalarT>
struct vec<1, ScalarT>
{
    using scalar_t = ScalarT;

    static const vec zero;
    static const vec one;
    static const vec unit_x;

    scalar_t x = static_cast<scalar_t>(0);

    constexpr scalar_t& operator[](int i) { return (&x)[i]; }
    constexpr scalar_t const& operator[](int i) const { return (&x)[i]; }

    constexpr vec() = default;
    constexpr vec(scalar_t v) : x(v) {}
    template <int D, class T, class = enable_if<D >= 1>>
    constexpr explicit vec(vec<D, T> const& v, scalar_t = ScalarT(0)) : x(ScalarT(v.x))
    {
    }
    template <int D, class T>
    constexpr explicit operator vec<D, T>() const
    {
        return vec<D, T>(*this);
    }
};
template <class ScalarT>
constexpr vec<1, ScalarT> vec<1, ScalarT>::zero = {ScalarT(0)};
template <class ScalarT>
constexpr vec<1, ScalarT> vec<1, ScalarT>::one = {ScalarT(1)};
template <class ScalarT>
constexpr vec<1, ScalarT> vec<1, ScalarT>::unit_x = {ScalarT(1)};

template <class ScalarT>
struct vec<2, ScalarT>
{
    using scalar_t = ScalarT;

    scalar_t x = static_cast<scalar_t>(0);
    scalar_t y = static_cast<scalar_t>(0);

    static const vec zero;
    static const vec one;
    static const vec unit_x;
    static const vec unit_y;

    constexpr scalar_t& operator[](int i) { return (&x)[i]; }
    constexpr scalar_t const& operator[](int i) const { return (&x)[i]; }

    constexpr vec() = default;
    constexpr explicit vec(scalar_t v) : x(v), y(v) {}
    constexpr vec(scalar_t x, scalar_t y) : x(x), y(y) {}
    template <int D, class T, class = enable_if<D >= 2>>
    constexpr explicit vec(vec<D, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y))
    {
    }
    template <class T>
    constexpr explicit vec(vec<1, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(fill)
    {
    }
    template <int D, class T>
    constexpr explicit operator vec<D, T>() const
    {
        return vec<D, T>(*this);
    }
};
template <class ScalarT>
constexpr vec<2, ScalarT> vec<2, ScalarT>::zero = {ScalarT(0), ScalarT(0)};
template <class ScalarT>
constexpr vec<2, ScalarT> vec<2, ScalarT>::one = {ScalarT(1), ScalarT(1)};
template <class ScalarT>
constexpr vec<2, ScalarT> vec<2, ScalarT>::unit_x = {ScalarT(1), ScalarT(0)};
template <class ScalarT>
constexpr vec<2, ScalarT> vec<2, ScalarT>::unit_y = {ScalarT(0), ScalarT(1)};

template <class ScalarT>
struct vec<3, ScalarT>
{
    using scalar_t = ScalarT;

    scalar_t x = static_cast<scalar_t>(0);
    scalar_t y = static_cast<scalar_t>(0);
    scalar_t z = static_cast<scalar_t>(0);

    static const vec zero;
    static const vec one;
    static const vec unit_x;
    static const vec unit_y;
    static const vec unit_z;

    constexpr scalar_t& operator[](int i) { return (&x)[i]; }
    constexpr scalar_t const& operator[](int i) const { return (&x)[i]; }

    constexpr vec() = default;
    constexpr explicit vec(scalar_t v) : x(v), y(v), z(v) {}
    constexpr vec(scalar_t x, scalar_t y, scalar_t z) : x(x), y(y), z(z) {}
    template <int D, class T, class = enable_if<D >= 3>>
    constexpr explicit vec(vec<D, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(ScalarT(v.z))
    {
    }
    template <class T>
    constexpr explicit vec(vec<2, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(fill)
    {
    }
    template <class T>
    constexpr explicit vec(vec<1, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(fill), z(fill)
    {
    }
    template <int D, class T, class = enable_if<D <= 3>>
    constexpr explicit operator vec<D, T>() const
    {
        return vec<D, T>(*this);
    }
};
template <class ScalarT>
constexpr vec<3, ScalarT> vec<3, ScalarT>::zero = {ScalarT(0), ScalarT(0), ScalarT(0)};
template <class ScalarT>
constexpr vec<3, ScalarT> vec<3, ScalarT>::one = {ScalarT(1), ScalarT(1), ScalarT(1)};
template <class ScalarT>
constexpr vec<3, ScalarT> vec<3, ScalarT>::unit_x = {ScalarT(1), ScalarT(0), ScalarT(0)};
template <class ScalarT>
constexpr vec<3, ScalarT> vec<3, ScalarT>::unit_y = {ScalarT(0), ScalarT(1), ScalarT(0)};
template <class ScalarT>
constexpr vec<3, ScalarT> vec<3, ScalarT>::unit_z = {ScalarT(0), ScalarT(0), ScalarT(1)};

template <class ScalarT>
struct vec<4, ScalarT>
{
    using scalar_t = ScalarT;

    static const vec zero;
    static const vec one;
    static const vec unit_x;
    static const vec unit_y;
    static const vec unit_z;
    static const vec unit_w;

    scalar_t x = static_cast<scalar_t>(0);
    scalar_t y = static_cast<scalar_t>(0);
    scalar_t z = static_cast<scalar_t>(0);
    scalar_t w = static_cast<scalar_t>(0);

    constexpr scalar_t& operator[](int i) { return (&x)[i]; }
    constexpr scalar_t const& operator[](int i) const { return (&x)[i]; }

    constexpr vec() = default;
    constexpr explicit vec(scalar_t v) : x(v), y(v), z(v), w(v) {}
    constexpr vec(scalar_t x, scalar_t y, scalar_t z, scalar_t w) : x(x), y(y), z(z), w(w) {}
    template <int D, class T, class = enable_if<D >= 4>>
    constexpr explicit vec(vec<D, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(ScalarT(v.z)), w(ScalarT(v.w))
    {
    }
    template <class T>
    constexpr explicit vec(vec<3, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(ScalarT(v.z)), w(fill)
    {
    }
    template <class T>
    constexpr explicit vec(vec<2, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(fill), w(fill)
    {
    }
    template <class T>
    constexpr explicit vec(vec<1, T> const& v, scalar_t fill = ScalarT(0)) : x(ScalarT(v.x)), y(fill), z(fill), w(fill)
    {
    }
    template <int D, class T, class = enable_if<D <= 4>>
    constexpr explicit operator vec<D, T>() const
    {
        return vec<D, T>(*this);
    }
};
template <class ScalarT>
constexpr vec<4, ScalarT> vec<4, ScalarT>::zero = {ScalarT(0), ScalarT(0), ScalarT(0), ScalarT(0)};
template <class ScalarT>
constexpr vec<4, ScalarT> vec<4, ScalarT>::one = {ScalarT(1), ScalarT(1), ScalarT(1), ScalarT(1)};
template <class ScalarT>
constexpr vec<4, ScalarT> vec<4, ScalarT>::unit_x = {ScalarT(1), ScalarT(0), ScalarT(0), ScalarT(0)};
template <class ScalarT>
constexpr vec<4, ScalarT> vec<4, ScalarT>::unit_y = {ScalarT(0), ScalarT(1), ScalarT(0), ScalarT(0)};
template <class ScalarT>
constexpr vec<4, ScalarT> vec<4, ScalarT>::unit_z = {ScalarT(0), ScalarT(0), ScalarT(1), ScalarT(0)};
template <class ScalarT>
constexpr vec<4, ScalarT> vec<4, ScalarT>::unit_w = {ScalarT(0), ScalarT(0), ScalarT(0), ScalarT(1)};

// comparison operators
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(vec, vec, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(vec, vec, bool, operator!=, ||, !=);

// deduction guides
#ifdef TG_SUPPORT_CXX17
template <class T>
vec(T const& x)->vec<1, T>;
template <class T>
vec(T const& x, T const& y)->vec<2, T>;
template <class T>
vec(T const& x, T const& y, T const& z)->vec<3, T>;
template <class T>
vec(T const& x, T const& y, T const& z, T const& w)->vec<4, T>;
#endif

} // namespace tg
