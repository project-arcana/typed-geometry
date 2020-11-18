#pragma once

#include <typed-geometry/types/scalars/default.hh>
#include "../pos.hh"
#include "../size.hh"
#include "../vec.hh"
#include "traits.hh"

// A segment is a line with defined start and end point
namespace tg
{
template <int D, class ScalarT>
struct segment;
// Common segment types

using segment1 = segment<1, f32>;
using segment2 = segment<2, f32>;
using segment3 = segment<3, f32>;
using segment4 = segment<4, f32>;

using fsegment1 = segment<1, f32>;
using fsegment2 = segment<2, f32>;
using fsegment3 = segment<3, f32>;
using fsegment4 = segment<4, f32>;

using dsegment1 = segment<1, f64>;
using dsegment2 = segment<2, f64>;
using dsegment3 = segment<3, f64>;
using dsegment4 = segment<4, f64>;

using isegment1 = segment<1, i32>;
using isegment2 = segment<2, i32>;
using isegment3 = segment<3, i32>;
using isegment4 = segment<4, i32>;

using usegment1 = segment<1, u32>;
using usegment2 = segment<2, u32>;
using usegment3 = segment<3, u32>;
using usegment4 = segment<4, u32>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT>
struct segment
{
    using scalar_t = ScalarT;
    using pos_t = pos<D, ScalarT>;

    pos_t pos0;
    pos_t pos1;

    constexpr segment() = default;
    constexpr segment(pos_t p0, pos_t p1) : pos0(p0), pos1(p1) {}

    template <class OtherT>
    explicit constexpr segment(segment<D, OtherT> const& v) : pos0(v.pos0), pos1(v.pos1)
    {
    }

    template <class Range, class = std::enable_if_t<tg::is_range<Range, pos_t>>>
    explicit constexpr segment(Range&& r)
    {
        auto it = tg::begin(r);
        auto end = tg::end(r);
        TG_CONTRACT(it != end);
        pos0 = pos_t(*it++);
        TG_CONTRACT(it != end);
        pos1 = pos_t(*it++);
        TG_CONTRACT(!(it != end));
    }

    [[nodiscard]] constexpr pos_t operator[](ScalarT t) const;

    [[nodiscard]] bool operator==(segment const& rhs) const { return pos0 == rhs.pos0 && pos1 == rhs.pos1; }
    [[nodiscard]] bool operator!=(segment const& rhs) const { return !operator==(rhs); }
};

template <class I, int D, class ScalarT>
constexpr void introspect(I&& i, segment<D, ScalarT>& v)
{
    i(v.pos0, "pos0");
    i(v.pos1, "pos1");
}

template <int D, class ScalarT>
struct object_traits<segment<D, ScalarT>> : detail::finite_object_traits<1, ScalarT, D, default_object_tag>
{
};
} // namespace tg
