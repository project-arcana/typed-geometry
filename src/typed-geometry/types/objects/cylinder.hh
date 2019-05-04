#pragma once

#include "../angle.hh"
#include "../dir.hh"
#include "../pos.hh"

#include "disk.hh"
#include "segment.hh"

namespace tg
{
/**
 * circular right cylinder with disks as bases
 * pos is center of cylinder
 * height is the perpendicular distance between the bases
 */
template <int D, class ScalarT>
struct cylinder;

// Common cylinder types

using cylinder2 = cylinder<2, f32>;
using cylinder3 = cylinder<3, f32>;

using fcylinder2 = cylinder<2, f32>;
using fcylinder3 = cylinder<3, f32>;

using dcylinder2 = cylinder<2, f64>;
using dcylinder3 = cylinder<3, f64>;

using icylinder2 = cylinder<2, i32>;
using icylinder3 = cylinder<3, i32>;

using ucylinder2 = cylinder<2, u32>;
using ucylinder3 = cylinder<3, u32>;


// ======== IMPLEMENTATION ========

template <class ScalarT>
struct cylinder<2, ScalarT>
{
    using pos_t = pos<2, ScalarT>;
    using segment_t = segment<2, ScalarT>;

    pos_t center;
    ScalarT radius; // TODO store in disks?
    ScalarT height;

    segment_t base;
    segment_t top; // TODO only store base?

    /*
     * // TODO allow for oblique cylinder?
    // lean is 0 for right cylinder
    angle<ScalarT> tilt;
    ScalarT offset; // disk centers' offset in x direction
angle<ScalarT> a, ScalarT o...
    tilt(a), offset(tan(a) * h)..
             base(c + pos_t(-offset / 2, -height / 2), r), top(c + pos_t(+offset / 2, +height / 2)
*/

    constexpr cylinder() = default;
    constexpr cylinder(pos_t c, ScalarT r, ScalarT h) : center(c), radius(r), height(h)
    {
        base(segment_t(c + pos_t(-r / 2, -h / 2), c + pos_t(r / 2, -h / 2)));
        top(segment_t(c + pos_t(-r / 2, +h / 2), c + pos_t(r / 2, +h / 2)));
    }
};

template <class ScalarT>
struct cylinder<3, ScalarT>
{
    using pos_t = pos<3, ScalarT>;
    using dir_t = dir<3, ScalarT>;
    using disk_t = disk<3, ScalarT>;

    pos_t center;
    ScalarT radius;
    ScalarT height;

    dir_t normal; // oriented in 3d space

    disk_t base;
    disk_t top; // TODO only store base?

    constexpr cylinder() = default;
    constexpr cylinder(pos_t c, ScalarT r, ScalarT h, dir_t n = dir_t::pos_y)
      : center(c), radius(r), height(h), normal(n), base(disk_t(c - h / 2 * n, r, n)), top(disk_t(c + h / 2 * n, r, n))
    {
    }
};
} // namespace tg
