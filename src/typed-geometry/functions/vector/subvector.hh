#pragma once

#include <typed-geometry/detail/constexpr_helper.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <int SX, int SD, int D, class ScalarT>
[[nodiscard]] constexpr vec<SD, ScalarT> subvector(vec<D, ScalarT> const& v)
{
    static_assert(SD <= D, "subvector cannot be larger");
    static_assert(SX >= 0, "subvector must start at least at 0");
    static_assert(SD + SX <= D, "subvector must be contained");

    vec<SD, ScalarT> r;
    for (auto i = 0; i < SD; ++i)
        detail::csubscript(r, i) = detail::csubscript(v, i + SX);
    return r;
}
} // namespace tg
