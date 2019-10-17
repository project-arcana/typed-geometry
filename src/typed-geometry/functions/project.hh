#pragma once

#include <typed-geometry/detail/operators/ops_vec.hh>
#include <typed-geometry/detail/special_values.hh>
#include <typed-geometry/tests/vec_tests.hh>
#include <typed-geometry/types/objects/capsule.hh>
#include <typed-geometry/types/objects/circle.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/disk.hh>
#include <typed-geometry/types/objects/inf_cone.hh>
#include <typed-geometry/types/objects/inf_tube.hh>
#include <typed-geometry/types/objects/line.hh>
#include <typed-geometry/types/objects/plane.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/tube.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

#include "contains.hh"
#include "coordinates.hh"
#include "normal.hh"

namespace tg
{
// ============================== Vec Projections ==============================

template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, ScalarT> project(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return b * dot(a, b) / dot(b, b);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, ScalarT> project(vec<D, ScalarT> const& a, dir<D, ScalarT> const& b)
{
    return b * dot(a, b);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, ScalarT> project(vec<D, ScalarT> const& v, hyperplane<D, ScalarT> const& pl)
{
    return v - pl.normal * dot(v, pl.normal);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, ScalarT> project(dir<D, ScalarT> const& v, hyperplane<D, ScalarT> const& pl)
{
    return v - pl.normal * dot(v, pl.normal);
}

// ============================== Pos Projections ==============================

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, line<D, ScalarT> const& l)
{
    return l.pos + project(p - l.pos, l.dir);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, ray<D, ScalarT> const& r)
{
    auto d = dot(p - r.origin, r.dir);
    return r.origin + max(d, ScalarT(0)) * r.dir;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, segment<D, ScalarT> const& s)
{
    auto t = coordinates(s, p);
    t = clamp(t, ScalarT(0), ScalarT(1));
    return s[t];
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, aabb<D, ScalarT> const& s)
{
    return clamp(p, s.min, s.max);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, hyperplane<D, ScalarT> const& pl)
{
    return p - pl.normal * (dot(p, pl.normal) - pl.dis);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, halfspace<D, ScalarT> const& pl)
{
    return p - pl.normal * tg::max(ScalarT(0), dot(p, pl.normal) - pl.dis);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, sphere<D, ScalarT> const& sp)
{
    auto dir_to_p = tg::normalize_safe(p - sp.center);
    if (is_zero_vector(dir_to_p))
        dir_to_p = vec<D, ScalarT>::unit_x;
    return sp.center + dir_to_p * sp.radius;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, hemisphere<D, ScalarT> const& h)
{
    auto dir_to_p = tg::normalize_safe(p - h.center);

    if (is_zero_vector(dir_to_p))
        return h.center + h.normal * h.radius;

    if (dot(dir_to_p, h.normal) < 0)
        return project(p, disk<D, ScalarT>(h.center, h.radius, h.normal));

    return h.center + dir_to_p * h.radius;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, ball<D, ScalarT> const& b)
{
    if (contains(b, p))
        return p;

    return project(p, sphere<D, ScalarT>(b.center, b.radius));
}

template <class ScalarT>
TG_NODISCARD constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, tube<3, ScalarT> const& t)
{
    auto lp = project(p, t.axis);
    auto dir = normalize_safe(p - lp);

    if (is_zero_vector(dir))
        dir = any_normal(t.axis.pos0 - t.axis.pos1);

    return lp + dir * t.radius;
}

template <class ScalarT>
TG_NODISCARD constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, disk<3, ScalarT> const& d)
{
    auto hp = project(p, hyperplane<3, ScalarT>(d.normal, d.center));

    if (distance_sqr(hp, d.center) <= d.radius * d.radius)
        return hp;

    auto dir = normalize_safe(hp - d.center);
    if (is_zero_vector(dir))
        dir = any_normal(d.normal);

    return d.center + dir * d.radius;
}

template <class ScalarT>
TG_NODISCARD constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, circle<3, ScalarT> const& c)
{
    auto hp = project(p, hyperplane<3, ScalarT>(c.normal, c.center));

    auto dir = normalize_safe(hp - c.center);
    if (is_zero_vector(dir))
        dir = any_normal(c.normal);

    return c.center + dir * c.radius;
}

template <class ScalarT>
TG_NODISCARD constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, cylinder<3, ScalarT> const& c)
{
    auto dir = direction(c);

    auto p0 = project(p, tube<3, ScalarT>(c.axis, c.radius));
    auto p1 = project(p, disk<3, ScalarT>(c.axis.pos0, c.radius, dir));
    auto p2 = project(p, disk<3, ScalarT>(c.axis.pos1, c.radius, dir));

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

template <class ScalarT>
TG_NODISCARD constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, capsule<3, ScalarT> const& c)
{
    auto t = coordinates(c.axis, p);

    if (t < 0)
        return project(p, sphere<3, ScalarT>(c.axis.pos0, c.radius));

    if (t > 1)
        return project(p, sphere<3, ScalarT>(c.axis.pos1, c.radius));

    return project(p, tube<3, ScalarT>(c.axis, c.radius));
}

template <class ScalarT>
TG_NODISCARD constexpr pos<3, ScalarT> project(pos<3, ScalarT> const& p, inf_cone<3, ScalarT> const& icone)
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
        if (dot(p_apex_dir, icone.opening_dir) < 0)
            return icone.apex;

        // any point on the cone in normal direction from p is the closest point
        auto h = tg::length(p_apex);
        auto l = tg::cos(icone.opening_angle / 2) * h;
        auto r = tan(icone.opening_angle / 2);
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
    if (dot(p - c, x_axis) < 0)
        x_axis = -x_axis;

    // construct the 2D surface normal of the cone in the plane
    auto r = tan(icone.opening_angle / 2);
    vec2_t r_ = {r, 0};
    vec2_t p_ = {dot(p - c, x_axis), dot(p - c, y_axis)};
    vec2_t peak_ = {0, 1};
    dir2_t r_vec = normalize(r_ - peak_);
    dir2_t n_ = tg::perpendicular(r_vec);
    if (n_.y < 0)
        n_ = -n_;

    // reconstruct 3D closest point
    if (dot(r_vec, p_ - peak_) > 0)
    {
        auto d = dot(p_ - peak_, n_);
        auto proj_p2 = p_ - d * n_;
        return c + proj_p2.x * x_axis + proj_p2.y * y_axis;
    }
    else
        return icone.apex;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr pos<D, ScalarT> project(pos<D, ScalarT> const& p, inf_tube<D, ScalarT> const& itube)
{
    auto vec = p - itube.axis.pos;
    auto h = dot(vec, itube.axis.dir);
    auto point_on_axis = itube.axis[h];
    return point_on_axis + tg::normalize_safe(p - point_on_axis) * itube.radius;
}
} // namespace tg
