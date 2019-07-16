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

    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>
    explicit constexpr pos(Obj const& v, ScalarT fill = ScalarT(0))
    {
        auto s = detail::get_dynamic_comp_size(v);
        x = detail::comp_get(v, 0, s, fill);
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

    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>
    explicit constexpr pos(Obj const& v, ScalarT fill = ScalarT(0))
    {
        auto s = detail::get_dynamic_comp_size(v);
        x = detail::comp_get(v, 0, s, fill);
        y = detail::comp_get(v, 1, s, fill);
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

    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>
    explicit constexpr pos(Obj const& v, ScalarT fill = ScalarT(0))
    {
        auto s = detail::get_dynamic_comp_size(v);
        x = detail::comp_get(v, 0, s, fill);
        y = detail::comp_get(v, 1, s, fill);
        z = detail::comp_get(v, 2, s, fill);
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

    template <class Obj, class = enable_if<is_comp_convertible<Obj, ScalarT>>>
    explicit constexpr pos(Obj const& v, ScalarT fill = ScalarT(0))
    {
        auto s = detail::get_dynamic_comp_size(v);
        x = detail::comp_get(v, 0, s, fill);
        y = detail::comp_get(v, 1, s, fill);
        z = detail::comp_get(v, 2, s, fill);
        w = detail::comp_get(v, 3, s, fill);
    }
};

// comparison operators
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(pos, pos, bool, operator==, &&, ==);
TG_IMPL_DEFINE_REDUCTION_OP_BINARY(pos, pos, bool, operator!=, ||, !=);

// deduction guides
TG_IMPL_COMP_DEDUCTION_GUIDES(pos);

} // namespace tg
