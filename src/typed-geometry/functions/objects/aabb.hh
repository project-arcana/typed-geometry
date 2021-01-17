#pragma once

#include <typed-geometry/feature/assert.hh>

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/capsule.hh>
#include <typed-geometry/types/objects/cylinder.hh>
#include <typed-geometry/types/objects/ellipse.hh>
#include <typed-geometry/types/objects/hemisphere.hh>
#include <typed-geometry/types/objects/pyramid.hh>
#include <typed-geometry/types/objects/quad.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr aabb<D, ScalarT> aabb_of(pos<D, ScalarT> const& v)
{
    return {v, v};
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr aabb<D, ScalarT> aabb_of(aabb<D, ScalarT, TraitsT> const& b)
{
    return aabb<D, ScalarT>(b);
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr aabb<D, ScalarT> aabb_of(sphere<D, ScalarT, D, TraitsT> const& s)
{
    return {s.center - s.radius, s.center + s.radius};
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr aabb<2, ScalarT> aabb_of(sphere<1, ScalarT, 2, TraitsT> const& s)
{
    const auto v = perpendicular(s.normal) * s.radius;
    return aabb_of(s.center - v, s.center + v);
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr aabb<3, ScalarT> aabb_of(sphere<2, ScalarT, 3, TraitsT> const& s)
{
    // See http://www.iquilezles.org/www/articles/diskbbox/diskbbox.htm
    const auto e = abs(s.radius) * sqrt(ScalarT(1) - comp(s.normal) * comp(s.normal));
    return {s.center - e, s.center + e};
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr aabb<1, ScalarT> aabb_of(ellipse<1, ScalarT, 1, TraitsT> const& e)
{
    const auto ex = abs(e.semi_axes[0]);
    return {e.center - ex, e.center + ex};
}
template <class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr aabb<DomainD, ScalarT> aabb_of(ellipse<2, ScalarT, DomainD, TraitsT> const& e)
{
    // See https://www.iquilezles.org/www/articles/ellipses/ellipses.htm
    const auto u = comp(e.semi_axes[0]);
    const auto v = comp(e.semi_axes[1]);
    const auto ex = sqrt(u * u + v * v);
    return {e.center - ex, e.center + ex};
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr aabb<3, ScalarT> aabb_of(ellipse<3, ScalarT, 3, TraitsT> const& e)
{
    // See https://members.loria.fr/SHornus/ellipsoid-bbox.html
    const auto ex = vec<3, ScalarT>(length(e.semi_axes.row(0)), length(e.semi_axes.row(1)), length(e.semi_axes.row(2)));
    return {e.center - ex, e.center + ex};
}

template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr aabb<1, ScalarT> aabb_of(hemisphere<1, ScalarT, TraitsT> const& h)
{
    return aabb_of(h.center, h.center + h.normal * h.radius);
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr aabb<2, ScalarT> aabb_of(hemisphere<2, ScalarT, TraitsT> const& h)
{
    const auto baseVec = h.radius * perpendicular(h.normal);
    const auto sphereCorner = h.center + h.radius * sign(vec(h.normal));
    return aabb_of(h.center - baseVec, h.center + baseVec, sphereCorner);
}
template <class ScalarT, class TraitsT>
[[nodiscard]] constexpr aabb<3, ScalarT> aabb_of(hemisphere<3, ScalarT, TraitsT> const& h)
{
    const auto disk = caps_of(h);
    const auto sphereCorner = h.center + h.radius * sign(vec(h.normal));
    return aabb_of(disk, sphereCorner);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr aabb<D, ScalarT> aabb_of(segment<D, ScalarT> const& s)
{
    return aabb_of(s.pos0, s.pos1);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr aabb<D, ScalarT> aabb_of(triangle<D, ScalarT> const& t)
{
    return aabb_of(t.pos0, t.pos1, t.pos2);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr aabb<D, ScalarT> aabb_of(quad<D, ScalarT> const& q)
{
    return aabb_of(q.pos00, q.pos10, q.pos11, q.pos01);
}

template <int ObjectD, class ScalarT, int DomainD, class TraitsT>
[[nodiscard]] constexpr aabb<DomainD, ScalarT> aabb_of(box<ObjectD, ScalarT, DomainD, TraitsT> const& b)
{
    vec<DomainD, ScalarT> diag;
    for (auto i = 0; i < ObjectD; ++i)
        diag += abs(b.half_extents[i]);

    return {b.center - diag, b.center + diag};
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr aabb<D, ScalarT> aabb_of(capsule<D, ScalarT, TraitsT> const& c)
{
    return aabb_of(sphere<D, ScalarT>(c.axis.pos0, c.radius), sphere<D, ScalarT>(c.axis.pos1, c.radius));
}

template <int D, class ScalarT, class TraitsT>
[[nodiscard]] constexpr aabb<D, ScalarT> aabb_of(cylinder<D, ScalarT, TraitsT> const& c)
{
    const auto n = normalize(c.axis.pos1 - c.axis.pos0);
    return aabb_of(sphere<2, ScalarT, 3>(c.axis.pos0, c.radius, n), sphere<2, ScalarT, 3>(c.axis.pos1, c.radius, n));
}

template <class BaseT, class TraitsT>
[[nodiscard]] constexpr aabb<3, typename BaseT::scalar_t> aabb_of(pyramid<BaseT, TraitsT> const& p)
{
    return aabb_of(p.base, apex_of(p));
}

template <class PrimA, class PrimB, class... PrimsT>
[[nodiscard]] constexpr auto aabb_of(PrimA const& pa, PrimB const& pb, PrimsT const&... prims) -> decltype(aabb_of(pa))
{
    auto ba = aabb_of(pa);
    auto bb = aabb_of(pb);
    static_assert(is_same<decltype(ba), decltype(bb)>, "all arguments must have the same aabb aabb type");
    auto b = decltype(ba)(min(ba.min, bb.min), max(ba.max, bb.max));
    return aabb_of(b, prims...);
}

template <class ContainerT>
[[nodiscard]] constexpr auto aabb_of(ContainerT const& c) -> decltype(aabb_of(*c.begin()))
{
    auto it = c.begin();
    auto end = c.end();
    TG_ASSERT(it != end && "cannot build AABB of empty container");

    auto bb = aabb_of(*it);
    ++it;
    while (it != end)
    {
        auto rhs = aabb_of(*it);
        bb = decltype(bb)(min(bb.min, rhs.min), max(bb.max, rhs.max));

        ++it;
    }

    return bb;
}

} // namespace tg
