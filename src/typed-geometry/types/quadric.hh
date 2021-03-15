#pragma once

#include "dir.hh"
#include "mat.hh"
#include "pos.hh"
#include "vec.hh"

namespace tg
{
/**
 * An error quadric in 2D or 3D
 *
 * Notable functions:
 *   - distance(pos, quadric)   for residual L2 error
 *   - distance_sqr(pos, quadric)  for residual squared L2 error
 *   - closest_point(quadric)   for the position minimizing the distance
 *   - operator + * /
 */
template <int D, class ScalarT>
struct quadric;

// Common quadric types
using quadric2 = quadric<2, f32>;
using fquadric2 = quadric<2, f32>;
using dquadric2 = quadric<2, f64>;

using quadric3 = quadric<3, f32>;
using fquadric3 = quadric<3, f32>;
using dquadric3 = quadric<3, f64>;


// ======== IMPLEMENTATION ========

template <class ScalarT>
struct quadric<2, ScalarT>
{
    using scalar_t = ScalarT;
    using vec_t = tg::vec<2, ScalarT>;
    using pos_t = tg::pos<2, ScalarT>;
    using mat_t = tg::mat<2, 2, ScalarT>;

    // x^T A x - 2 b^T x + c
public:
    scalar_t A00 = scalar_t(0);
    scalar_t A01 = scalar_t(0);
    scalar_t A11 = scalar_t(0);

    scalar_t b0 = scalar_t(0);
    scalar_t b1 = scalar_t(0);

    scalar_t c = scalar_t(0);

public:
    constexpr quadric() = default;

    static constexpr quadric from_coefficients(mat_t const& A, vec_t const& b, scalar_t c)
    {
        quadric q;
        q.A00 = A[0][0];
        q.A01 = A[0][1];
        q.A11 = A[1][1];
        q.b0 = b[0];
        q.b1 = b[1];
        q.c = c;
        return q;
    }

    // TODO: constexpr explicit quadric(mat<3, 3, ScalarT> const& m);
    // TODO: constexpr explicit operator mat<3, 3, ScalarT>() const;

    // TODO: operator== and !=

    // TODO: find better place for these?
public:
    /// Adds a line (pos, normal) with a given uncertainty sigma
    /// `normal` should be normalized
    /// `sigma` is the standard deviation of the normal distribution added to `normal`
    void add_line(pos_t pos, vec_t normal, scalar_t sigma)
    {
        auto d = dot(pos - pos_t::zero, normal);
        auto s2 = sigma * sigma;

        A00 += normal.x * normal.x + s2;
        A01 += normal.x * normal.y;
        A11 += normal.y * normal.y + s2;

        b0 += normal.x * d + pos.x * s2;
        b1 += normal.y * d + pos.y * s2;

        // TODO: maybe positional uncertainty
        c += d * d + distance_sqr_to_origin(pos) * s2;
    }

    // add two quadrics
    void add(quadric const& rhs)
    {
        A00 += rhs.A00;
        A01 += rhs.A01;
        A11 += rhs.A11;

        b0 += rhs.b0;
        b1 += rhs.b1;

        c += rhs.c;
    }

    // Residual L2 error as given by x^T A x - 2 r^T x + c
    [[nodiscard]] constexpr ScalarT operator()(pos<2, ScalarT> const& p) const
    {
        vec<2, ScalarT> Ax = {
            A00 * p.x + A01 * p.y, //
            A01 * p.x + A11 * p.y, //
        };

        return dot(vec<2, ScalarT>(p), Ax) // x^T A x
               - 2 * (b0 * p.x + b1 * p.y) // - 2 r^T x
               + c;                        // + c
    }

    // TODO: fix me
    template <class T>
    friend constexpr T distance_sqr(pos<2, T> const& p, quadric<2, T> const& q);
    template <class T>
    friend constexpr pos<2, T> closest_point(quadric<2, T> const& q);
};

template <class ScalarT>
struct quadric<3, ScalarT>
{
    using scalar_t = ScalarT;
    using vec_t = tg::vec<3, ScalarT>;
    using pos_t = tg::pos<3, ScalarT>;
    using mat_t = tg::mat<3, 3, ScalarT>;
    using mat4_t = tg::mat<4, 4, ScalarT>;

    // x^T A x - 2 b^T x + c
public:
    scalar_t A00 = scalar_t(0);
    scalar_t A01 = scalar_t(0);
    scalar_t A02 = scalar_t(0);
    scalar_t A11 = scalar_t(0);
    scalar_t A12 = scalar_t(0);
    scalar_t A22 = scalar_t(0);

    scalar_t b0 = scalar_t(0);
    scalar_t b1 = scalar_t(0);
    scalar_t b2 = scalar_t(0);

    scalar_t c = scalar_t(0);

public:
    constexpr quadric() = default;
    // TODO: constexpr explicit quadric(mat<4, 4, ScalarT> const& m);
    // TODO: constexpr explicit operator mat<4, 4, ScalarT>() const;

    // TODO: operator== and !=

