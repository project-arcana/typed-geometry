#pragma once

#include <typed-geometry/detail/macros.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/quadric.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr quadric<D, ScalarT> point_quadric(tg::pos<D, ScalarT> const& p)
{
    quadric<D, ScalarT> q;
    q.add(mat<D, ScalarT>::identity(), vec<D, ScalarT>(p), ScalarT(0));
    return q;
}
}
