#pragma once

#include "../../types/objects/box.hh"
#include "../../types/size.hh"
#include "../operators/ops_vec.hh"
#include "../scalar_traits.hh"

namespace tg
{
template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> volume(size<3, ScalarT> const& s)
{
    return s.width * s.height * s.depth;
}

template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> volume(box<3, ScalarT> const& b)
{
    return area(size<3, ScalarT>(b.max - b.min));
}
} // namespace tg
