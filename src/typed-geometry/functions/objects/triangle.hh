#pragma once

#include <typed-geometry/functions/objects/distance.hh>
#include <typed-geometry/types/objects/sphere.hh>
#include <typed-geometry/types/objects/triangle.hh>

namespace tg
{
template <class ScalarT>
[[nodiscard]] sphere<2, ScalarT> circumcircle(triangle<2, ScalarT> const& t)
{
    auto const& a = t.pos0;
    auto const& b = t.pos1;
    auto const& c = t.pos2;

    auto const d = ScalarT(2) * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
    auto const a_len_sqr = distance_sqr_to_origin(a);
    auto const b_len_sqr = distance_sqr_to_origin(b);
    auto const c_len_sqr = distance_sqr_to_origin(c);
    auto const x = (a_len_sqr * (b.y - c.y) + b_len_sqr * (c.y - a.y) + c_len_sqr * (a.y - b.y)) / d;
    auto const y = (a_len_sqr * (c.x - b.x) + b_len_sqr * (a.x - c.x) + c_len_sqr * (b.x - a.x)) / d;

    auto const center = pos<2, ScalarT>(x, y);
    auto const r = distance(center, a);

    return {center, r};
}
}
