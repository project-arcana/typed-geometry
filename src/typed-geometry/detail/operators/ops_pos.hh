#pragma once

#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/size.hh>
#include <typed-geometry/types/vec.hh>
#include <typed-geometry/detail/macros.hh>
#include <typed-geometry/detail/scalar_traits.hh>

namespace tg
{
// pos +- vec = pos
TG_IMPL_DEFINE_BINARY_OP(pos, vec, pos, +);
TG_IMPL_DEFINE_BINARY_OP(pos, vec, pos, -);

// pos */ size = pos
TG_IMPL_DEFINE_BINARY_OP(pos, size, pos, *);
TG_IMPL_DEFINE_BINARY_OP(pos, size, pos, /);

// pos - pos = vec
TG_IMPL_DEFINE_BINARY_OP(pos, pos, vec, -);

// +pos, -pos
TG_IMPL_DEFINE_UNARY_OP(pos, +);
TG_IMPL_DEFINE_UNARY_OP(pos, -);

// scalar OP pos, pos OP scalar
TG_IMPL_DEFINE_BINARY_OP_SCALAR(pos, -);
TG_IMPL_DEFINE_BINARY_OP_SCALAR(pos, +);
TG_IMPL_DEFINE_BINARY_OP_SCALAR(pos, *);
TG_IMPL_DEFINE_BINARY_OP_SCALAR_DIV(pos);

} // namespace tg
