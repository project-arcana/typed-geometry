#pragma once

#include "../../types/objects/ball.hh"
#include "../../types/objects/box.hh"
#include "../../types/size.hh"
#include "../constants.hh"
#include "../operators/ops_vec.hh"
#include "../scalar_traits.hh"
#include "../scalars/scalar_math.hh"

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
    return volume(size<3, ScalarT>(b.max - b.min));
}

template <class ScalarT>
TG_NODISCARD constexpr squared_result<ScalarT> volume(ball<3, ScalarT> const& b)
{
    return (tg::pi<ScalarT>.radians() * ScalarT(4) / ScalarT(3)) * tg::pow3(b.radius);
}
} // namespace tg
