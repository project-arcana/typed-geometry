#pragma once

#include "../../types/pos.hh"
#include "../../types/size.hh"
#include "../../types/vec.hh"
#include "../scalar_traits.hh"
#include "../scalars/scalar_math.hh"

namespace tg
{
// pos
TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(pos, floor);
TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(pos, ifloor);

TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(pos, ceil);
TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(pos, iceil);

TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(pos, round);
TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(pos, iround);

// size
TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(vec, floor);
TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(vec, ifloor);

TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(vec, ceil);
TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(vec, iceil);

TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(vec, round);
TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(vec, iround);

// vec
TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(size, floor);
TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(size, ifloor);

TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(size, ceil);
TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(size, iceil);

TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(size, round);
TG_IMPL_DEFINE_COMPWISE_FUNC_UNARY(size, iround);

} // namespace tg
