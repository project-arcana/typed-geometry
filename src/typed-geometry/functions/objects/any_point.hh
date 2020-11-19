#pragma once

#include <typed-geometry/functions/objects/centroid.hh>
#include <typed-geometry/functions/objects/normal.hh>

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/capsule.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/ellipse.hh>
#include <typed-geometry/types/objects/halfspace.hh>
#include <typed-geometry/types/objects/hemisphere.hh>
#include <typed-geometry/types/objects/inf_cone.hh>
#include <typed-geometry/types/objects/inf_cylinder.hh>
#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/plane.hh>
#include <typed-geometry/types/objects/pyramid.hh>
#include <typed-geometry/types/objects/quad.hh>
#include <typed-geometry/types/objects/ray.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

#include <typed-geometry/detail/operators/ops_comp.hh>
#include <typed-geometry/detail/operators/ops_pos.hh>
#include <typed-geometry/detail/operators/ops_vec.hh>

// returns a point on the object

namespace tg
{
template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr pos<D, ScalarT> any_point(aabb<D, ScalarT, TraitsT> const& b)
{
    return b.max;
}

template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr pos<DomainD, ScalarT> any_point(box<ObjectD, ScalarT, DomainD, TraitsT> const& b)
{
    if constexpr (std::is_same_v<TraitsT, default_object_tag>)
        return b.center;
    else
        return b[comp<ObjectD, ScalarT>(1)];
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr pos<3, ScalarT> any_point(capsule<3, ScalarT, TraitsT> const& c)
{
    if constexpr (std::is_same_v<TraitsT, default_object_tag>)
        return c.axis.pos1;
    else
        return c.axis.pos1 + c.radius * any_normal(c.axis.pos1 - c.axis.pos0);
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr pos<3, ScalarT> any_point(cylinder<3, ScalarT, TraitsT> const& c)
{
    if constexpr (std::is_same_v<TraitsT, default_object_tag>)
        return c.axis.pos1;
    else
        return c.axis.pos1 + c.radius * any_normal(c.axis.pos1 - c.axis.pos0);
}

template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr pos<DomainD, ScalarT> any_point(ellipse<ObjectD, ScalarT, DomainD, TraitsT> const& e)
{
    if constexpr (std::is_same_v<TraitsT, default_object_tag>)
        return e.center;
    else
        return e.center + e.semi_axes[0];
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> any_point(halfspace<D, ScalarT> const& h)
{
    return pos(- h.dis * h.normal);
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr pos<D, ScalarT> any_point(hemisphere<D, ScalarT, TraitsT> const& h)
{
    if constexpr (std::is_same_v<TraitsT, boundary_no_caps_tag>)
        return h.center + h.radius * h.normal;
    else
        return h.center;
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr pos<D, ScalarT> any_point(inf_cone<D, ScalarT, TraitsT> const& c)
{
    return c.apex;
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr pos<D, ScalarT> any_point(inf_cylinder<D, ScalarT, TraitsT> const& c)
{
    if constexpr (std::is_same_v<TraitsT, default_object_tag>)
        return c.axis.pos;
    else
    {
        if constexpr (D == 3)
            return c.axis.pos + c.radius * any_normal(c.axis.dir);
        else
            return c.axis.pos + c.radius * perpendicular(c.axis.dir);
    }
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> any_point(line<D, ScalarT> const& l)
{
    return l.pos;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> any_point(plane<D, ScalarT> const& p)
{
    return pos(p.dis * p.normal);
}

template <class BaseT, class TraitsT>
[[nodiscard]] constexpr pos<3, typename BaseT::scalar_t> any_point(pyramid<BaseT, TraitsT> const& py)
{
    if constexpr (std::is_same_v<TraitsT, default_object_tag>)
        return centroid_of(py.base);
    else
        return apex_of(py);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> any_point(quad<D, ScalarT> const& q)
{
    return q.pos00;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> any_point(ray<D, ScalarT> const& r)
{
    return r.origin;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> any_point(segment<D, ScalarT> const& s)
{
    return s.pos0;
}

template <int ObjectD, class ScalarT, int DomainD>
[[nodiscard]] constexpr pos<DomainD, ScalarT> any_point(sphere<ObjectD, ScalarT, DomainD> const& s)
{
    return s.center;
}
template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> any_point(sphere_boundary<D, ScalarT> const& s)
{
    return s.center + s.radius * dir<D, ScalarT>::pos_x;
}
template <class ScalarT>
[[nodiscard]] constexpr pos<3, ScalarT> any_point(sphere_boundary<2, ScalarT, 3> const& s)
{
    return s.center + s.radius * any_normal(s.normal);
}
template <class ScalarT>
[[nodiscard]] constexpr pos<2, ScalarT> any_point(sphere_boundary<1, ScalarT, 2> const& s)
{
    return s.center + s.radius * perpendicular(s.normal);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> any_point(triangle<D, ScalarT> const& t)
{
    return t.pos0;
}
} // namespace tg
