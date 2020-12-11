#pragma once

#include <typed-geometry/detail/special_values.hh>
#include <typed-geometry/functions/tests/vec_tests.hh>
#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/capsule.hh>
#include <typed-geometry/types/objects/cone.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/halfspace.hh>
#include <typed-geometry/types/objects/hemisphere.hh>
#include <typed-geometry/types/objects/inf_cone.hh>
#include <typed-geometry/types/objects/inf_cylinder.hh>
#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/plane.hh>
#include <typed-geometry/types/objects/pyramid.hh>
#include <typed-geometry/types/objects/ray.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/functions/vector/project.hh>

#include "boundary.hh"
#include "contains.hh"
#include "coordinates.hh"
#include "normal.hh"

namespace tg
{
// ============== project to plane ==============

template <int D, class ScalarT>
[[nodiscard]] constexpr vec<D, ScalarT> project(vec<D, ScalarT> const& v, plane<D, ScalarT> const& pl)
{
    return v - pl.normal * dot(v, pl.normal);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr vec<D, ScalarT> project(dir<D, ScalarT> const& v, plane<D, ScalarT> const& pl)
{
    return v - pl.normal * dot(v, pl.normal);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, plane<D, ScalarT> const& pl)
{
    return p - pl.normal * (dot(p, pl.normal) - pl.dis);
}


// ============== project to halfspace ==============

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, halfspace<D, ScalarT> const& pl)
{
    return p - pl.normal * tg::max(ScalarT(0), dot(p, pl.normal) - pl.dis);
}


// ============== project to point ==============

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project([[maybe_unused]] pos<D, ScalarT> const& p, pos<D, ScalarT> const& q)
{
    return q;
}


// ============== project to line / ray / segment ==============

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, line<D, ScalarT> const& l)
{
    return l.pos + project(p - l.pos, l.dir);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, ray<D, ScalarT> const& r)
{
    auto d = dot(p - r.origin, r.dir);
    return r.origin + max(d, ScalarT(0)) * r.dir;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, segment<D, ScalarT> const& s)
{
    auto t = coordinates(s, p);
    t = clamp(t, ScalarT(0), ScalarT(1));
    return s[t];
}


// ============== project to aabb ==============

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, aabb<D, ScalarT> const& b)
{
    return clamp(p, b.min, b.max);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, aabb<D, ScalarT, boundary_tag> const& b)
{
    auto res = p;
    auto projectionNeeded = true;
    auto closestDist = max<ScalarT>();
    auto closestDim = 0;             // Will be overwritten
    auto closestDimVal = ScalarT(0); // Will be overwritten
    for (auto i = 0; i < D; ++i)
    {
        if (p[i] <= b.min[i])
        {
            res[i] = b.min[i];
            projectionNeeded = false;
        }
        else if (p[i] >= b.max[i])
        {
            res[i] = b.max[i];
            projectionNeeded = false;
        }
        else if (projectionNeeded)
        {
            const auto distMin = abs(p[i] - b.min[i]);
            const auto distMax = abs(p[i] - b.max[i]);
            if (distMin <= distMax)
            {
                if (distMin < closestDist)
                {
                    closestDist = distMin;
                    closestDim = i;
                    closestDimVal = b.min[i];
                }
            }
            else
            {
                if (distMax < closestDist)
                {
                    closestDist = distMax;
                    closestDim = i;
                    closestDimVal = b.max[i];
                }
            }
        }
    }

    if (projectionNeeded)
        res[closestDim] = closestDimVal;

    return res;
}


// ============== project to box ==============

