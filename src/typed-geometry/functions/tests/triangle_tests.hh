#pragma once

#include <typed-geometry/detail/utility.hh>
#include <typed-geometry/functions/vector/distance.hh>
#include <typed-geometry/types/objects/triangle.hh>

namespace tg
{
enum class triangle_obtuseness
{
    obtuse, // largest angle > 90°
    right,  // largest angle = 90°
    acute   // largest angles < 90°
};

template <int D, class ScalarT>
[[nodiscard]] constexpr triangle_obtuseness classify_obtuseness(triangle<D, ScalarT> const& t)
{
    auto e0sqr = tg::distance_sqr(t.pos0, t.pos1);
    auto e1sqr = tg::distance_sqr(t.pos1, t.pos2);
    auto e2sqr = tg::distance_sqr(t.pos2, t.pos0);

    // make e0sqr longest (squared) edge
    if (e1sqr > e0sqr)
        tg::detail::swap(e0sqr, e1sqr);
    if (e2sqr > e0sqr)
        tg::detail::swap(e0sqr, e2sqr);

    auto test = e0sqr - e1sqr - e2sqr;
    if (test > ScalarT(0))
        return triangle_obtuseness::obtuse;
    if (test < ScalarT(0))
        return triangle_obtuseness::acute;
    return triangle_obtuseness::right;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr bool is_obtuse_triangle(triangle<D, ScalarT> const& t)
{
    return classify_obtuseness(t) == triangle_obtuseness::obtuse;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr bool is_right_triangle(triangle<D, ScalarT> const& t)
{
    return classify_obtuseness(t) == triangle_obtuseness::right;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr bool is_nonobtuse_triangle(triangle<D, ScalarT> const& t)
{
    return classify_obtuseness(t) != triangle_obtuseness::obtuse;
}

template <int D, class ScalarT>
[[nodiscard]] constexpr bool is_acute_triangle(triangle<D, ScalarT> const& t)
{
    return classify_obtuseness(t) == triangle_obtuseness::acute;
}
}
