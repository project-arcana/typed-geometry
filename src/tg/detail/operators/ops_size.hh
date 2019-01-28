#pragma once

#include "../../types/size.hh"
#include "../macros.hh"
#include "../traits.hh"

namespace tg
{
// size +- size = size
TG_IMPL_DEFINE_BINARY_OP(size, size, size, +);
TG_IMPL_DEFINE_BINARY_OP(size, size, size, -);

// size */ size = size
TG_IMPL_DEFINE_BINARY_OP(size, size, size, *);
TG_IMPL_DEFINE_BINARY_OP(size, size, size, /);

// +size, -size
TG_IMPL_DEFINE_UNARY_OP(size, +);
TG_IMPL_DEFINE_UNARY_OP(size, -);

// scalar OP size, size OP scalar
TG_IMPL_DEFINE_BINARY_OP_SCALAR(size, -);
TG_IMPL_DEFINE_BINARY_OP_SCALAR(size, +);
TG_IMPL_DEFINE_BINARY_OP_SCALAR(size, *);
TG_IMPL_DEFINE_BINARY_OP_SCALAR_DIV(size);
} // namespace tg
