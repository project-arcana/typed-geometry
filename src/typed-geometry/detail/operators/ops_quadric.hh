#pragma once

#include <typed-geometry/types/quadric.hh>

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr quadric<D, ScalarT> operator+(quadric<D, ScalarT> const& a, quadric<D, ScalarT> const& b)
{
    quadric<D, ScalarT> r = a; // copy
    r.add(b);
    return r;
}

// TODO: scalar scaling + division?
}
