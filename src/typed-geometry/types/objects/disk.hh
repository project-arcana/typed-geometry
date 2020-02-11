#pragma once

#include <typed-geometry/types/objects/sphere.hh>

namespace tg
{
template <class ScalarT, class TraitsT = default_object_tag>
using disk = sphere<2, ScalarT, 3, TraitsT>;
template <class ScalarT>
using disk_boundary = disk<ScalarT, boundary_tag>;

using disk3 = disk<f32>;
using fdisk3 = disk<f32>;
using ddisk3 = disk<f64>;
using idisk3 = disk<i32>;
using udisk3 = disk<u32>;
}
