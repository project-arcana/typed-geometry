#pragma once

#include <typed-geometry/types/scalars/default.hh>
#include "../dir.hh"
#include "../pos.hh"
#include "traits.hh"

// A line has a direction and a point lying on it
// The direction is assumed to be normalized
namespace tg
{
template <int D, class ScalarT>
struct line;

// Common line types

using line1 = line<1, f32>;
using line2 = line<2, f32>;
using line3 = line<3, f32>;
using line4 = line<4, f32>;

using fline1 = line<1, f32>;
using fline2 = line<2, f32>;
using fline3 = line<3, f32>;
using fline4 = line<4, f32>;

using dline1 = line<1, f64>;
using dline2 = line<2, f64>;
using dline3 = line<3, f64>;
using dline4 = line<4, f64>;

using iline1 = line<1, i32>;
using iline2 = line<2, i32>;
using iline3 = line<3, i32>;
using iline4 = line<4, i32>;

using uline1 = line<1, u32>;
using uline2 = line<2, u32>;
using uline3 = line<3, u32>;
using uline4 = line<4, u32>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT>
struct line
{
    using scalar_t = ScalarT;
    using dir_t = tg::dir<D, ScalarT>; // Note: gcc requires the tg:: prefix to distinguish from member variable
    using pos_t = tg::pos<D, ScalarT>;

    pos_t pos;
    dir_t dir;

    constexpr line() = default;
    constexpr line(pos_t p, dir_t dir) : pos(p), dir(dir) {}

    template <class OtherT>
    explicit constexpr line(line<D, OtherT> const& v) : pos(v.pos), dir(v.dir)
    {
    }

    /// constructs a line such that
    /// pos = a
    /// dir = normalize(b - a)
    static constexpr line from_points(pos_t a, pos_t b);

    [[nodiscard]] constexpr pos_t operator[](ScalarT t) const;

    [[nodiscard]] bool operator==(line const& rhs) const { return pos == rhs.pos && dir == rhs.dir; }
    [[nodiscard]] bool operator!=(line const& rhs) const { return !operator==(rhs); }
};

template <class I, int D, class ScalarT>
constexpr void introspect(I&& i, line<D, ScalarT>& v)
{
    i(v.pos, "pos");
    i(v.dir, "dir");
}

template <int D, class ScalarT>
struct object_traits<line<D, ScalarT>> : detail::infinite_object_traits<1, ScalarT, D, default_object_tag>
{
};
} // namespace tg
