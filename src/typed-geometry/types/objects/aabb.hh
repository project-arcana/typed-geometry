#pragma once

#include <typed-geometry/common/assert.hh>

#include <typed-geometry/types/scalars/default.hh>
#include "../pos.hh"
#include "../vec.hh"

namespace tg
{
template <int D, class ScalarT>
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


// ======== IMPLEMENTATION ========

template <int D, class ScalarT>
struct aabb
{
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

    template <class OtherT>
    constexpr aabb(pos<D, OtherT> min, pos<D, OtherT> max) : aabb(pos_t(min), pos_t(max))
    {
    }

    TG_NODISCARD constexpr pos_t operator[](tg::comp<D, ScalarT> const& c) const;

    TG_NODISCARD bool operator==(aabb const& rhs) const { return min == rhs.min && max == rhs.max; }
    TG_NODISCARD bool operator!=(aabb const& rhs) const { return !operator==(rhs); }
};
} // namespace tg
