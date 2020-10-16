#pragma once

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/capsule.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/hemisphere.hh>
#include <typed-geometry/types/objects/inf_cone.hh>
#include <typed-geometry/types/objects/inf_cylinder.hh>
#include <typed-geometry/types/objects/pyramid.hh>
#include <typed-geometry/types/objects/sphere.hh>

// tg::boundary_of(obj) converts an object to its boundary (identity for objects that are already boundaries)

namespace tg
{
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr aabb_boundary<D, ScalarT> boundary_of(aabb<D, ScalarT, TraitsT> const& v)
{
    return {v.min, v.max};
}
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr box_boundary<D, ScalarT> boundary_of(box<D, ScalarT, D, TraitsT> const& v)
{
    return {v.center, v.half_extents};
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr box_boundary<2, ScalarT, 3> boundary_of(box<2, ScalarT, 3, TraitsT> const& v)
{
    return {v.center, v.half_extents, v.normal};
}
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr capsule_boundary<D, ScalarT> boundary_of(capsule<D, ScalarT, TraitsT> const& v)
{
    return {v.axis, v.radius};
}
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr cylinder_boundary<D, ScalarT> boundary_of(cylinder<D, ScalarT, TraitsT> const& v)
{
    return {v.axis, v.radius};
}
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr hemisphere_boundary<D, ScalarT> boundary_of(hemisphere<D, ScalarT, TraitsT> const& v)
{
    return {v.center, v.radius, v.normal};
}
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr inf_cone_boundary<D, ScalarT> boundary_of(inf_cone<D, ScalarT, TraitsT> const& v)
{
    return {v.apex, v.opening_dir, v.opening_angle};
}
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr inf_cylinder_boundary<D, ScalarT> boundary_of(inf_cylinder<D, ScalarT, TraitsT> const& v)
{
    return {v.axis, v.radius};
}
template <class BaseT, class TraitsT>
[[nodiscard]] constexpr pyramid_boundary<BaseT> boundary_of(pyramid<BaseT, TraitsT> const& v)
{
    return {v.base, v.height};
}
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr sphere_boundary<D, ScalarT> boundary_of(sphere<D, ScalarT, D, TraitsT> const& v)
{
    return {v.center, v.radius};
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr sphere_boundary<1, ScalarT, 2> boundary_of(sphere<1, ScalarT, 2, TraitsT> const& v)
{
    return {v.center, v.radius, v.normal};
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr sphere_boundary<2, ScalarT, 3> boundary_of(sphere<2, ScalarT, 3, TraitsT> const& v)
{
    return {v.center, v.radius, v.normal};
}

// === no caps versions ===

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr cylinder_boundary_no_caps<D, ScalarT> boundary_no_caps_of(cylinder<D, ScalarT, TraitsT> const& v)
{
    return {v.axis, v.radius};
}
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr hemisphere_boundary_no_caps<D, ScalarT> boundary_no_caps_of(hemisphere<D, ScalarT, TraitsT> const& v)
{
    return {v.center, v.radius, v.normal};
}
template <class BaseT, class TraitsT>
[[nodiscard]] constexpr pyramid_boundary_no_caps<BaseT> boundary_no_caps_of(pyramid<BaseT, TraitsT> const& v)
{
    return {v.base, v.height};
}

template <int D, class ScalarT>
[[nodiscard]] constexpr cylinder_boundary_no_caps<D, ScalarT> boundary_of(cylinder_boundary_no_caps<D, ScalarT> v)
{
    return v;
}
template <int D, class ScalarT>
[[nodiscard]] constexpr hemisphere_boundary_no_caps<D, ScalarT> boundary_of(hemisphere_boundary_no_caps<D, ScalarT> v)
{
    return v;
}
template <class BaseT>
[[nodiscard]] constexpr pyramid_boundary_no_caps<BaseT> boundary_of(pyramid_boundary_no_caps<BaseT> v)
{
    return v;
}

// === only caps versions ===

template <class ScalarT>
[[nodiscard]] constexpr disk<3, ScalarT> caps_of(hemisphere<3, ScalarT> const& v)
{
    return {v.center, v.radius, v.normal};
}
template <class ScalarT>
[[nodiscard]] constexpr segment<2, ScalarT> caps_of(hemisphere<2, ScalarT> const& v)
{
    auto half = perpendicular(v.normal) * v.radius;
    return {v.center - half, v.center + half};
}
template <class ScalarT>
[[nodiscard]] constexpr disk<3, ScalarT> caps_of(hemisphere_boundary<3, ScalarT> const& v)
{
    return {v.center, v.radius, v.normal};
}
template <class ScalarT>
[[nodiscard]] constexpr segment<2, ScalarT> caps_of(hemisphere_boundary<2, ScalarT> const& v)
{
    auto half = perpendicular(v.normal) * v.radius;
    return {v.center - half, v.center + half};
}
template <class ScalarT>
[[nodiscard]] constexpr circle<3, ScalarT> caps_of(hemisphere_boundary_no_caps<3, ScalarT> const& v)
{
    return {v.center, v.radius, v.normal};
}
template <class ScalarT>
[[nodiscard]] constexpr sphere_boundary<1, ScalarT, 2> caps_of(hemisphere_boundary_no_caps<2, ScalarT> const& v)
{
    return {v.center, v.radius, v.normal};
}

template <class BaseT>
[[nodiscard]] constexpr BaseT caps_of(pyramid<BaseT> const& v)
{
    return v.base;
}
template <class BaseT>
[[nodiscard]] constexpr BaseT caps_of(pyramid<BaseT, boundary_tag> const& v)
{
    return v.base;
}
template <class BaseT>
[[nodiscard]] constexpr auto caps_of(pyramid<BaseT, boundary_no_caps_tag> const& v) -> decltype(boundary_of(v.base))
{
    return boundary_of(v.base);
}

// === infinite versions ===

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr auto inf_of(cone<D, ScalarT, TraitsT> const& v)
{
    const auto apex = apex_of(v);
    const auto openingDir = -v.base.normal;
    const auto openingAngle = ScalarT(2) * angle_between(normalize(v.base.center + any_normal(v.base.normal) * v.base.radius - apex), openingDir);
    if constexpr (std::is_same_v<TraitsT, default_object_tag>)
        return inf_cone<D, ScalarT, default_object_tag>(apex, openingDir, openingAngle);
    else
        return inf_cone<D, ScalarT, boundary_tag>(apex, openingDir, openingAngle);
}
}
