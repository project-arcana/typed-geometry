#pragma once

#include <typed-geometry/types/scalars/default.hh>
#include "../detail/comp_traits.hh"
#include "../detail/macros.hh"
#include "../detail/scalar_traits.hh"
#include "../detail/utility.hh"
#include "fwd.hh"

namespace tg
{
template <class ScalarT>
struct quat;

using fquat = quat<float>;
using dquat = quat<double>;

template <class ScalarT>
struct quat
{
    ScalarT x = ScalarT(0); // imag x
    ScalarT y = ScalarT(0); // imag y
    ScalarT z = ScalarT(0); // imag z
    ScalarT w = ScalarT(0); // real

    static const quat zero;
    static const quat identity;

    TG_DECLARE_COMP_TYPE_4_NO_SWIZZLE(quat);

    TG_NODISCARD constexpr bool operator==(quat const& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
    TG_NODISCARD constexpr bool operator!=(quat const& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w; }
};
template <class T>
const quat<T> quat<T>::zero = {T(0), T(0), T(0), T(0)};
template <class T>
const quat<T> quat<T>::identity = {T(0), T(0), T(0), T(1)};
}
