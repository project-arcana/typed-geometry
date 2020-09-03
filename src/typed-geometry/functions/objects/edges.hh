#pragma once

#include <typed-geometry/types/array.hh>
#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/triangle.hh>

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] array<segment<D, ScalarT>, 3> edges(triangle<D, ScalarT> const& t)
{
    return {{{t.pos0, t.pos1}, {t.pos1, t.pos2}, {t.pos2, t.pos0}}};
}

template <class ScalarT>
[[nodiscard]] array<segment<2, ScalarT>, 4> edges(aabb<2, ScalarT> const& bb)
{
    auto p00 = pos<3, ScalarT>(bb.min.x, bb.min.y);
    auto p01 = pos<3, ScalarT>(bb.min.x, bb.max.y);
    auto p10 = pos<3, ScalarT>(bb.max.x, bb.min.y);
    auto p11 = pos<3, ScalarT>(bb.max.x, bb.max.y);

    return {{{p00, p01}, {p01, p10}, {p10, p11}, {p11, p00}}};
}

template <class ScalarT>
[[nodiscard]] array<segment<3, ScalarT>, 12> edges(aabb<3, ScalarT> const& bb)
{
    auto p000 = pos<3, ScalarT>(bb.min.x, bb.min.y, bb.min.z);
    auto p001 = pos<3, ScalarT>(bb.min.x, bb.min.y, bb.max.z);
    auto p010 = pos<3, ScalarT>(bb.min.x, bb.max.y, bb.min.z);
    auto p011 = pos<3, ScalarT>(bb.min.x, bb.max.y, bb.max.z);
    auto p100 = pos<3, ScalarT>(bb.max.x, bb.min.y, bb.min.z);
    auto p101 = pos<3, ScalarT>(bb.max.x, bb.min.y, bb.max.z);
    auto p110 = pos<3, ScalarT>(bb.max.x, bb.max.y, bb.min.z);
    auto p111 = pos<3, ScalarT>(bb.max.x, bb.max.y, bb.max.z);

    return {{
        {p000, p100},
        {p001, p101},
        {p010, p110},
        {p011, p111}, // x dir

        {p000, p010},
        {p001, p011},
        {p100, p110},
        {p101, p111}, // y dir

        {p000, p001},
        {p010, p011},
        {p100, p101},
        {p110, p111}, // z dir
    }};
}
}
