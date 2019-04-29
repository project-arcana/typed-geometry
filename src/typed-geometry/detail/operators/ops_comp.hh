#pragma once

#include <typed-geometry/detail/macros.hh>
#include <typed-geometry/types/comp.hh>

namespace tg
{
// comp is always component-wise
TG_IMPL_DEFINE_BINARY_OP(comp, comp, comp, +);
TG_IMPL_DEFINE_BINARY_OP(comp, comp, comp, -);
TG_IMPL_DEFINE_BINARY_OP(comp, comp, comp, *);
TG_IMPL_DEFINE_BINARY_OP(comp, comp, comp, /);

TG_IMPL_DEFINE_UNARY_OP(comp, +);
TG_IMPL_DEFINE_UNARY_OP(comp, -);
}
