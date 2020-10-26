#pragma once

#include <initializer_list>

#include <typed-geometry/types/scalars/default.hh>
#include <typed-geometry/types/span.hh>
#include "vec.hh"

#include <typed-geometry/feature/assert.hh>

namespace tg
{
/*
 * Memory layout of a 4x4:
 *  0  4  8 12
 *  1  5  9 13
 *  2  6 10 14
 *  3  7 11 15
 *
 * i.e. col-major
 *
 * matCxR (cols x rows)
 *
 * mat3x4 has no translational part
 * mat4x3 has no projective part
 *
 * mat[i] is i-th column (assignable as vec)
 * mat.col(i), mat.row(i) return i-th column and row, respectively
 *
 * NOTE: default initialization is zero, NOT identity
 *       use mat4::identity to get identity matrix
 */
template <int C, int R, class ScalarT>
struct mat;

// square matrices

using mat1 = mat<1, 1, f32>;
using mat2 = mat<2, 2, f32>;
using mat3 = mat<3, 3, f32>;
using mat4 = mat<4, 4, f32>;

using fmat1 = mat<1, 1, f32>;
using fmat2 = mat<2, 2, f32>;
using fmat3 = mat<3, 3, f32>;
using fmat4 = mat<4, 4, f32>;

using dmat1 = mat<1, 1, f64>;
using dmat2 = mat<2, 2, f64>;
using dmat3 = mat<3, 3, f64>;
using dmat4 = mat<4, 4, f64>;

using imat1 = mat<1, 1, i32>;
using imat2 = mat<2, 2, i32>;
using imat3 = mat<3, 3, i32>;
using imat4 = mat<4, 4, i32>;

using umat1 = mat<1, 1, u32>;
using umat2 = mat<2, 2, u32>;
using umat3 = mat<3, 3, u32>;
using umat4 = mat<4, 4, u32>;

// rectangle matrices

using mat2x2 = mat<2, 2, f32>;
using mat2x3 = mat<2, 3, f32>;
using mat2x4 = mat<2, 4, f32>;
using mat3x2 = mat<3, 2, f32>;
using mat3x3 = mat<3, 3, f32>;
using mat3x4 = mat<3, 4, f32>;
using mat4x2 = mat<4, 2, f32>;
using mat4x3 = mat<4, 3, f32>;
using mat4x4 = mat<4, 4, f32>;

using fmat2x2 = mat<2, 2, f32>;
using fmat2x3 = mat<2, 3, f32>;
using fmat2x4 = mat<2, 4, f32>;
using fmat3x2 = mat<3, 2, f32>;
using fmat3x3 = mat<3, 3, f32>;
using fmat3x4 = mat<3, 4, f32>;
using fmat4x2 = mat<4, 2, f32>;
using fmat4x3 = mat<4, 3, f32>;
using fmat4x4 = mat<4, 4, f32>;

using dmat2x2 = mat<2, 2, f64>;
using dmat2x3 = mat<2, 3, f64>;
using dmat2x4 = mat<2, 4, f64>;
using dmat3x2 = mat<3, 2, f64>;
using dmat3x3 = mat<3, 3, f64>;
using dmat3x4 = mat<3, 4, f64>;
using dmat4x2 = mat<4, 2, f64>;
using dmat4x3 = mat<4, 3, f64>;
using dmat4x4 = mat<4, 4, f64>;

using imat2x2 = mat<2, 2, i32>;
using imat2x3 = mat<2, 3, i32>;
using imat2x4 = mat<2, 4, i32>;
using imat3x2 = mat<3, 2, i32>;
using imat3x3 = mat<3, 3, i32>;
using imat3x4 = mat<3, 4, i32>;
using imat4x2 = mat<4, 2, i32>;
using imat4x3 = mat<4, 3, i32>;
using imat4x4 = mat<4, 4, i32>;

using umat2x2 = mat<2, 2, u32>;
using umat2x3 = mat<2, 3, u32>;
using umat2x4 = mat<2, 4, u32>;
using umat3x2 = mat<3, 2, u32>;
using umat3x3 = mat<3, 3, u32>;
using umat3x4 = mat<3, 4, u32>;
using umat4x2 = mat<4, 2, u32>;
using umat4x3 = mat<4, 3, u32>;
using umat4x4 = mat<4, 4, u32>;


// ======== IMPLEMENTATION ========


namespace detail
{
template <int R, class ScalarT>
constexpr vec<1, ScalarT> mat_row(mat<1, R, ScalarT> const& m, int i)
{
    return {m[0][i]};
}
template <int R, class ScalarT>
constexpr vec<2, ScalarT> mat_row(mat<2, R, ScalarT> const& m, int i)
{
    return {m[0][i], m[1][i]};
}
template <int R, class ScalarT>
constexpr vec<3, ScalarT> mat_row(mat<3, R, ScalarT> const& m, int i)
{
    return {m[0][i], m[1][i], m[2][i]};
}
template <int R, class ScalarT>
constexpr vec<4, ScalarT> mat_row(mat<4, R, ScalarT> const& m, int i)
{
    return {m[0][i], m[1][i], m[2][i], m[3][i]};
}
} // namespace detail

template <int C, int R, class ScalarT>
struct mat
{
private:
    vec<R, ScalarT> m[C];

public:
    constexpr mat() = default;
    template <class Obj, class = enable_if<is_mat_convertible<Obj, ScalarT>>>
    explicit constexpr mat(Obj const& v)
    {
        // init to id
        m[0][0] = ScalarT(1);
        if constexpr (R >= 2 && C >= 2)
            m[1][1] = ScalarT(1);
        if constexpr (R >= 3 && C >= 3)
            m[2][2] = ScalarT(1);
        if constexpr (R >= 4 && C >= 4)
            m[3][3] = ScalarT(1);

        auto s = detail::get_dynamic_comp_size(v);
        m[0] = detail::comp_get(v, 0, s, vec<R, ScalarT>::zero);

        if constexpr (C >= 2)
            m[1] = detail::comp_get(v, 1, s, vec<R, ScalarT>::zero);

        if constexpr (C >= 3)
            m[2] = detail::comp_get(v, 2, s, vec<R, ScalarT>::zero);

        if constexpr (C >= 4)
            m[3] = detail::comp_get(v, 3, s, vec<R, ScalarT>::zero);
    }

