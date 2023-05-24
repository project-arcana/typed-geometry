#pragma once

#include <typed-geometry/tg.hh>

namespace helper
{
template <int D = 3, class ScalarT = tg::f32>
tg::vec<D, ScalarT> rndVec(tg::rng& rng, ScalarT range = ScalarT(5.0))
{
    auto box = tg::aabb<D, ScalarT>(-tg::pos<D, ScalarT>(range), tg::pos<D, ScalarT>(range));
    return tg::vec<D, ScalarT>(uniform(rng, box));
}

} // namespace helper
