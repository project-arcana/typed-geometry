#pragma once

#include <typed-geometry/types/scalars/default.hh>
#include "../detail/comp_traits.hh"
#include "../detail/macros.hh"
#include "../detail/scalar_traits.hh"
#include "../detail/utility.hh"
#include "fwd.hh"
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
    ScalarT width = ScalarT(0);

    static const size zero;
    static const size ones;
    static const size unit;

    TG_DECLARE_COMP_TYPE_1(size);
};

template <class ScalarT>
struct size<2, ScalarT>
{
    ScalarT width = ScalarT(0);
    ScalarT height = ScalarT(0);

    static const size zero;
    static const size ones;
    static const size unit;

    TG_DECLARE_COMP_TYPE_2(size);
};

template <class ScalarT>
struct size<3, ScalarT>
{
    ScalarT width = ScalarT(0);
    ScalarT height = ScalarT(0);
    ScalarT depth = ScalarT(0);

    static const size zero;
    static const size ones;
    static const size unit;

    TG_DECLARE_COMP_TYPE_3(size);
};

template <class ScalarT>
struct size<4, ScalarT>
{
    ScalarT width = ScalarT(0);
    ScalarT height = ScalarT(0);
    ScalarT depth = ScalarT(0);
    ScalarT w = ScalarT(0);

    static const size zero;
    static const size ones;
    static const size unit;

    TG_DECLARE_COMP_TYPE_4(size);
};

// comparison operators
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(size, size, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(size, size, bool, operator!=, ||, !=);

// deduction guides
TG_IMPL_COMP_DEDUCTION_GUIDES(size);

// reflection
TG_IMPL_COMP_INTROSPECT(size);

} // namespace tg