template <int ObjectD, class ScalarT, int DomainD>
[[nodiscard]] constexpr pos<DomainD, ScalarT> project(pos<DomainD, ScalarT> const& p, box<ObjectD, ScalarT, DomainD> const& b)
{
    auto localAabb = aabb<ObjectD, ScalarT>::minus_one_to_one;
    auto pLocal = pos(coordinates(b, p));
    return b.center + b.half_extents * vec(project(pLocal, localAabb));
}
template <int ObjectD, class ScalarT, int DomainD>
[[nodiscard]] constexpr pos<DomainD, ScalarT> project(pos<DomainD, ScalarT> const& p, box_boundary<ObjectD, ScalarT, DomainD> const& b)
{
    auto boxSize = pos<ObjectD, ScalarT>();
    for (auto i = 0; i < ObjectD; ++i)
        boxSize[i] = length(b.half_extents[i]);
    auto localAabb = aabb_boundary<ObjectD, ScalarT>(-boxSize, boxSize);

    auto pLocal = pos(coordinates(b, p));
    auto projLocal = project(pLocal * comp(boxSize), localAabb) / comp(boxSize); // Up-scaling and down-scaling handles stretched boxes
    return b.center + b.half_extents * vec(projLocal);
}


// ============== project to triangle ==============

template <class ScalarT>
[[nodiscard]] constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, triangle<3, ScalarT> const& t)
{
    auto n = normal_of(t);
    auto pPlane = project(p, plane<3, ScalarT>(n, t.pos0));
    auto edges = edges_of(t);

    // Check if projection is already in the triangle. Additionally find closest projection to one of the edges.
    pos<3, ScalarT> closestProj;
    auto minDist = tg::max<ScalarT>();
    auto leftOfAllEdges = true;
    for (const auto& edge : edges)
    {
        auto pEdge = project(pPlane, edge);
        if (leftOfAllEdges)
        {
            auto edgeNormal = normalize(cross(edge.pos1 - edge.pos0, n));
            leftOfAllEdges = dot(edgeNormal, pPlane - pEdge) <= ScalarT(0);
        }

        auto dist = distance_sqr(p, pEdge);
        if (dist < minDist)
        {
            minDist = dist;
            closestProj = pEdge;
        }
    }
    return leftOfAllEdges ? pPlane : closestProj;
}

template <class ScalarT>
[[nodiscard]] constexpr pos<2, ScalarT> project(pos<2, ScalarT> const& p, triangle<2, ScalarT> const& t)
{
    if (contains(t, p))
        return p;

    auto p0 = project(p, segment<2, ScalarT>(t.pos0, t.pos1));
    auto p1 = project(p, segment<2, ScalarT>(t.pos0, t.pos2));
    auto p2 = project(p, segment<2, ScalarT>(t.pos1, t.pos2));

    auto d0 = distance_sqr(p0, p);
    auto d1 = distance_sqr(p1, p);
    auto d2 = distance_sqr(p2, p);

    if (d0 <= d1 && d0 <= d2)
        return p0;
    else if (d1 <= d2)
        return p1;
    else
        return p2;
}


// ============== project to sphere ==============

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, sphere<D, ScalarT> const& s)
{
    if (contains(s, p))
        return p;

    return project(p, boundary_of(s));
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, sphere_boundary<D, ScalarT> const& sp)
{
    auto dir_to_p = tg::normalize_safe(p - sp.center);
    if (is_zero_vector(dir_to_p))
        dir_to_p = vec<D, ScalarT>::unit_x;
    return sp.center + dir_to_p * sp.radius;
}

template <class ScalarT>
[[nodiscard]] constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, sphere<2, ScalarT, 3> const& d)
{
    auto hp = project(p, plane<3, ScalarT>(d.normal, d.center));

    if (distance_sqr(hp, d.center) <= d.radius * d.radius)
        return hp;

    auto dir = normalize_safe(hp - d.center);
    if (is_zero_vector(dir))
        dir = any_normal(d.normal);

    return d.center + dir * d.radius;
}

template <class ScalarT>
[[nodiscard]] constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, sphere_boundary<2, ScalarT, 3> const& c)
{
    auto hp = project(p, plane<3, ScalarT>(c.normal, c.center));

    auto dir = normalize_safe(hp - c.center);
    if (is_zero_vector(dir))
        dir = any_normal(c.normal);

    return c.center + dir * c.radius;
}

