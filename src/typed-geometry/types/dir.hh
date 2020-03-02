#pragma once

#include <typed-geometry/feature/assert.hh>

#include "../detail/comp_traits.hh"
#include "../detail/macros.hh"
#include "../detail/scalar_traits.hh"
#include "../detail/utility.hh"

#include <typed-geometry/types/scalars/default.hh>
#include "fwd.hh"

namespace tg
{
/// A direction, i.e. a normalized vector
///
/// It is always a normalized dir
///
/// NOTE: dir has a default ctor for ergonomic purposes but computations with non-normalized dirs are undefined behavior
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

namespace detail
{
template <class ScalarT>
bool is_dir_valid(dir<1, ScalarT> const& d)
{
    if constexpr (is_abstract_scalar<ScalarT>)
        return true;
    else
    {
        auto const l = d.x * d.x;
        return ScalarT(0.99) <= l && l <= ScalarT(1.01);
    }
}
template <class ScalarT>
bool is_dir_valid(dir<2, ScalarT> const& d)
{
    if constexpr (is_abstract_scalar<ScalarT>)
        return true;
    else
    {
        auto const l = d.x * d.x + d.y * d.y;
        return ScalarT(0.99) <= l && l <= ScalarT(1.01);
    }
}
template <class ScalarT>
bool is_dir_valid(dir<3, ScalarT> const& d)
{
    if constexpr (is_abstract_scalar<ScalarT>)
        return true;
    else
    {
        auto const l = d.x * d.x + d.y * d.y + d.z * d.z;
        return ScalarT(0.99) <= l && l <= ScalarT(1.01);
    }
}
template <class ScalarT>
bool is_dir_valid(dir<4, ScalarT> const& d)
{
    if constexpr (is_abstract_scalar<ScalarT>)
        return true;
    else
    {
        auto const l = d.x * d.x + d.y * d.y + d.z * d.z + d.w * d.w;
        return ScalarT(0.99) <= l && l <= ScalarT(1.01);
    }
}
}

template <class ScalarT>
struct dir<1, ScalarT>
{
    ScalarT x = ScalarT(0);

    static const dir pos_x;
    static const dir neg_x;

    TG_DECLARE_COMP_TYPE_1(dir);

    template <class T>
    constexpr explicit operator dir<1, T>() const
    {
        return {T(x)};
    }

    constexpr operator vec<1, ScalarT>() const { return {x}; }
};

template <class ScalarT>
struct dir<2, ScalarT>
{
    ScalarT x = ScalarT(0);
    ScalarT y = ScalarT(0);

    static const dir pos_x;
    static const dir pos_y;
    static const dir neg_x;
    static const dir neg_y;

    TG_DECLARE_COMP_TYPE_2(dir);

    template <class T>
    constexpr explicit operator dir<2, T>() const
    {
        return {T(x), T(y)};
    }

    constexpr operator vec<2, ScalarT>() const { return {x, y}; }
};

template <class ScalarT>
struct dir<3, ScalarT>
{
    ScalarT x = ScalarT(0);
    ScalarT y = ScalarT(0);
    ScalarT z = ScalarT(0);

    static const dir pos_x;
    static const dir pos_y;
    static const dir pos_z;
    static const dir neg_x;
    static const dir neg_y;
    static const dir neg_z;

    TG_DECLARE_COMP_TYPE_3(dir);

    template <class T>
    constexpr explicit operator dir<3, T>() const
    {
        return {T(x), T(y), T(z)};
    }

    constexpr operator vec<3, ScalarT>() const { return {x, y, z}; }
};

template <class ScalarT>
struct dir<4, ScalarT>
{
    ScalarT x = ScalarT(0);
    ScalarT y = ScalarT(0);
    ScalarT z = ScalarT(0);
    ScalarT w = ScalarT(0);

    static const dir pos_x;
    static const dir pos_y;
    static const dir pos_z;
    static const dir pos_w;
    static const dir neg_x;
    static const dir neg_y;
    static const dir neg_z;
    static const dir neg_w;

    TG_DECLARE_COMP_TYPE_4(dir);

    template <class T>
    constexpr explicit operator dir<4, T>() const
    {
        return {T(x), T(y), T(z), T(w)};
    }

    constexpr operator vec<4, ScalarT>() const { return {x, y, z, w}; }
};

// non-deducable vec or dir
template <int D, class ScalarT>
using vec_or_dir = dont_deduce<vec<D, ScalarT>>;

// comparison operators
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(dir, dir, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(dir, dir, bool, operator!=, ||, !=);

// deduction guides
TG_IMPL_COMP_DEDUCTION_GUIDES(dir);

// reflection
TG_IMPL_COMP_INTROSPECT(dir);

} // namespace tg
