#pragma once

#include "../../types/pos.hh"
#include "../../types/size.hh"
#include "../../types/vec.hh"
#include "../macros.hh"
#include "../traits.hh"

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

// assignment ops
TG_IMPL_DEFINE_ASSIGNMENT_OP(pos, vec, +);
TG_IMPL_DEFINE_ASSIGNMENT_OP(pos, vec, -);
TG_IMPL_DEFINE_ASSIGNMENT_OP(pos, size, *);
TG_IMPL_DEFINE_ASSIGNMENT_OP(pos, size, /);

TG_IMPL_DEFINE_ASSIGNMENT_OP_SCALAR(pos, +);
TG_IMPL_DEFINE_ASSIGNMENT_OP_SCALAR(pos, -);
TG_IMPL_DEFINE_ASSIGNMENT_OP_SCALAR(pos, *);
TG_IMPL_DEFINE_ASSIGNMENT_OP_SCALAR(pos, /);

} // namespace tg