template <class ScalarT>
[[nodiscard]] constexpr pos<2, ScalarT> project(pos<2, ScalarT> const& p, sphere<1, ScalarT, 2> const& s)
{
    auto v = perpendicular(s.normal) * s.radius;
    auto seg = segment<2, ScalarT>(s.center - v, s.center + v); // sphere1in2 is the same as segment2
    return project(p, seg);
}

template <class ScalarT>
[[nodiscard]] constexpr pos<2, ScalarT> project(pos<2, ScalarT> const& p, sphere_boundary<1, ScalarT, 2> const& s)
{
    auto v = perpendicular(s.normal) * s.radius;
    return dot(p - s.center, v) >= ScalarT(0) ? s.center + v : s.center - v;
}


// ============== project to hemisphere ==============

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, hemisphere<D, ScalarT> const& h)
{
    auto toP = p - h.center;
    if (dot(toP, h.normal) >= ScalarT(0)) // On the round side of the hemisphere or inside
    {
        if (length_sqr(toP) <= h.radius * h.radius)
            return p;
        else
            return h.center + normalize(toP) * h.radius;
    }
    // On the flat side of the hemisphere
    return project(p, caps_of(h));
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, hemisphere_boundary<D, ScalarT> const& h)
{
    auto closestOnFlat = project(p, caps_of(h));

    auto dirToP = tg::normalize_safe(p - h.center);
    if (dot(dirToP, h.normal) > ScalarT(0))
    {
        auto closestOnRound = h.center + dirToP * h.radius;
        return length_sqr(p - closestOnRound) >= length_sqr(p - closestOnFlat) ? closestOnFlat : closestOnRound;
    }
    return closestOnFlat;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, hemisphere_boundary_no_caps<D, ScalarT> const& h)
{
    auto dirToP = tg::normalize_safe(p - h.center);
    if (dot(dirToP, h.normal) > ScalarT(0))
        return h.center + dirToP * h.radius;

    return project(p, caps_of(h));
}


// ============== project to cylinder ==============

template <class ScalarT>
[[nodiscard]] constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, cylinder<3, ScalarT> const& c) // same as project(tube) for the internal case
{
    auto lp = project(p, line<3, ScalarT>(c.axis.pos0, normalize(c.axis.pos1 - c.axis.pos0)));
    auto sp = project(lp, c.axis);
    auto dir = p - lp;
    auto l = length(dir);
    if (l > c.radius)
        dir *= c.radius / l;

    return sp + dir;
}

template <class ScalarT>
[[nodiscard]] constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, cylinder_boundary_no_caps<3, ScalarT> const& t)
{
    auto lp = project(p, line<3, ScalarT>(t.axis.pos0, normalize(t.axis.pos1 - t.axis.pos0)));
    auto sp = project(lp, t.axis);
    auto dir = normalize_safe(p - lp);
    if (is_zero_vector(dir))
        dir = any_normal(t.axis.pos1 - t.axis.pos0);

    return sp + dir * t.radius;
}

template <class ScalarT>
[[nodiscard]] constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, cylinder_boundary<3, ScalarT> const& c) // boundary, including caps
{
    auto dir = direction(c);

    auto p0 = project(p, boundary_no_caps_of(c));
    auto p1 = project(p, sphere<2, ScalarT, 3>(c.axis.pos0, c.radius, dir));
    auto p2 = project(p, sphere<2, ScalarT, 3>(c.axis.pos1, c.radius, dir));

    auto d0 = distance_sqr(p0, p);
    auto d1 = distance_sqr(p1, p);
    auto d2 = distance_sqr(p2, p);

    if (d0 <= d1 && d0 <= d2)
        return p0;
    else if (d1 <= d2)
        return p1;
    else
        return p2;
}


