#pragma once

#include "../../types/line.hh"
#include "../../types/plane.hh"
#include "../../types/pos.hh"
#include "../../types/vec.hh"
#include "../operators/ops_vec.hh"

namespace tg
{
template <int D, class ScalarT>
constexpr vec<D, ScalarT> project(vec<D, ScalarT> const& a, vec<D, ScalarT> const& b)
{
    return dot(a, b) / length(b);
}
} // namespace tg
