#pragma once

#include <typed-geometry/functions/tests/triangle_tests.hh>
#include <typed-geometry/functions/vector/distance.hh>
#include <typed-geometry/functions/vector/interpolate.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr aabb<D, ScalarT> shrink(aabb<D, ScalarT> const& b, tg::dont_deduce<ScalarT> s)
{
    // TODO: what if result is invalid?
    return {b.min + s, b.max - s};
}
template <int D, class ScalarT>
[[nodiscard]] constexpr aabb<D, ScalarT> shrink(aabb<D, ScalarT> const& b, size<D, ScalarT> s)
{
    // TODO: what if result is invalid?
    return {b.min + tg::vec<D, ScalarT>(s), b.max - tg::vec<D, ScalarT>(s)};
}
template <int D, class ScalarT>
[[nodiscard]] constexpr aabb<D, ScalarT> expand(aabb<D, ScalarT> const& b, tg::dont_deduce<ScalarT> s)
{
    // TODO: what if result is invalid?
    return {b.min - s, b.max + s};
}
template <int D, class ScalarT>
[[nodiscard]] constexpr aabb<D, ScalarT> expand(aabb<D, ScalarT> const& b, size<D, ScalarT> s)
{
    // TODO: what if result is invalid?
    return {b.min - tg::vec<D, ScalarT>(s), b.max + tg::vec<D, ScalarT>(s)};
}

template <int D, class ScalarT>
[[nodiscard]] constexpr sphere<D, ScalarT> bounding_sphere_of(triangle<D, ScalarT> const& t)
{
    auto e0sqr = distance_sqr(t.pos0, t.pos1);
    auto e1sqr = distance_sqr(t.pos1, t.pos2);
    auto e2sqr = distance_sqr(t.pos2, t.pos0);

    auto const* a = &t.pos2;
    auto const* b = &t.pos0;
    auto const* c = &t.pos1;

    // "rotate" triangle s.t. e0 is the largest edge and
    // lies opposite of point a (equivalently, e1 <-> b, e2 <-> c)
    if (e2sqr > e1sqr)
    {
        // make e1 larger than e2
        tg::detail::swap(e1sqr, e2sqr);
        tg::detail::swap(b, c);
    }
    if (e1sqr > e0sqr)
    {
        // make e0 larger than e1
        tg::detail::swap(e0sqr, e1sqr);
        tg::detail::swap(a, b);
    }

    if (e0sqr >= e1sqr + e2sqr) // triangle is obtuse or right
    {
        auto radius = sqrt(e0sqr) / ScalarT(2);
        auto center = mix(*b, *c, ScalarT(0.5));
        return {center, radius};
    }

    // compute barycentric coords
    auto alpha = e0sqr * (e1sqr + e2sqr - e0sqr);
    auto beta = e1sqr * (e2sqr + e0sqr - e1sqr);
    auto gamma = e2sqr * (e0sqr + e1sqr - e2sqr);
    auto bary_sum = alpha + beta + gamma;

    auto center = tg::interpolate(triangle(*a, *b, *c), alpha, beta, gamma) / bary_sum;
    auto radius = tg::distance(center, t.pos0);

    return {center, radius};
}
}