// ============== project to inf_cylinder ==============

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, inf_cylinder<D, ScalarT> const& c)
{
    if (contains(c, p))
        return p;

    return project(p, boundary_of(c));
}

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, inf_cylinder_boundary<D, ScalarT> const& c)
{
    auto vec = p - c.axis.pos;
    auto h = dot(vec, c.axis.dir);
    auto point_on_axis = c.axis[h];
    return point_on_axis + tg::normalize_safe(p - point_on_axis) * c.radius;
}


// ============== project to capsule ==============

template <class ScalarT>
[[nodiscard]] constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, capsule<3, ScalarT> const& c) // including caps
{
    auto t = coordinates(c.axis, p);

    if (t < ScalarT(0))
        return project(p, sphere<3, ScalarT>(c.axis.pos0, c.radius));

    if (t > ScalarT(1))
        return project(p, sphere<3, ScalarT>(c.axis.pos1, c.radius));

    return project(p, cylinder<3, ScalarT>(c.axis, c.radius));
}

template <class ScalarT>
[[nodiscard]] constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, capsule_boundary<3, ScalarT> const& c) // boundary, including caps
{
    auto t = coordinates(c.axis, p);

    if (t < ScalarT(0))
        return project(p, sphere_boundary<3, ScalarT>(c.axis.pos0, c.radius));

    if (t > ScalarT(1))
        return project(p, sphere_boundary<3, ScalarT>(c.axis.pos1, c.radius));

    return project(p, cylinder_boundary_no_caps<3, ScalarT>(c.axis, c.radius));
}


// ============== project to pyramids ==============

// all cone variants
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, cone<3, ScalarT, TraitsT> const& c)
{
    auto closestOnBase = project(p, caps_of(c));
    if (dot(p - closestOnBase, apex_of(c) - closestOnBase) <= ScalarT(0)) // Base is closer than any point on the cone can be
        return closestOnBase;

    // Return closer projection
    auto closestOnSide = project(p, inf_of<3, ScalarT>(c));
    return distance_sqr(p, closestOnSide) >= distance_sqr(p, closestOnBase) ? closestOnBase : closestOnSide;
}

// other pyramids
template <class BaseT, typename = std::enable_if_t<!std::is_same_v<BaseT, sphere<2, typename BaseT::scalar_t, 3>>>>
[[nodiscard]] constexpr pos<3, typename BaseT::scalar_t> project(pos<3, typename BaseT::scalar_t> const& p, pyramid<BaseT> const& py)
{
    if (contains(py, p))
        return p;

    return project(p, boundary_of(py));
}
template <class BaseT, typename = std::enable_if_t<!std::is_same_v<BaseT, sphere<2, typename BaseT::scalar_t, 3>>>>
[[nodiscard]] constexpr pos<3, typename BaseT::scalar_t> project(pos<3, typename BaseT::scalar_t> const& p, pyramid_boundary<BaseT> const& py)
{
    auto closestOnBase = project(p, caps_of(py));
    if (dot(p - closestOnBase, apex_of(py) - closestOnBase) <= typename BaseT::scalar_t(0)) // Base is closer than any point on the pyramid can be
        return closestOnBase;

    // Return closer projection
    auto closestOnSide = project(p, boundary_no_caps_of(py));
    return distance_sqr(p, closestOnSide) >= distance_sqr(p, closestOnBase) ? closestOnBase : closestOnSide;
}
template <class BaseT, typename = std::enable_if_t<!std::is_same_v<BaseT, sphere<2, typename BaseT::scalar_t, 3>>>>
[[nodiscard]] constexpr pos<3, typename BaseT::scalar_t> project(pos<3, typename BaseT::scalar_t> const& p, pyramid_boundary_no_caps<BaseT> const& py)
{
    auto bestDist = max<float>();
    auto bestProj = p;

    for (const auto& face : faces_of(py))
    {
        const auto proj = project(p, face);
        const auto dist = distance_sqr(p, proj);
        if (dist < bestDist)
        {
            bestDist = dist;
            bestProj = proj;
        }
    }

    return bestProj;
}


