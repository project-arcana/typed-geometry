#pragma once

#include "../../types/mat.hh"

namespace tg
{
template <int C, int R, class T>
constexpr mat<R, C, T> transpose(mat<C, R, T> const& m)
{
    mat<R, C, T> r;
    for (auto y = 0; y < R; ++y)
        for (auto x = 0; x < C; ++x)
            r[y][x] = m[x][y];
    return r;
}
} // namespace tg
