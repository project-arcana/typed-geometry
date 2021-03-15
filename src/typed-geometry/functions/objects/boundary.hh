#pragma once

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/capsule.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/ellipse.hh>
#include <typed-geometry/types/objects/halfspace.hh>
#include <typed-geometry/types/objects/hemisphere.hh>
#include <typed-geometry/types/objects/inf_cone.hh>
#include <typed-geometry/types/objects/inf_cylinder.hh>
#include <typed-geometry/types/objects/plane.hh>
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
template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr box_boundary<ObjectD, ScalarT, DomainD> boundary_of(box<ObjectD, ScalarT, DomainD, TraitsT> const& v)
{
    return {v.center, v.half_extents};
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
template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr ellipse_boundary<ObjectD, ScalarT, DomainD> boundary_of(ellipse<ObjectD, ScalarT, DomainD, TraitsT> const& v)
{
    return {v.center, v.semi_axes};
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
template <int D, class ScalarT>
[[nodiscard]] constexpr plane<D, ScalarT> boundary_of(halfspace<D, ScalarT> const& v)
{
    return {v.normal, v.dis};
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

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr auto caps_of(hemisphere<3, ScalarT, TraitsT> const& v)
{
    if constexpr (std::is_same_v<TraitsT, boundary_no_caps_tag>)
        return circle<3, ScalarT>(v.center, v.radius, v.normal);
    else
        return disk<3, ScalarT>(v.center, v.radius, v.normal);
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr auto caps_of(hemisphere<2, ScalarT, TraitsT> const& v)
{
    if constexpr (std::is_same_v<TraitsT, boundary_no_caps_tag>)
        return sphere_boundary<1, ScalarT, 2>(v.center, v.radius, v.normal);
    else
    {
        auto half = perpendicular(v.normal) * v.radius;
        return segment<2, ScalarT>(v.center - half, v.center + half);
    }
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr pos<1, ScalarT> caps_of(hemisphere<1, ScalarT, TraitsT> const& v)
{
    static_assert(!std::is_same_v<TraitsT, boundary_no_caps_tag> && "1D hemisphere_boundary_no_caps does not have any caps");
    return v.center;
}

template <class BaseT, class TraitsT>
[[nodiscard]] constexpr auto caps_of(pyramid<BaseT, TraitsT> const& v)
{
    if constexpr (std::is_same_v<TraitsT, boundary_no_caps_tag>)
        return boundary_of(v.base);
    else
        return v.base;
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr auto caps_of(cylinder<D, ScalarT, TraitsT> const& v)
{
    const auto normal = normalize(v.axis.pos1 - v.axis.pos0);
    if constexpr (std::is_same_v<TraitsT, boundary_no_caps_tag>)
        return array<sphere<D - 1, ScalarT, D, boundary_tag>, 2>{{{v.axis.pos0, v.radius, -normal}, {v.axis.pos1, v.radius, normal}}};
    else
        return array<sphere<D - 1, ScalarT, D>, 2>{{{v.axis.pos0, v.radius, -normal}, {v.axis.pos1, v.radius, normal}}};
}

// === solid version ===

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr aabb<D, ScalarT> solid_of(aabb<D, ScalarT, TraitsT> const& v)
{
    return {v.min, v.max};
}
template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr box<ObjectD, ScalarT, DomainD> solid_of(box<ObjectD, ScalarT, DomainD, TraitsT> const& v)
{
    return {v.center, v.half_extents};
}
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr capsule<D, ScalarT> solid_of(capsule<D, ScalarT, TraitsT> const& v)
{
    return {v.axis, v.radius};
}
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr cylinder<D, ScalarT> solid_of(cylinder<D, ScalarT, TraitsT> const& v)
{
    return {v.axis, v.radius};
}
template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr ellipse<ObjectD, ScalarT, DomainD> solid_of(ellipse<ObjectD, ScalarT, DomainD, TraitsT> const& v)
{
    return {v.center, v.semi_axes};
}
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr hemisphere<D, ScalarT> solid_of(hemisphere<D, ScalarT, TraitsT> const& v)
{
    return {v.center, v.radius, v.normal};
}
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr inf_cone<D, ScalarT> solid_of(inf_cone<D, ScalarT, TraitsT> const& v)
{
    return {v.apex, v.opening_dir, v.opening_angle};
}
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr inf_cylinder<D, ScalarT> solid_of(inf_cylinder<D, ScalarT, TraitsT> const& v)
{
    return {v.axis, v.radius};
}
template <class BaseT, class TraitsT>
[[nodiscard]] constexpr pyramid<BaseT> solid_of(pyramid<BaseT, TraitsT> const& v)
{
    return {v.base, v.height};
}
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr sphere<D, ScalarT> solid_of(sphere<D, ScalarT, D, TraitsT> const& v)
{
    return {v.center, v.radius};
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr sphere<1, ScalarT, 2> solid_of(sphere<1, ScalarT, 2, TraitsT> const& v)
{
    return {v.center, v.radius, v.normal};
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr sphere<2, ScalarT, 3> solid_of(sphere<2, ScalarT, 3, TraitsT> const& v)
{
    return {v.center, v.radius, v.normal};
}

// === infinite versions ===

template <int D, class ScalarT>
[[nodiscard]] constexpr line<D, ScalarT> inf_of(segment<D, ScalarT> const& v)
{
    return {v.pos0, normalize(v.pos1 - v.pos0)};
}
template <int D, class ScalarT>
[[nodiscard]] constexpr line<D, ScalarT> inf_of(ray<D, ScalarT> const& v)
{
    return {v.origin, v.dir};
}

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

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr auto inf_of(cylinder<D, ScalarT, TraitsT> const& v)
{
    const auto axis = inf_of(v.axis);
    if constexpr (std::is_same_v<TraitsT, default_object_tag>)
        return inf_cylinder<D, ScalarT, default_object_tag>(axis, v.radius);
    else
        return inf_cylinder<D, ScalarT, boundary_tag>(axis, v.radius);
}
}
