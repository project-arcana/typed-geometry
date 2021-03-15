#pragma once

#include <typed-geometry/feature/basic.hh>
#include <typed-geometry/functions/objects/edges.hh>

namespace tg
{
/// calls on_segment for each edge of the object
/// on_segment: (tg::segment) -> void
template <class Obj, class OnSegment, std::enable_if_t<has_edges_of<Obj>, int> = 0>
constexpr void segmentize(Obj const& obj, OnSegment&& on_segment)
{
    for (auto&& s : edges_of(obj))
        on_segment(s);
}

/// calls on_segment for a discretized version of the sphere
template <class ScalarT, class TraitsT, class OnSegment>
void segmentize(sphere<2, ScalarT, 2, TraitsT> const& s, int segs, OnSegment&& on_segment)
{
    TG_ASSERT(segs >= 3);

    using dir_t = dir<2, ScalarT>;

    auto const dir_of = [&](int i) -> dir_t {
        if (i == 0 || i == segs)
            return dir_t(1, 0);

        auto [sa, ca] = sin_cos(tau<ScalarT> * i / ScalarT(segs));
        return dir_t(ca, sa);
    };
    auto const pos_of = [&](int i) {
        auto d = dir_of(i);
        return s.center + d * s.radius;
    };

    auto prev = pos_of(0);
    for (auto i = 1; i <= segs; ++i)
    {
        auto curr = pos_of(i);
        on_segment(tg::segment<2, ScalarT>(prev, curr));
        prev = curr;
    }
}
template <class ScalarT, class TraitsT, class OnSegment>
void segmentize(sphere<2, ScalarT, 3, TraitsT> const& s, int segs, OnSegment&& on_segment)
{
    TG_ASSERT(segs >= 3);
    auto const dx = any_normal(s.normal);
    auto const dy = cross(s.normal, dx);

    using dir_t = dir<2, ScalarT>;

    auto const dir_of = [&](int i) -> dir_t {
        if (i == 0 || i == segs)
            return dir_t(1, 0);

        auto [sa, ca] = sin_cos(tau<ScalarT> * i / ScalarT(segs));
        return dir_t(ca, sa);
    };
    auto const pos_of = [&](int i) {
        auto d = dir_of(i) * s.radius;
        return s.center + dx * d.x + dy * d.y;
    };

    auto prev = pos_of(0);
    for (auto i = 1; i <= segs; ++i)
    {
        auto curr = pos_of(i);
        on_segment(tg::segment<3, ScalarT>(prev, curr));
        prev = curr;
    }
}
}
