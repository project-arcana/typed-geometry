#pragma once

#include "../detail/comp_traits.hh"
#include "../detail/macros.hh"
#include "../detail/scalar_traits.hh"
#include "../detail/utility.hh"
#include "fwd.hh"
#include "scalar.hh"

namespace tg
{
template <int D, class ScalarT>
struct comp;

// Common N-dimensional component type

using comp1 = comp<1, f32>;
using comp2 = comp<2, f32>;
using comp3 = comp<3, f32>;
using comp4 = comp<4, f32>;

using fcomp1 = comp<1, f32>;
using fcomp2 = comp<2, f32>;
using fcomp3 = comp<3, f32>;
using fcomp4 = comp<4, f32>;

using dcomp1 = comp<1, f64>;
using dcomp2 = comp<2, f64>;
using dcomp3 = comp<3, f64>;
using dcomp4 = comp<4, f64>;

using icomp1 = comp<1, i32>;
using icomp2 = comp<2, i32>;
using icomp3 = comp<3, i32>;
using icomp4 = comp<4, i32>;

using ucomp1 = comp<1, u32>;
using ucomp2 = comp<2, u32>;
using ucomp3 = comp<3, u32>;
using ucomp4 = comp<4, u32>;


// ======== IMPLEMENTATION ========

template <class ScalarT>
struct comp<1, ScalarT>
{
    static const comp zero;

    ScalarT comp0 = ScalarT(0);

    constexpr ScalarT& operator[](int i) { return comp0; }
    constexpr ScalarT const& operator[](int i) const { return comp0; }

    constexpr comp() = default;
    constexpr comp(ScalarT v) : comp0(v) {}

    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>
    explicit constexpr comp(Obj const& v, ScalarT fill = ScalarT(0))
    {
        auto s = detail::get_dynamic_comp_size(v);
        comp0 = detail::comp_get(v, 0, s, fill);
    }
};

template <class ScalarT>
struct comp<2, ScalarT>
{
    static const comp zero;

    ScalarT comp0 = ScalarT(0);
    ScalarT comp1 = ScalarT(0);

    constexpr ScalarT& operator[](int i) { return (&comp0)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&comp0)[i]; }

    constexpr comp() = default;
    constexpr explicit comp(ScalarT v) : comp0(v), comp1(v) {}
    constexpr comp(ScalarT x, ScalarT y) : comp0(x), comp1(y) {}

    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>
    explicit constexpr comp(Obj const& v, ScalarT fill = ScalarT(0))
    {
        auto s = detail::get_dynamic_comp_size(v);
        comp0 = detail::comp_get(v, 0, s, fill);
        comp1 = detail::comp_get(v, 1, s, fill);
    }
};

template <class ScalarT>
struct comp<3, ScalarT>
{
    static const comp zero;

    ScalarT comp0 = ScalarT(0);
    ScalarT comp1 = ScalarT(0);
    ScalarT comp2 = ScalarT(0);

    constexpr ScalarT& operator[](int i) { return (&comp0)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&comp0)[i]; }

    constexpr comp() = default;
    constexpr explicit comp(ScalarT v) : comp0(v), comp1(v), comp2(v) {}
    constexpr comp(ScalarT x, ScalarT y, ScalarT z) : comp0(x), comp1(y), comp2(z) {}

    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>
    explicit constexpr comp(Obj const& v, ScalarT fill = ScalarT(0))
    {
        auto s = detail::get_dynamic_comp_size(v);
        comp0 = detail::comp_get(v, 0, s, fill);
        comp1 = detail::comp_get(v, 1, s, fill);
        comp2 = detail::comp_get(v, 2, s, fill);
    }
};

template <class ScalarT>
struct comp<4, ScalarT>
{
    static const comp zero;

    ScalarT comp0 = ScalarT(0);
    ScalarT comp1 = ScalarT(0);
    ScalarT comp2 = ScalarT(0);
    ScalarT comp3 = ScalarT(0);

    constexpr ScalarT& operator[](int i) { return (&comp0)[i]; }
    constexpr ScalarT const& operator[](int i) const { return (&comp0)[i]; }

    constexpr comp() = default;
    constexpr explicit comp(ScalarT v) : comp0(v), comp1(v), comp2(v), comp3(v) {}
    constexpr comp(ScalarT x, ScalarT y, ScalarT z, ScalarT w) : comp0(x), comp1(y), comp2(z), comp3(w) {}

    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>
    explicit constexpr comp(Obj const& v, ScalarT fill = ScalarT(0))
    {
        auto s = detail::get_dynamic_comp_size(v);
        comp0 = detail::comp_get(v, 0, s, fill);
        comp1 = detail::comp_get(v, 1, s, fill);
        comp2 = detail::comp_get(v, 2, s, fill);
        comp3 = detail::comp_get(v, 3, s, fill);
    }
};

// comparison operators
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(comp, comp, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(comp, comp, bool, operator!=, ||, !=);

// deduction guides
template <class A, class = enable_if<is_scalar<A>>>
comp(A const& x)->comp<1, A>;
template <class A, class B>
comp(A const& x, B const& y)->comp<2, promoted_scalar<A, B>>;
template <class A, class B, class C>
comp(A const& x, B const& y, C const& z)->comp<3, promoted_scalar<A, promoted_scalar<B, C>>>;
template <class A, class B, class C, class D>
comp(A const& x, B const& y, C const& z, D const& w)->comp<4, promoted_scalar<promoted_scalar<A, B>, promoted_scalar<C, D>>>;

template <int D, class T, template <int, class> class Obj>
comp(Obj<D, T> const&)->comp<D, T>;
} // namespace tg
