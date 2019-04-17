#pragma once

#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/size.hh>
#include <typed-geometry/types/vec.hh>
#include "macros.hh"

namespace tg
{
TG_IMPL_DEFINE_CONV_CTOR_IMPL(vec, pos);
TG_IMPL_DEFINE_CONV_CTOR_IMPL(vec, size);

TG_IMPL_DEFINE_CONV_CTOR_IMPL(pos, vec);
TG_IMPL_DEFINE_CONV_CTOR_IMPL(pos, size);

TG_IMPL_DEFINE_CONV_CTOR_IMPL(size, vec);
TG_IMPL_DEFINE_CONV_CTOR_IMPL(size, pos);
} // namespace tg
