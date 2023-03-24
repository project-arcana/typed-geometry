#pragma once

// Add this message because the [[deprecated]] attribute for tg::optional does not create warnings because it is templated
#pragma message("Header <typed-geometry/detail/optional.hh> is deprecated")

#include <clean-core/optional.hh>

namespace tg
{
template <class T>
using optional [[deprecated("tg::optional has been replaced by cc::optional")]] = cc::optional<T>;
}
