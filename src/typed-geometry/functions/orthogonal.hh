#pragma once

//#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/vec.hh>

// Computes orthogonal vector or direction respectively
namespace tg
{
// Base case for vector
template <int D, class ScalarT>
TG_NODISCARD constexpr vec<D, ScalarT> distance2(vec<D, ScalarT> const& v)
{
    // TODO find strongest component, use that unit vector in cross
}
} // namespace tg
