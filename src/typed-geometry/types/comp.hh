#pragma once

#include <typed-geometry/types/scalars/default.hh>

#include "../detail/comp_traits.hh"
#include "../detail/macros.hh"
#include "../detail/scalar_traits.hh"
#include "../detail/utility.hh"
#include "fwd.hh"

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

using bcomp1 = comp<1, bool>;
using bcomp2 = comp<2, bool>;
using bcomp3 = comp<3, bool>;
using bcomp4 = comp<4, bool>;


// ======== IMPLEMENTATION ========

template <class ScalarT>
struct comp<1, ScalarT>
{
    static const comp zero;

    ScalarT comp0 = ScalarT(0);

    TG_DECLARE_COMP_TYPE_1(comp);
};

template <class ScalarT>
struct comp<2, ScalarT>
{
    static const comp zero;

    ScalarT comp0 = ScalarT(0);
    ScalarT comp1 = ScalarT(0);

    TG_DECLARE_COMP_TYPE_2(comp);
};

template <class ScalarT>
struct comp<3, ScalarT>
{
    static const comp zero;

    ScalarT comp0 = ScalarT(0);
    ScalarT comp1 = ScalarT(0);
    ScalarT comp2 = ScalarT(0);

    TG_DECLARE_COMP_TYPE_3(comp);
};

template <class ScalarT>
struct comp<4, ScalarT>
{
    static const comp zero;

    ScalarT comp0 = ScalarT(0);
    ScalarT comp1 = ScalarT(0);
    ScalarT comp2 = ScalarT(0);
    ScalarT comp3 = ScalarT(0);

    TG_DECLARE_COMP_TYPE_4(comp);
};

// comparison operators
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(comp, comp, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(comp, comp, bool, operator!=, ||, !=);

// deduction guides
TG_IMPL_COMP_DEDUCTION_GUIDES(comp);

// reflection
TG_IMPL_COMP_INTROSPECT(comp);

} // namespace tg
