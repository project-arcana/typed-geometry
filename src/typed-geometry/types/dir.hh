#pragma once

#include "../detail/macros.hh"
#include "../detail/scalar_traits.hh"
#include "../detail/utility.hh"
#include "fwd.hh"
#include "scalar.hh"

namespace tg
{
/// A direction, i.e. a normalized vector
template <int D, class ScalarT>
struct dir;

// Common direction types

using dir1 = dir<1, f32>;
using dir2 = dir<2, f32>;
using dir3 = dir<3, f32>;
using dir4 = dir<4, f32>;

using fdir1 = dir<1, f32>;
using fdir2 = dir<2, f32>;
using fdir3 = dir<3, f32>;
using fdir4 = dir<4, f32>;

using ddir1 = dir<1, f64>;
using ddir2 = dir<2, f64>;
using ddir3 = dir<3, f64>;
using ddir4 = dir<4, f64>;

using idir1 = dir<1, i32>;
using idir2 = dir<2, i32>;
using idir3 = dir<3, i32>;
using idir4 = dir<4, i32>;


// ======== IMPLEMENTATION ========

template <class ScalarT>
struct dir<1, ScalarT>
{
    static const dir pos_x;
    static const dir neg_x;

    ScalarT x;

    constexpr ScalarT& operator[](int i) { return (&x)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&x)[i]; }

    constexpr dir(ScalarT x);
    constexpr explicit dir(vec<1, ScalarT> const& v); // CAUTION: this does not normalize!
    template <class T>
    constexpr explicit dir(dir<1, T> const& v) : x(ScalarT(v.x))
    {
    }
    template <class T>
    constexpr explicit operator dir<1, T>() const
    {
        return dir<1, T>(*this);
    }
};

template <class ScalarT>
struct dir<2, ScalarT>
{
    ScalarT x;
    ScalarT y;

    static const dir pos_x;
    static const dir pos_y;
    static const dir neg_x;
    static const dir neg_y;

    constexpr ScalarT& operator[](int i) { return (&x)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&x)[i]; }

    constexpr dir(ScalarT x, ScalarT y);
    constexpr explicit dir(vec<2, ScalarT> const& v); // CAUTION: this does not normalize!
    template <class T>
    constexpr explicit dir(dir<2, T> const& v) : x(ScalarT(v.x)), y(ScalarT(v.y))
    {
    }
    template <class T>
    constexpr explicit operator dir<2, T>() const
    {
        return dir<2, T>(*this);
    }
};

template <class ScalarT>
struct dir<3, ScalarT>
{
    ScalarT x;
    ScalarT y;
    ScalarT z;

    static const dir pos_x;
    static const dir pos_y;
    static const dir pos_z;
    static const dir neg_x;
    static const dir neg_y;
    static const dir neg_z;

    constexpr ScalarT& operator[](int i) { return (&x)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&x)[i]; }

    constexpr dir(ScalarT x, ScalarT y, ScalarT z);
    constexpr explicit dir(vec<3, ScalarT> const& v); // CAUTION: this does not normalize!
    template <class T>
    constexpr explicit dir(dir<3, T> const& v) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(ScalarT(v.z))
    {
    }
    template <class T>
    constexpr explicit operator dir<3, T>() const
    {
        return dir<3, T>(*this);
    }
};

template <class ScalarT>
struct dir<4, ScalarT>
{
    ScalarT x = static_cast<ScalarT>(0);
    ScalarT y = static_cast<ScalarT>(0);
    ScalarT z = static_cast<ScalarT>(0);
    ScalarT w = static_cast<ScalarT>(0);

    static const dir pos_x;
    static const dir pos_y;
    static const dir pos_z;
    static const dir pos_w;
    static const dir neg_x;
    static const dir neg_y;
    static const dir neg_z;
    static const dir neg_w;

    constexpr ScalarT& operator[](int i) { return (&x)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&x)[i]; }

    constexpr dir(ScalarT x, ScalarT y, ScalarT z, ScalarT w);
    constexpr explicit dir(vec<4, ScalarT> const& v); // CAUTION: this does not normalize!
    template <class T>
    constexpr explicit dir(dir<4, T> const& v) : x(ScalarT(v.x)), y(ScalarT(v.y)), z(ScalarT(v.z)), w(ScalarT(v.w))
    {
    }
    template <class T>
    constexpr explicit operator dir<4, T>() const
    {
        return dir<4, T>(*this);
    }
};

// comparison operators
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(dir, dir, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(dir, dir, bool, operator!=, ||, !=);

// deduction guides
#ifdef TG_SUPPORT_CXX17
template <class A, class = enable_if<is_scalar<A>>>
dir(A const& x)->dir<1, A>;
template <class A, class B>
dir(A const& x, B const& y)->dir<2, promoted_scalar<A, B>>;
template <class A, class B, class C>
dir(A const& x, B const& y, C const& z)->dir<3, promoted_scalar<A, promoted_scalar<B, C>>>;
template <class A, class B, class C, class D>
dir(A const& x, B const& y, C const& z, D const& w)->dir<4, promoted_scalar<promoted_scalar<A, B>, promoted_scalar<C, D>>>;

template <int D, class T>
dir(vec<D, T> const&)->dir<D, T>;
#endif

} // namespace tg
