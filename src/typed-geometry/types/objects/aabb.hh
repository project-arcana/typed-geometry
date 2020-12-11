#pragma once

#include <typed-geometry/feature/assert.hh>

#include <typed-geometry/types/scalars/default.hh>
#include "../pos.hh"
#include "../vec.hh"
#include "traits.hh"

namespace tg
{
template <int D, class ScalarT, class TraitsT = default_object_tag>
struct aabb;

// Common aabb types

using aabb1 = aabb<1, f32>;
using aabb2 = aabb<2, f32>;
using aabb3 = aabb<3, f32>;
using aabb4 = aabb<4, f32>;

using faabb1 = aabb<1, f32>;
using faabb2 = aabb<2, f32>;
using faabb3 = aabb<3, f32>;
using faabb4 = aabb<4, f32>;

using daabb1 = aabb<1, f64>;
using daabb2 = aabb<2, f64>;
using daabb3 = aabb<3, f64>;
using daabb4 = aabb<4, f64>;

using iaabb1 = aabb<1, i32>;
using iaabb2 = aabb<2, i32>;
using iaabb3 = aabb<3, i32>;
using iaabb4 = aabb<4, i32>;

using uaabb1 = aabb<1, u32>;
using uaabb2 = aabb<2, u32>;
using uaabb3 = aabb<3, u32>;
using uaabb4 = aabb<4, u32>;

template <int D, class ScalarT>
using aabb_boundary = aabb<D, ScalarT, boundary_tag>;


// ======== IMPLEMENTATION ========

template <int D, class ScalarT, class TraitsT>
struct aabb
{
    using scalar_t = ScalarT;
    using vec_t = vec<D, ScalarT>;
    using pos_t = pos<D, ScalarT>;

    static const aabb minus_one_to_one;
    static const aabb unit_from_zero;
    static const aabb unit_centered;

    pos_t min;
    pos_t max;

    constexpr aabb() = default;
    explicit constexpr aabb(ScalarT min, ScalarT max) : min(min), max(max) { TG_CONTRACT(min <= max); }
    constexpr aabb(pos_t min, pos_t max) : min(min), max(max)
    {
        for (auto i = 0; i < D; ++i)
            TG_CONTRACT(min[i] <= max[i]);
    }

    template <class OtherT, class OtherTraitsT>
    explicit constexpr aabb(aabb<D, OtherT, OtherTraitsT> const& v) : min(v.min), max(v.max)
    {
    }

    template <class OtherT>
    constexpr aabb(pos<D, OtherT> min, pos<D, OtherT> max) : aabb(pos_t(min), pos_t(max))
    {
    }

    template <class OtherT>
    constexpr aabb(pos<D, OtherT> min, size<D, OtherT> size) : min(min)
    {
        for (auto i = 0; i < D; ++i)
            TG_CONTRACT(ScalarT(size[i]) >= ScalarT(0));

        this->max.x = ScalarT(min.x + size.width);
        if constexpr (D >= 2)
            this->max.y = ScalarT(min.y + size.height);
        if constexpr (D >= 3)
            this->max.z = ScalarT(min.z + size.depth);
        if constexpr (D >= 4)
            this->max.w = ScalarT(min.w + size.w);
    }

    [[nodiscard]] constexpr pos_t operator[](tg::comp<D, ScalarT> const& c) const;

    [[nodiscard]] bool operator==(aabb const& rhs) const { return min == rhs.min && max == rhs.max; }
    [[nodiscard]] bool operator!=(aabb const& rhs) const { return !operator==(rhs); }
};

template <class I, int D, class ScalarT, class TraitsT>
constexpr void introspect(I&& i, aabb<D, ScalarT, TraitsT>& v)
{
    i(v.min, "min");
    i(v.max, "max");
}

template <int D, class ScalarT, class TraitsT>
struct object_traits<aabb<D, ScalarT, TraitsT>> : detail::finite_object_traits<D, ScalarT, D, TraitsT>
{
};
} // namespace tg