    static constexpr quadric from_coefficients(scalar_t A00, scalar_t A01, scalar_t A02, scalar_t A11, scalar_t A12, scalar_t A22, scalar_t b0, scalar_t b1, scalar_t b2, scalar_t c)
    {
        quadric q;
        q.A00 = A00;
        q.A01 = A01;
        q.A02 = A02;
        q.A11 = A11;
        q.A12 = A12;
        q.A22 = A22;
        q.b0 = b0;
        q.b1 = b1;
        q.b2 = b2;
        q.c = c;
        return q;
    }
    static constexpr quadric from_coefficients(mat_t const& A, vec_t const& b, scalar_t c)
    {
        quadric q;
        q.A00 = A[0][0];
        q.A01 = A[0][1];
        q.A02 = A[0][2];
        q.A11 = A[1][1];
        q.A12 = A[1][2];
        q.A22 = A[2][2];
        q.b0 = b[0];
        q.b1 = b[1];
        q.b2 = b[2];
        q.c = c;
        return q;
    }
    static constexpr quadric from_coefficients(mat4_t const& Q)
    {
        quadric q;
        q.A00 = Q[0][0];
        q.A01 = Q[0][1];
        q.A02 = Q[0][2];
        q.A11 = Q[1][1];
        q.A12 = Q[1][2];
        q.A22 = Q[2][2];
        q.b0 = -Q[3][0];
        q.b1 = -Q[3][1];
        q.b2 = -Q[3][2];
        q.c = Q[3][3];
        return q;
    }

public:
    // x^T Q x
    constexpr mat<4, 4, ScalarT> const Q() const
    {
        mat<4, 4, ScalarT> m;
        m[0][0] = A00;
        m[0][1] = A01;
        m[0][2] = A02;
        m[1][0] = A01;
        m[1][1] = A11;
        m[1][2] = A12;
        m[2][0] = A02;
        m[2][1] = A12;
        m[2][2] = A22;
        m[3][0] = -b0;
        m[3][1] = -b1;
        m[3][2] = -b2;
        m[0][3] = -b0;
        m[1][3] = -b1;
        m[2][3] = -b2;
        m[3][3] = c;
        return m;
    }
    constexpr mat<3, 3, ScalarT> const A() const
    {
        mat<3, 3, ScalarT> m;
        m[0][0] = A00;
        m[0][1] = A01;
        m[0][2] = A02;
        m[1][0] = A01;
        m[1][1] = A11;
        m[1][2] = A12;
        m[2][0] = A02;
        m[2][1] = A12;
        m[2][2] = A22;
        return m;
    }
    constexpr vec<3, ScalarT> const b() const { return {b0, b1, b2}; }

    // TODO: find better place for these?
public:
    /// Adds a plane (pos, normal) with a given uncertainty sigma
    /// `normal` should be normalized
    /// `sigma` is the standard deviation of the normal distribution added to `normal`
    void add_plane(pos_t pos, vec_t normal, scalar_t sigma)
    {
        auto d = dot(pos - pos_t::zero, normal);
        auto s2 = sigma * sigma;

        A00 += normal.x * normal.x + s2;
        A01 += normal.x * normal.y;
        A02 += normal.x * normal.z;
        A11 += normal.y * normal.y + s2;
        A12 += normal.y * normal.z;
        A22 += normal.z * normal.z + s2;

        b0 += normal.x * d + pos.x * s2;
        b1 += normal.y * d + pos.y * s2;
        b2 += normal.z * d + pos.z * s2;

        // TODO: maybe positional uncertainty
        c += d * d + distance_sqr_to_origin(pos) * s2;
    }

    /// Adds two quadrics
    void add(quadric const& rhs)
    {
        A00 += rhs.A00;
        A01 += rhs.A01;
        A02 += rhs.A02;
        A11 += rhs.A11;
        A12 += rhs.A12;
        A22 += rhs.A22;

        b0 += rhs.b0;
        b1 += rhs.b1;
        b2 += rhs.b2;

        c += rhs.c;
    }

    // adds a quadric given in matrix form (x^T A x + b^T x + c)
    // optionally with a c value
    void add(mat<3, 3, scalar_t> const& A, vec<3, scalar_t> const& b, scalar_t c = {})
    {
        A00 += A[0][0];
        A01 += A[0][1];
        A02 += A[0][2];
        A11 += A[1][1];
        A12 += A[1][2];
        A22 += A[2][2];

        b0 += b.x;
        b1 += b.y;
        b2 += b.z;

        c += c;
    }

    // Residual L2 error as given by x^T A x - 2 r^T x + c
    [[nodiscard]] constexpr ScalarT operator()(pos<3, ScalarT> const& p) const
    {
        vec<3, ScalarT> Ax = {
            A00 * p.x + A01 * p.y + A02 * p.z, //
            A01 * p.x + A11 * p.y + A12 * p.z, //
            A02 * p.x + A12 * p.y + A22 * p.z, //
        };

        return dot(vec<3, ScalarT>(p), Ax)            // x^T A x
               - 2 * (p.x * b0 + p.y * b1 + p.z * b2) // - 2 r^T x
               + c;                                   // + c
    }

    // TODO: fix me
    template <class T>
    friend constexpr T distance_sqr(pos<3, T> const& p, quadric<3, T> const& q);
    template <class T>
    friend constexpr pos<3, T> closest_point(quadric<3, T> const& q);
};

// reflection
template <class I, int D, class ScalarT>
constexpr void introspect(I&& i, quadric<D, ScalarT>& v)
{
    if constexpr (D == 2)
    {
        i(v.A00, "A00");
        i(v.A01, "A01");
        i(v.A11, "A11");
        i(v.b0, "b0");
        i(v.b1, "b1");
        i(v.c, "c");
    }
    else if constexpr (D == 3)
    {
        i(v.A00, "A00");
        i(v.A01, "A01");
        i(v.A02, "A02");
        i(v.A11, "A11");
        i(v.A12, "A12");
        i(v.A22, "A22");
        i(v.b0, "b0");
        i(v.b1, "b1");
        i(v.b2, "b2");
        i(v.c, "c");
    }
    else
        static_assert(always_false<D>, "quadrics are only defined for 2D and 3D");
}
}
