#pragma once

#include <typed-geometry/types/array.hh>
#include <typed-geometry/types/objects/triangle.hh>

namespace tg
{
template <int D, class T>
[[nodiscard]] array<pos<D, T>, 3> vertices(triangle<D, T> const& t)
{
    return {{t.pos0, t.pos1, t.pos2}};
}
}
