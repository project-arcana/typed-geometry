#pragma once

#include <typed-geometry/types/array.hh>
#include <typed-geometry/types/objects/segment.hh>
#include <typed-geometry/types/objects/triangle.hh>

namespace tg
{
template <int D, class T>
[[nodiscard]] array<segment<D, T>, 3> edges(triangle<D, T> const& t)
{
    return {{{t.pos0, t.pos1}, {t.pos1, t.pos2}, {t.pos2, t.pos0}}};
}
}
