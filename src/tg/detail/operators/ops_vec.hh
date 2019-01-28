#pragma once

#include "../../types/vec.hh"
#include "../macros.hh"
#include "../traits.hh"

namespace tg
{
// vec +- vec = vec
TG_IMPL_DEFINE_BINARY_OP(vec, vec, vec, +);
TG_IMPL_DEFINE_BINARY_OP(vec, vec, vec, -);

// vec */ size = vec
TG_IMPL_DEFINE_BINARY_OP(vec, size, vec, *);
TG_IMPL_DEFINE_BINARY_OP(vec, size, vec, /);

// +vec, -vec
TG_IMPL_DEFINE_UNARY_OP(vec, +);
TG_IMPL_DEFINE_UNARY_OP(vec, -);

// scalar OP vec, vec OP scalar
TG_IMPL_DEFINE_BINARY_OP_SCALAR(vec, -);
TG_IMPL_DEFINE_BINARY_OP_SCALAR(vec, +);
TG_IMPL_DEFINE_BINARY_OP_SCALAR(vec, *);
TG_IMPL_DEFINE_BINARY_OP_SCALAR_DIV(vec);

// assignment ops
TG_IMPL_DEFINE_ASSIGNMENT_OP(vec, vec, +);
TG_IMPL_DEFINE_ASSIGNMENT_OP(vec, vec, -);
TG_IMPL_DEFINE_ASSIGNMENT_OP(vec, size, *);
TG_IMPL_DEFINE_ASSIGNMENT_OP(vec, size, /);

TG_IMPL_DEFINE_ASSIGNMENT_OP_SCALAR(vec, +);
TG_IMPL_DEFINE_ASSIGNMENT_OP_SCALAR(vec, -);
TG_IMPL_DEFINE_ASSIGNMENT_OP_SCALAR(vec, *);
TG_IMPL_DEFINE_ASSIGNMENT_OP_SCALAR(vec, /);
} // namespace tg
