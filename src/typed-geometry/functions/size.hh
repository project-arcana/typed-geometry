#pragma once

#include <typed-geometry/types/size.hh>

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/box.hh>

#include <typed-geometry/detail/operators/ops_pos.hh>

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr size<D, ScalarT> size_of(aabb<D, ScalarT> const& b)
{
    return size<D, ScalarT>(b.max - b.min);
}

// TODO: size_of(box)
}