    template <class... Args, class = enable_if<sizeof...(Args) == C - 1 && (... && is_same<Args, vec<R, ScalarT>>)>>
    constexpr mat(vec<R, ScalarT> const& c0, Args const&... cN)
    {
        // init to id
        m[0][0] = ScalarT(1);
        if constexpr (R >= 2 && C >= 2)
            m[1][1] = ScalarT(1);
        if constexpr (R >= 3 && C >= 3)
            m[2][2] = ScalarT(1);
        if constexpr (R >= 4 && C >= 4)
            m[3][3] = ScalarT(1);

        m[0] = c0;
        auto i = 1;
        ((m[i++] = cN), ...);
    }

    template <int C2, int R2, class ScalarT2>
    explicit constexpr mat(mat<C2, R2, ScalarT2> const& rhs)
    {
        // init to id
        m[0][0] = ScalarT(1);
        if constexpr (R >= 2 && C >= 2)
            m[1][1] = ScalarT(1);
        if constexpr (R >= 3 && C >= 3)
            m[2][2] = ScalarT(1);
        if constexpr (R >= 4 && C >= 4)
            m[3][3] = ScalarT(1);

        // copy submatrix
        for (auto x = 0; x < (C < C2 ? C : C2); ++x)
            for (auto y = 0; y < (R < R2 ? R : R2); ++y)
                m[x][y] = ScalarT(rhs[x][y]);
    }

    constexpr vec<R, ScalarT>& operator[](int i)
    {
        TG_CONTRACT(0 <= i && i < C);
        return m[i];
    }
    constexpr vec<R, ScalarT> const& operator[](int i) const
    {
        TG_CONTRACT(0 <= i && i < C);
        return m[i];
    }
    constexpr ScalarT& operator()(int col, int row)
    {
        TG_CONTRACT(0 <= col && col < C);
        TG_CONTRACT(0 <= row && row < R);
        return m[col][row];
    }
    constexpr ScalarT const& operator()(int col, int row) const
    {
        TG_CONTRACT(0 <= col && col < C);
        TG_CONTRACT(0 <= row && row < R);
        return m[col][row];
    }

    constexpr vec<R, ScalarT> const col(int i) const
    {
        TG_CONTRACT(0 <= i && i < C);
        return m[i];
    }
    constexpr vec<C, ScalarT> const row(int i) const
    {
        TG_CONTRACT(0 <= i && i < R);
        return detail::mat_row(*this, i);
    }

    constexpr void set_row(int i, vec<C, ScalarT> const& v)
    {
        TG_CONTRACT(0 <= i && i < R);
        m[0][i] = v[0];
        if constexpr (C >= 2)
            m[1][i] = v[1];
        if constexpr (C >= 3)
            m[2][i] = v[2];
        if constexpr (C >= 4)
            m[3][i] = v[3];
    }

