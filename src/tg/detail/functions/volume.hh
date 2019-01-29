#pragma once

#include "../../types/box.hh"
#include "../../types/size.hh"
#include "../operators/ops_vec.hh"
#include "../scalar_traits.hh"

namespace tg
{
template <class ScalarT>
constexpr squared_result<ScalarT> area(size<3, ScalarT> const& s)
{
    return s.width * s.height * s.depth;
}

template <class ScalarT>
constexpr squared_result<ScalarT> area(box<3, ScalarT> const& b)
{
    return area(size<3, ScalarT>(b.max - b.min));
}
} // namespace tg
