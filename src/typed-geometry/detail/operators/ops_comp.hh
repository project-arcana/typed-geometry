#pragma once

#include <typed-geometry/detail/macros.hh>
#include <typed-geometry/types/comp.hh>

namespace tg
{
// TODO: make proper generic comp ops

// comp is always component-wise
TG_IMPL_DEFINE_BINARY_OP(comp, comp, comp, +);
TG_IMPL_DEFINE_BINARY_OP(comp, comp, comp, -);
TG_IMPL_DEFINE_BINARY_OP(comp, comp, comp, *);
TG_IMPL_DEFINE_BINARY_OP(comp, comp, comp, /);

// color
TG_IMPL_DEFINE_BINARY_OP(comp, color, color, +);
TG_IMPL_DEFINE_BINARY_OP(color, comp, color, +);
TG_IMPL_DEFINE_BINARY_OP(comp, color, color, -);
TG_IMPL_DEFINE_BINARY_OP(color, comp, color, -);
TG_IMPL_DEFINE_BINARY_OP(comp, color, color, *);
TG_IMPL_DEFINE_BINARY_OP(color, comp, color, *);
TG_IMPL_DEFINE_BINARY_OP(comp, color, color, /);
TG_IMPL_DEFINE_BINARY_OP(color, comp, color, /);

// dir
TG_IMPL_DEFINE_BINARY_OP(comp, dir, dir, +);
TG_IMPL_DEFINE_BINARY_OP(dir, comp, dir, +);
TG_IMPL_DEFINE_BINARY_OP(comp, dir, dir, -);
TG_IMPL_DEFINE_BINARY_OP(dir, comp, dir, -);
TG_IMPL_DEFINE_BINARY_OP(comp, dir, dir, *);
TG_IMPL_DEFINE_BINARY_OP(dir, comp, dir, *);
TG_IMPL_DEFINE_BINARY_OP(comp, dir, dir, /);
TG_IMPL_DEFINE_BINARY_OP(dir, comp, dir, /);

// pos
TG_IMPL_DEFINE_BINARY_OP(comp, pos, pos, +);
TG_IMPL_DEFINE_BINARY_OP(pos, comp, pos, +);
TG_IMPL_DEFINE_BINARY_OP(comp, pos, pos, -);
TG_IMPL_DEFINE_BINARY_OP(pos, comp, pos, -);
TG_IMPL_DEFINE_BINARY_OP(comp, pos, pos, *);
TG_IMPL_DEFINE_BINARY_OP(pos, comp, pos, *);
TG_IMPL_DEFINE_BINARY_OP(comp, pos, pos, /);
TG_IMPL_DEFINE_BINARY_OP(pos, comp, pos, /);

// size
TG_IMPL_DEFINE_BINARY_OP(comp, size, size, +);
TG_IMPL_DEFINE_BINARY_OP(size, comp, size, +);
TG_IMPL_DEFINE_BINARY_OP(comp, size, size, -);
TG_IMPL_DEFINE_BINARY_OP(size, comp, size, -);
TG_IMPL_DEFINE_BINARY_OP(comp, size, size, *);
TG_IMPL_DEFINE_BINARY_OP(size, comp, size, *);
TG_IMPL_DEFINE_BINARY_OP(comp, size, size, /);
TG_IMPL_DEFINE_BINARY_OP(size, comp, size, /);

// vec
TG_IMPL_DEFINE_BINARY_OP(comp, vec, vec, +);
TG_IMPL_DEFINE_BINARY_OP(vec, comp, vec, +);
TG_IMPL_DEFINE_BINARY_OP(comp, vec, vec, -);
TG_IMPL_DEFINE_BINARY_OP(vec, comp, vec, -);
TG_IMPL_DEFINE_BINARY_OP(comp, vec, vec, *);
TG_IMPL_DEFINE_BINARY_OP(vec, comp, vec, *);
TG_IMPL_DEFINE_BINARY_OP(comp, vec, vec, /);
TG_IMPL_DEFINE_BINARY_OP(vec, comp, vec, /);

TG_IMPL_DEFINE_UNARY_OP(comp, +);
TG_IMPL_DEFINE_UNARY_OP(comp, -);
TG_IMPL_DEFINE_UNARY_OP(comp, !);

// scalar OP comp, comp OP scalar
TG_IMPL_DEFINE_BINARY_OP_SCALAR(comp, -);
TG_IMPL_DEFINE_BINARY_OP_SCALAR(comp, +);
TG_IMPL_DEFINE_BINARY_OP_SCALAR(comp, *);
TG_IMPL_DEFINE_BINARY_OP_SCALAR_DIV(comp);
}