// ============== project to inf_cone ==============

template <int D, class ScalarT>
[[nodiscard]] constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, inf_cone<D, ScalarT> const& icone)
{
    if (contains(icone, p))
        return p;

    return project(p, boundary_of(icone));
}

template <class ScalarT>
[[nodiscard]] constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, inf_cone_boundary<3, ScalarT> const& icone)
{
    using dir_t = dir<3, ScalarT>;
    using vec2_t = vec<2, ScalarT>;
    using dir2_t = dir<2, ScalarT>;

    // check if p lies on the center axis
    auto p_apex = p - icone.apex;
    auto p_apex_dir = normalize_safe(p_apex);
    if (p_apex_dir == vec<3, ScalarT>::zero)
        return icone.apex;

    if (tg::are_collinear(p_apex_dir, static_cast<vec<3, ScalarT>>(icone.opening_dir)))
    {
        // p is "above" the apex
        if (dot(p_apex_dir, icone.opening_dir) < ScalarT(0))
            return icone.apex;

        // any point on the cone in normal direction from p is the closest point
        auto h = tg::length(p_apex);
        auto l = tg::cos(icone.opening_angle / ScalarT(2)) * h;
        auto r = tan(icone.opening_angle / ScalarT(2));
        dir_t ortho_dir = tg::any_normal(icone.opening_dir);
        auto pt_on_cone = icone.apex + icone.opening_dir + ortho_dir * r;
        dir_t on_surface_dir = normalize(pt_on_cone - icone.apex);
        return icone.apex + l * on_surface_dir;
    }

    // construct a reference point c (in 3d) inside the cone on the center axis
    auto c = icone.apex + icone.opening_dir;

    // construct a 2D coordinate system in the plane spanned by the c (origin), apex, and p
    dir_t y_axis = -icone.opening_dir;
    dir_t plane_normal = normalize(cross(p - c, vec<3, ScalarT>(y_axis)));
    dir_t x_axis = normalize(cross(y_axis, plane_normal));
    if (dot(p - c, x_axis) < ScalarT(0))
        x_axis = -x_axis;

    // construct the 2D surface normal of the cone in the plane
    auto r = tan(icone.opening_angle / ScalarT(2));
    vec2_t r_ = {r, ScalarT(0)};
    vec2_t p_ = {dot(p - c, x_axis), dot(p - c, y_axis)};
    vec2_t peak_ = {ScalarT(0), ScalarT(1)};
    dir2_t r_vec = normalize(r_ - peak_);
    dir2_t n_ = tg::perpendicular(r_vec);
    if (n_.y < ScalarT(0))
        n_ = -n_;

    // reconstruct 3D closest point
    if (dot(r_vec, p_ - peak_) > ScalarT(0))
    {
        auto d = dot(p_ - peak_, n_);
        auto proj_p2 = p_ - d * n_;
        return c + proj_p2.x * x_axis + proj_p2.y * y_axis;
    }
    else
        return icone.apex;
}
template <class ScalarT>
[[nodiscard]] constexpr pos<2, ScalarT> project(pos<2, ScalarT> const& p, inf_cone_boundary<2, ScalarT> const& icone)
{
    auto ray1 = ray<2, ScalarT>(icone.apex, rotate(icone.opening_dir, icone.opening_angle / ScalarT(2)));
    auto ray2 = ray<2, ScalarT>(icone.apex, rotate(icone.opening_dir, -icone.opening_angle / ScalarT(2)));
    auto proj1 = project(p, ray1);
    auto proj2 = project(p, ray2);
    return distance_sqr(p, proj1) <= distance_sqr(p, proj2) ? proj1 : proj2;
}
} // namespace tg
