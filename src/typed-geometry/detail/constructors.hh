#pragma once

#include <typed-geometry/types/objects/hyperplane.hh>

#include <typed-geometry/detail/functions/dot.hh>

// Header for all constructors that depend on functions

namespace tg
{
template <int D, class ScalarT>
constexpr hyperplane<D, ScalarT>::hyperplane(vec_t n, pos_t p) : n(n), d(dot(p, n))
{
}
}
