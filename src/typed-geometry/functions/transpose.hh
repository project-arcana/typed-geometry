#pragma once

#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/vec.hh>

namespace tg
{
template <int C, class ScalarT>
[[nodiscard]] constexpr mat<1, C, ScalarT> transpose(mat<C, 1, ScalarT> const& m)
{
    mat<1, C, ScalarT> r;
    r[0] = m.row(0);
    return r;
}
template <int C, class ScalarT>
[[nodiscard]] constexpr mat<2, C, ScalarT> transpose(mat<C, 2, ScalarT> const& m)
{
    mat<2, C, ScalarT> r;
    r[0] = m.row(0);
    r[1] = m.row(1);
    return r;
}
template <int C, class ScalarT>
[[nodiscard]] constexpr mat<3, C, ScalarT> transpose(mat<C, 3, ScalarT> const& m)
{
    mat<3, C, ScalarT> r;
    r[0] = m.row(0);
    r[1] = m.row(1);
    r[2] = m.row(2);
    return r;
}
template <int C, class ScalarT>
[[nodiscard]] constexpr mat<4, C, ScalarT> transpose(mat<C, 4, ScalarT> const& m)
{
    mat<4, C, ScalarT> r;
    r[0] = m.row(0);
    r[1] = m.row(1);
    r[2] = m.row(2);
    r[3] = m.row(3);
    return r;
}
template <int R, class ScalarT>
[[nodiscard]] constexpr mat<R, 1, ScalarT> transpose(vec<R, ScalarT> const& v)
{
    mat<R, 1, ScalarT> m;
    m.set_row(0, v);
    return m;
}
} // namespace tg
