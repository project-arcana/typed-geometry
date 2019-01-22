#pragma once

#include "../../types/pos.hh"
#include "../../types/vec.hh"
#include "../scalars/scalar_math.hh"
#include "../traits.hh"

namespace tg
{
// TODO: pos, size, vec

TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(pos, floor);
TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(pos, ifloor);

TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(pos, ceil);
TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(pos, iceil);

TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(pos, round);
TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(pos, iround);

} // namespace tg
