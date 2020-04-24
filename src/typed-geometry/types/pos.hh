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
    ScalarT x = ScalarT(0);

    static const pos zero;

    TG_DECLARE_COMP_TYPE_1(pos);
};

template <class ScalarT>
struct pos<2, ScalarT>
{
    ScalarT x = ScalarT(0);
    ScalarT y = ScalarT(0);

    static const pos zero;

    TG_DECLARE_COMP_TYPE_2(pos);
};

template <class ScalarT>
struct pos<3, ScalarT>
{
    ScalarT x = ScalarT(0);
    ScalarT y = ScalarT(0);
    ScalarT z = ScalarT(0);

    static const pos zero;

    TG_DECLARE_COMP_TYPE_3(pos);
};

template <class ScalarT>
struct pos<4, ScalarT>
{
    ScalarT x = ScalarT(0);
    ScalarT y = ScalarT(0);
    ScalarT z = ScalarT(0);
    ScalarT w = ScalarT(0);

    static const pos zero;

    TG_DECLARE_COMP_TYPE_4(pos);
};

// comparison operators
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(pos, pos, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(pos, pos, bool, operator!=, ||, !=);

// deduction guides
TG_IMPL_COMP_DEDUCTION_GUIDES(pos);

// reflection
TG_IMPL_COMP_INTROSPECT(pos);

} // namespace tg
