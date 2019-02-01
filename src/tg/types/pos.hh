#pragma once

#include "../detail/macros.hh"
#include "../detail/scalar_traits.hh"
#include "../detail/utility.hh"
#include "scalar.hh"

namespace tg
{
template <int D, class ScalarT>
struct pos;

// Common position types

using pos1 = pos<1, f32>;
using pos2 = pos<2, f32>;
using pos3 = pos<3, f32>;
using pos4 = pos<4, f32>;

using fpos1 = pos<1, f32>;
using fpos2 = pos<2, f32>;
using fpos3 = pos<3, f32>;
using fpos4 = pos<4, f32>;

using dpos1 = pos<1, f64>;
using dpos2 = pos<2, f64>;
using dpos3 = pos<3, f64>;
using dpos4 = pos<4, f64>;

using ipos1 = pos<1, i32>;
using ipos2 = pos<2, i32>;
using ipos3 = pos<3, i32>;
using ipos4 = pos<4, i32>;

using upos1 = pos<1, u32>;
using upos2 = pos<2, u32>;
using upos3 = pos<3, u32>;
using upos4 = pos<4, u32>;


// ======== IMPLEMENTATION ========

template <class ScalarT>
struct pos<1, ScalarT>
{
    static const pos zero;

    ScalarT x = static_cast<ScalarT>(0);

    constexpr ScalarT& operator[](int i) { return (&x)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&x)[i]; }

    constexpr pos() = default;
    constexpr pos(ScalarT v) : x(v) {}
    template <int D, class T, class = enable_if<D >= 1>>
    constexpr explicit pos(pos<D, T> const& v, ScalarT = ScalarT(0)) : x(ScalarT(v.x))
    {
    }
    template <int D, class T>
    constexpr explicit operator pos<D, T>() const
    {
        return pos<D, T>(*this);
    }
};

template <class ScalarT>
struct pos<2, ScalarT>
{
    ScalarT x = static_cast<ScalarT>(0);
    ScalarT y = static_cast<ScalarT>(0);

    static const pos zero;

    constexpr ScalarT& operator[](int i) { return (&x)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&x)[i]; }

    constexpr pos() = default;
    constexpr explicit pos(ScalarT v) : x(v), y(v) {}
    constexpr pos(ScalarT x, ScalarT y) : x(x), y(y) {}
    template <int D, class T, class = enable_if<D >= 2>>
    constexpr explicit pos(pos<D, T> const& v, ScalarT fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y))
    {
    }
    template <class T>
    constexpr explicit pos(pos<1, T> const& v, ScalarT fill = ScalarT(0)) : x(ScalarT(v.x)), y(fill)
    {
    }
    template <int D, class T>
    constexpr explicit operator pos<D, T>() const
    {
        return pos<D, T>(*this);
    }
};

template <class ScalarT>
struct pos<3, ScalarT>
{
    ScalarT x = static_cast<ScalarT>(0);
    ScalarT y = static_cast<ScalarT>(0);
    ScalarT z = static_cast<ScalarT>(0);

    static const pos zero;

    constexpr ScalarT& operator[](int i) { return (&x)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&x)[i]; }

    constexpr pos() = default;
    constexpr explicit pos(ScalarT v) : x(v), y(v), z(v) {}
    constexpr pos(ScalarT x, ScalarT y, ScalarT z) : x(x), y(y), z(z) {}
    template <int D, class T, class = enable_if<D >= 3>>
    constexpr explicit pos(pos<D, T> const& v, ScalarT fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(ScalarT(v.z))
    {
    }
    template <class T>
    constexpr explicit pos(pos<2, T> const& v, ScalarT fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(fill)
    {
    }
    template <class T>
    constexpr explicit pos(pos<1, T> const& v, ScalarT fill = ScalarT(0)) : x(ScalarT(v.x)), y(fill), z(fill)
    {
    }
    template <int D, class T, class = enable_if<D <= 3>>
    constexpr explicit operator pos<D, T>() const
    {
        return pos<D, T>(*this);
    }
};

template <class ScalarT>
struct pos<4, ScalarT>
{
    static const pos zero;

    ScalarT x = static_cast<ScalarT>(0);
    ScalarT y = static_cast<ScalarT>(0);
    ScalarT z = static_cast<ScalarT>(0);
    ScalarT w = static_cast<ScalarT>(0);

    constexpr ScalarT& operator[](int i) { return (&x)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&x)[i]; }

    constexpr pos() = default;
    constexpr explicit pos(ScalarT v) : x(v), y(v), z(v), w(v) {}
    constexpr pos(ScalarT x, ScalarT y, ScalarT z, ScalarT w) : x(x), y(y), z(z), w(w) {}
    template <int D, class T, class = enable_if<D >= 4>>
    constexpr explicit pos(pos<D, T> const& v, ScalarT fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(ScalarT(v.z)), w(ScalarT(v.w))
    {
    }
    template <class T>
    constexpr explicit pos(pos<3, T> const& v, ScalarT fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(ScalarT(v.z)), w(fill)
    {
    }
    template <class T>
    constexpr explicit pos(pos<2, T> const& v, ScalarT fill = ScalarT(0)) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(fill), w(fill)
    {
    }
    template <class T>
    constexpr explicit pos(pos<1, T> const& v, ScalarT fill = ScalarT(0)) : x(ScalarT(v.x)), y(fill), z(fill), w(fill)
    {
    }
    template <int D, class T, class = enable_if<D <= 4>>
    constexpr explicit operator pos<D, T>() const
    {
        return pos<D, T>(*this);
    }
};

// comparison operators
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(pos, pos, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(pos, pos, bool, operator!=, ||, !=);

// deduction guides
#ifdef TG_SUPPORT_CXX17
template <class A>
pos(A const& x)->pos<1, A>;
template <class A, class B>
pos(A const& x, B const& y)->pos<2, promoted_scalar<A, B>>;
template <class A, class B, class C>
pos(A const& x, B const& y, C const& z)->pos<3, promoted_scalar<A, promoted_scalar<B, C>>>;
template <class A, class B, class C, class D>
pos(A const& x, B const& y, C const& z, D const& w)->pos<4, promoted_scalar<promoted_scalar<A, B>, promoted_scalar<C, D>>>;
#endif

} // namespace tg