    constexpr void set_col(int i, vec<R, ScalarT> const& v)
    {
        TG_CONTRACT(0 <= i && i < C);
        m[i] = v;
    }

    static const mat zero;
    static const mat ones;
    static const mat identity;

    static constexpr mat diag(ScalarT v);
    static constexpr mat diag(vec<detail::min(C, R), ScalarT> const& v);

    /// creates a matrix from contiguous memory, treating the data as row-by-row
    static constexpr mat from_data_rowwise(ScalarT const* data)
    {
        mat<C, R, ScalarT> m;
        m.set_row(0, vec<C, ScalarT>::from_data(data));
        if constexpr (R >= 2)
            m.set_row(1, vec<C, ScalarT>::from_data(data + C));
        if constexpr (R >= 3)
            m.set_row(2, vec<C, ScalarT>::from_data(data + C * 2));
        if constexpr (R >= 4)
            m.set_row(3, vec<C, ScalarT>::from_data(data + C * 3));
        return m;
    }
    static constexpr mat from_data_rowwise(span<ScalarT const> data)
    {
        TG_ASSERT(data.size() == C * R && "wrong amount of data");
        return mat::from_data_rowwise(data.data());
    }
    static constexpr mat from_data_rowwise(std::initializer_list<ScalarT> data)
    {
        TG_ASSERT(data.size() == C * R && "wrong amount of data");
        return mat::from_data_rowwise(data.begin());
    }
    /// creates a matrix from contiguous memory, treating the data as col-by-col
    static constexpr mat from_data_colwise(ScalarT const* data)
    {
        mat<C, R, ScalarT> m;
        m[0] = vec<R, ScalarT>::from_data(data);
        if constexpr (C >= 2)
            m[1] = vec<R, ScalarT>::from_data(data + R);
        if constexpr (C >= 3)
            m[2] = vec<R, ScalarT>::from_data(data + R * 2);
        if constexpr (C >= 4)
            m[3] = vec<R, ScalarT>::from_data(data + R * 3);
        return m;
    }
    static constexpr mat from_data_colwise(span<ScalarT const> data)
    {
        TG_ASSERT(data.size() == C * R && "wrong amount of data");
        return mat::from_data_colwise(data.data());
    }
    static constexpr mat from_data_colwise(std::initializer_list<ScalarT> data)
    {
        TG_ASSERT(data.size() == C * R && "wrong amount of data");
        return mat::from_data_colwise(data.begin());
    }

    template <class... Args>
    static constexpr mat from_cols(Args const&... args)
    {
        return from_cols_(args...);
    }
    template <class... Args>
    static constexpr mat from_rows(Args const&... args)
    {
        return from_rows_(args...);
    }

    // explicit from_rows and from_cols to make it work properly with {...}
    template <int R2 = R, class = enable_if<R2 == 1>>
    static constexpr mat from_rows(vec<C, ScalarT> const& row0)
    {
        return from_rows_(row0);
    }
    template <int R2 = R, class = enable_if<R2 == 2>>
    static constexpr mat from_rows(vec<C, ScalarT> const& row0, vec<C, ScalarT> const& row1)
    {
        return from_rows_(row0, row1);
    }
    template <int R2 = R, class = enable_if<R2 == 3>>
    static constexpr mat from_rows(vec<C, ScalarT> const& row0, vec<C, ScalarT> const& row1, vec<C, ScalarT> const& row2)
    {
        return from_rows_(row0, row1, row2);
    }
    template <int R2 = R, class = enable_if<R2 == 4>>
    static constexpr mat from_rows(vec<C, ScalarT> const& row0, vec<C, ScalarT> const& row1, vec<C, ScalarT> const& row2, vec<C, ScalarT> const& row3)
    {
        return from_rows_(row0, row1, row2, row3);
    }

