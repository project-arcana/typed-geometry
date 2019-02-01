#pragma once

#include "../detail/macros.hh"
#include "../detail/scalar_traits.hh"
#include "../detail/utility.hh"
#include "scalar.hh"
#include "vec.hh"

namespace tg
{
template <int D, class ScalarT>
struct size;

// Common size types

using size1 = size<1, f32>;
using size2 = size<2, f32>;
using size3 = size<3, f32>;
using size4 = size<4, f32>;

using fsize1 = size<1, f32>;
using fsize2 = size<2, f32>;
using fsize3 = size<3, f32>;
using fsize4 = size<4, f32>;

using dsize1 = size<1, f64>;
using dsize2 = size<2, f64>;
using dsize3 = size<3, f64>;
using dsize4 = size<4, f64>;

using isize1 = size<1, i32>;
using isize2 = size<2, i32>;
using isize3 = size<3, i32>;
using isize4 = size<4, i32>;

using usize1 = size<1, u32>;
using usize2 = size<2, u32>;
using usize3 = size<3, u32>;
using usize4 = size<4, u32>;


// ======== IMPLEMENTATION ========

template <class ScalarT>
struct size<1, ScalarT>
{
    static const size zero;
    static const size ones;
    static const size unit;

    ScalarT width = static_cast<ScalarT>(0);

    constexpr ScalarT& operator[](int i) { return (&width)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&width)[i]; }

    constexpr size() = default;
    constexpr size(ScalarT v) : width(v) {}
    template <int D, class T, class = enable_if<D >= 1>>
    constexpr explicit size(size<D, T> const& v) : width(ScalarT(v.width))
    {
    }
    template <int D, class T, class = enable_if<D >= 1>>
    constexpr explicit size(vec<D, T> const& v) : width(ScalarT(v.x))
    {
    }
    template <int D, class T, class = enable_if<D <= 1>>
    constexpr explicit operator size<D, T>() const
    {
        return size<D, T>(*this);
    }
};

template <class ScalarT>
struct size<2, ScalarT>
{
    static const size zero;
    static const size ones;
    static const size unit;

    ScalarT width = static_cast<ScalarT>(0);
    ScalarT height = static_cast<ScalarT>(0);

    constexpr ScalarT& operator[](int i) { return (&width)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&width)[i]; }

    constexpr size() = default;
    constexpr explicit size(ScalarT v) : width(v), height(v) {}
    constexpr size(ScalarT width, ScalarT height) : width(width), height(height) {}
    template <int D, class T, class = enable_if<D >= 2>>
    constexpr explicit size(size<D, T> const& v) : width(ScalarT(v.width)), height(ScalarT(v.height))
    {
    }
    template <int D, class T, class = enable_if<D >= 2>>
    constexpr explicit size(vec<D, T> const& v) : width(ScalarT(v.x)), height(ScalarT(v.y))
    {
    }
    template <int D, class T, class = enable_if<D <= 2>>
    constexpr explicit operator size<D, T>() const
    {
        return size<D, T>(*this);
    }
};

template <class ScalarT>
struct size<3, ScalarT>
{
    static const size zero;
    static const size ones;
    static const size unit;

    ScalarT width = static_cast<ScalarT>(0);
    ScalarT height = static_cast<ScalarT>(0);
    ScalarT depth = static_cast<ScalarT>(0);

    constexpr ScalarT& operator[](int i) { return (&width)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&width)[i]; }

    constexpr size() = default;
    constexpr explicit size(ScalarT v) : width(v), height(v), depth(v) {}
    constexpr size(ScalarT width, ScalarT height, ScalarT depth) : width(width), height(height), depth(depth) {}
    template <int D, class T, class = enable_if<D >= 3>>
    constexpr explicit size(size<D, T> const& v) : width(ScalarT(v.width)), height(ScalarT(v.height)), depth(ScalarT(v.depth))
    {
    }
    template <int D, class T, class = enable_if<D >= 3>>
    constexpr explicit size(vec<D, T> const& v) : width(ScalarT(v.x)), height(ScalarT(v.y)), depth(ScalarT(v.z))
    {
    }
    template <int D, class T, class = enable_if<D <= 3>>
    constexpr explicit operator size<D, T>() const
    {
        return size<D, T>(*this);
    }
};

template <class ScalarT>
struct size<4, ScalarT>
{
    static const size zero;
    static const size ones;
    static const size unit;

    ScalarT width = static_cast<ScalarT>(0);
    ScalarT height = static_cast<ScalarT>(0);
    ScalarT depth = static_cast<ScalarT>(0);
    ScalarT w = static_cast<ScalarT>(0);

    constexpr ScalarT& operator[](int i) { return (&width)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&width)[i]; }

    constexpr size() = default;
    constexpr explicit size(ScalarT v) : width(v), height(v), depth(v), w(v) {}
    constexpr size(ScalarT width, ScalarT height, ScalarT depth, ScalarT w) : width(width), height(height), depth(depth), w(w) {}
    template <int D, class T, class = enable_if<D >= 4>>
    constexpr explicit size(size<D, T> const& v) : width(ScalarT(v.width)), height(ScalarT(v.height)), depth(ScalarT(v.depth)), w(ScalarT(v.w))
    {
    }
    template <int D, class T, class = enable_if<D >= 4>>
    constexpr explicit size(vec<D, T> const& v) : width(ScalarT(v.x)), height(ScalarT(v.y)), depth(ScalarT(v.z)), w(ScalarT(v.w))
    {
    }
    template <int D, class T, class = enable_if<D <= 4>>
    constexpr explicit operator size<D, T>() const
    {
        return size<D, T>(*this);
    }
};

// comparison operators
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(size, size, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(size, size, bool, operator!=, ||, !=);

// deduction guides
#ifdef TG_SUPPORT_CXX17
template <class A>
size(A const& x)->size<1, A>;
template <class A, class B>
size(A const& x, B const& y)->size<2, promoted_scalar<A, B>>;
template <class A, class B, class C>
size(A const& x, B const& y, C const& z)->size<3, promoted_scalar<A, promoted_scalar<B, C>>>;
template <class A, class B, class C, class D>
size(A const& x, B const& y, C const& z, D const& w)->size<4, promoted_scalar<promoted_scalar<A, B>, promoted_scalar<C, D>>>;
#endif
} // namespace tg
