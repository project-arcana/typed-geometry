#pragma once

#include <typed-geometry/types/quadric.hh>

namespace tg
{
template <class ScalarT>
[[nodiscard]] constexpr quadric<3, ScalarT> operator+(quadric<3, ScalarT> const& a, quadric<3, ScalarT> const& b)
{
    quadric<3, ScalarT> r;
    r.add(a);
    r.add(b);
    return r;
}

// TODO: scalar scaling + division?
}
