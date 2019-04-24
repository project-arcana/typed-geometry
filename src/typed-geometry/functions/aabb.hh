#pragma once

#include <typed-geometry/types/objects/box.hh>
#include <typed-geometry/types/objects/triangle.hh>

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr box<D, ScalarT> aabb_of(pos<D, ScalarT> const& v)
{
    return {v, v};
}

template <int D, class ScalarT>
TG_NODISCARD constexpr box<D, ScalarT> aabb_of(box<D, ScalarT> const& b)
{
    return b;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr box<D, ScalarT> aabb_of(triangle<D, ScalarT> const& t)
{
    return aabb_of(t.pos0, t.pos1, t.pos2);
}

template <class PrimA, class PrimB, class... PrimsT>
TG_NODISCARD constexpr auto aabb_of(PrimA const& pa, PrimB const& pb, PrimsT const&... prims) -> decltype(aabb_of(pa))
{
    auto ba = aabb_of(pa);
    auto bb = aabb_of(pb);
    static_assert(is_same<decltype(ba), decltype(bb)>, "all arguments must have the same aabb box type");
    auto b = decltype(ba)(min(ba.min, bb.min), max(ba.max, bb.max));
    return aabb_of(b, prims...);
}


} // namespace tg
