#pragma once

#include <typed-geometry/detail/macros.hh>
#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/size.hh>

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