    template <int C2 = C, class = enable_if<C2 == 1>>
    static constexpr mat from_cols(vec<R, ScalarT> const& col0)
    {
        return from_cols_(col0);
    }
    template <int C2 = C, class = enable_if<C2 == 2>>
    static constexpr mat from_cols(vec<R, ScalarT> const& col0, vec<R, ScalarT> const& col1)
    {
        return from_cols_(col0, col1);
    }
    template <int C2 = C, class = enable_if<C2 == 3>>
    static constexpr mat from_cols(vec<R, ScalarT> const& col0, vec<R, ScalarT> const& col1, vec<R, ScalarT> const& col2)
    {
        return from_cols_(col0, col1, col2);
    }
    template <int C2 = C, class = enable_if<C2 == 4>>
    static constexpr mat from_cols(vec<R, ScalarT> const& col0, vec<R, ScalarT> const& col1, vec<R, ScalarT> const& col2, vec<R, ScalarT> const& col3)
    {
        return from_cols_(col0, col1, col2, col3);
    }

private:
    template <class... Args>
    static constexpr mat from_cols_(Args const&... args)
    {
        static_assert(sizeof...(args) == C, "must provide exactly C args");

        mat<C, R, ScalarT> m;
        auto i = 0;
        ((m[i++] = vec<R, ScalarT>(args)), ...);

        return m;
    }
    template <class... Args>
    static constexpr mat from_rows_(Args const&... args)
    {
        static_assert(sizeof...(args) == R, "must provide exactly R args");

        mat<R, C, ScalarT> m;
        auto i = 0;
        ((m[i++] = vec<C, ScalarT>(args)), ...);

        mat<C, R, ScalarT> r;
        r[0] = m.row(0);
        if constexpr (C >= 2)
            r[1] = m.row(1);
        if constexpr (C >= 3)
            r[2] = m.row(2);
        if constexpr (C >= 4)
            r[3] = m.row(3);

        return r;
    }
};

template <int R, class ScalarT>
constexpr mat<1, R, ScalarT> from_cols(vec<R, ScalarT> const& c0)
{
    return mat<1, R, ScalarT>::from_cols(c0);
}
template <int R, class ScalarT>
constexpr mat<2, R, ScalarT> from_cols(vec<R, ScalarT> const& c0, vec<R, ScalarT> const& c1)
{
    return mat<2, R, ScalarT>::from_cols(c0, c1);
}
template <int R, class ScalarT>
constexpr mat<3, R, ScalarT> from_cols(vec<R, ScalarT> const& c0, vec<R, ScalarT> const& c1, vec<R, ScalarT> const& c2)
{
    return mat<3, R, ScalarT>::from_cols(c0, c1, c2);
}
template <int R, class ScalarT>
constexpr mat<4, R, ScalarT> from_cols(vec<R, ScalarT> const& c0, vec<R, ScalarT> const& c1, vec<R, ScalarT> const& c2, vec<R, ScalarT> const& c3)
{
    return mat<4, R, ScalarT>::from_cols(c0, c1, c2, c3);
}

template <int C, class ScalarT>
constexpr mat<C, 1, ScalarT> from_rows(vec<C, ScalarT> const& r0)
{
    return mat<C, 1, ScalarT>::from_rows(r0);
}
template <int C, class ScalarT>
constexpr mat<C, 2, ScalarT> from_rows(vec<C, ScalarT> const& r0, vec<C, ScalarT> const& r1)
{
    return mat<C, 2, ScalarT>::from_rows(r0, r1);
}
template <int C, class ScalarT>
constexpr mat<C, 3, ScalarT> from_rows(vec<C, ScalarT> const& r0, vec<C, ScalarT> const& r1, vec<C, ScalarT> const& r2)
{
    return mat<C, 3, ScalarT>::from_rows(r0, r1, r2);
}
template <int C, class ScalarT>
constexpr mat<C, 4, ScalarT> from_rows(vec<C, ScalarT> const& r0, vec<C, ScalarT> const& r1, vec<C, ScalarT> const& r2, vec<C, ScalarT> const& r3)
{
    return mat<C, 4, ScalarT>::from_rows(r0, r1, r2, r3);
}

template <int R, class ScalarT>
constexpr bool operator==(mat<1, R, ScalarT> const& a, mat<1, R, ScalarT> const& b)
{
    return a[0] == b[0];
}
template <int R, class ScalarT>
constexpr bool operator==(mat<2, R, ScalarT> const& a, mat<2, R, ScalarT> const& b)
{
    return a[0] == b[0] && a[1] == b[1];
}
template <int R, class ScalarT>
constexpr bool operator==(mat<3, R, ScalarT> const& a, mat<3, R, ScalarT> const& b)
{
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}
template <int R, class ScalarT>
constexpr bool operator==(mat<4, R, ScalarT> const& a, mat<4, R, ScalarT> const& b)
{
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
}
template <int C, int R, class ScalarT>
constexpr bool operator!=(mat<C, R, ScalarT> const& a, mat<C, R, ScalarT> const& b)
{
    return !(a == b);
}

// reflection
template <class I, int C, int R, class ScalarT>
constexpr void introspect(I&& i, mat<C, R, ScalarT>& v)
{
    i(v[0], "col0");
    if constexpr (C >= 2)
        i(v[1], "col1");
    if constexpr (C >= 3)
        i(v[2], "col2");
    if constexpr (C >= 4)
        i(v[3], "col3");
}

} // namespace tg
