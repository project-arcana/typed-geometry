#pragma once

#include <typed-geometry/common/assert.hh>

#include <typed-geometry/types/objects/aabb.hh>
#include <typed-geometry/types/objects/triangle.hh>

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr aabb<D, ScalarT> aabb_of(pos<D, ScalarT> const& v)
{
    return {v, v};
}

template <int D, class ScalarT>
TG_NODISCARD constexpr aabb<D, ScalarT> aabb_of(aabb<D, ScalarT> const& b)
{
    return b;
}

template <int D, class ScalarT>
TG_NODISCARD constexpr aabb<D, ScalarT> aabb_of(triangle<D, ScalarT> const& t)
{
    return aabb_of(t.pos0, t.pos1, t.pos2);
}

template <class PrimA, class PrimB, class... PrimsT>
TG_NODISCARD constexpr auto aabb_of(PrimA const& pa, PrimB const& pb, PrimsT const&... prims) -> decltype(aabb_of(pa))
{
    auto ba = aabb_of(pa);
    auto bb = aabb_of(pb);
    static_assert(is_same<decltype(ba), decltype(bb)>, "all arguments must have the same aabb aabb type");
    auto b = decltype(ba)(min(ba.min, bb.min), max(ba.max, bb.max));
    return aabb_of(b, prims...);
}

template <class ContainerT>
TG_NODISCARD constexpr auto aabb_of(ContainerT const& c) -> decltype((void)aabb_of(*c.begin()), aabb_of(*c.end()))
{
    auto it = c.begin();
    auto end = c.end();
    TG_ASSERT(it != end && "cannot build AABB of empty container");

    auto bb = aabb_of(*it);
    it++;
    while (it != end)
    {
        auto rhs = aabb_of(*it);
        bb = decltype(bb)(min(bb.min, rhs.min), max(bb.max, rhs.max));

        it++;
    }

    return bb;
}

} // namespace tg
