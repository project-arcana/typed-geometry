#pragma once

#include <typed-geometry/detail/macros.hh>
#include <typed-geometry/types/color.hh>

namespace tg
{
// scalar OP color, color OP scalar
TG_IMPL_DEFINE_BINARY_OP_SCALAR(color, -);
TG_IMPL_DEFINE_BINARY_OP_SCALAR(color, +);
TG_IMPL_DEFINE_BINARY_OP_SCALAR(color, *);
TG_IMPL_DEFINE_BINARY_OP_SCALAR_DIV(color);
}
