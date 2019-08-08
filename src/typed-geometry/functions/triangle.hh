#pragma once

#include <typed-geometry/functions/distance.hh>
#include <typed-geometry/types/objects/triangle.hh>

namespace tg
{
enum class obtuseness_class
{
    obtuse,   // one angle > 90°
    right,    // one angle = 90°
    nonobtuse // all angles < 90° (aka acute)
};

template <int D, class ScalarT>
TG_NODISCARD constexpr obtuseness_class triangle_obtuseness(triangle<D, ScalarT> const& t)
{
    auto e0sqr = tg::distance(t.pos0, t.pos1);
    auto e1sqr = tg::distance(t.pos1, t.pos2);
    auto e2sqr = tg::distance(t.pos2, t.pos0);

    // make e0sqr longest (squared) edge
    if (e1sqr > e0sqr)
        std::swap(e0sqr, e1sqr);
    if (e2sqr > e0sqr)
        std::swap(e0sqr, e2sqr);

    auto test = e0sqr - e1sqr - e2sqr;
    if (test > ScalarT(0))
        return obtuseness_class::obtuse;
    if (test < ScalarT(0))
        return obtuseness_class::nonobtuse;
    return obtuseness_class::right;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr obtuseness_class is_obtuse_triangle(triangle<D, ScalarT> const& t)
{
    return triangle_obtuseness(t) == obtuseness_class::obtuse;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr obtuseness_class is_right_triangle(triangle<D, ScalarT> const& t)
{
    return triangle_obtuseness(t) == obtuseness_class::right;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr obtuseness_class is_nonobtuse_triangle(triangle<D, ScalarT> const& t)
{
    return triangle_obtuseness(t) == obtuseness_class::nonobtuse;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr obtuseness_class is_acute_triangle(triangle<D, ScalarT> const& t)
{
    return is_nonobtuse_triangle(t);
}
}
