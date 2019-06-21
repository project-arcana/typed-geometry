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
    using vec2_t = tg::vec<2, ScalarT>;
    using pos2_t = tg::pos<2, ScalarT>;
    using dir2_t = tg::dir<2, ScalarT>;

private:
    scalar_t A00 = 0;
    scalar_t A01 = 0;
    scalar_t A11 = 0;

    vec2_t r = vec2_t::zero;

    scalar_t d_sqr = 0;

public:
    constexpr quadric() = default;

    // TODO: constexpr explicit quadric(mat<3, 3, ScalarT> const& m);
    // TODO: constexpr explicit operator mat<3, 3, ScalarT>() const;

    // TODO: operator== and !=

    // TODO: find better place for these?
public:
    /// Adds a line (pos, normal) with a given uncertainty sigma
    /// `normal` should be normalized
    /// `sigma` is the standard deviation of the normal distribution added to `normal`
    void add_line(pos2_t pos, dir2_t normal, scalar_t sigma)
    {
        auto d = dot(pos - pos2_t::zero, normal);
        auto s2 = sigma * sigma;

        A00 += normal.x * normal.x + s2;
        A01 += normal.x * normal.y;
        A11 += normal.y * normal.y + s2;

        r += normal * d + vec2_t(pos) * s2;

        // TODO: maybe positional uncertainty
        d_sqr += d * d + distance_sqr_to_origin(pos) * s2;
    }

    // TODO: fix me
    template<class T>
    friend constexpr T distance_sqr(pos<2, T> const& p, quadric<2, T> const& q);
    template<class T>
    friend constexpr pos<2, T> closest_point(quadric<2, T> const& q);
};

template <class ScalarT>
struct quadric<3, ScalarT>
{
    using scalar_t = ScalarT;
    using vec3_t = tg::vec<3, ScalarT>;
    using pos3_t = tg::pos<3, ScalarT>;
    using dir3_t = tg::dir<3, ScalarT>;

private:
    scalar_t A00 = 0;
    scalar_t A01 = 0;
    scalar_t A02 = 0;
    scalar_t A11 = 0;
    scalar_t A12 = 0;
    scalar_t A22 = 0;

    vec3_t r = vec3_t::zero;

    scalar_t d_sqr = 0;

public:
    constexpr quadric() = default;
    // TODO: constexpr explicit quadric(mat<4, 4, ScalarT> const& m);
    // TODO: constexpr explicit operator mat<4, 4, ScalarT>() const;

    // TODO: operator== and !=

    // TODO: find better place for these?
public:
    /// Adds a plane (pos, normal) with a given uncertainty sigma
    /// `normal` should be normalized
    /// `sigma` is the standard deviation of the normal distribution added to `normal`
    void add_plane(pos3_t pos, dir3_t normal, scalar_t sigma)
    {
        auto d = dot(pos - pos3_t::zero, normal);
        auto s2 = sigma * sigma;

        A00 += normal.x * normal.x + s2;
        A01 += normal.x * normal.y;
        A02 += normal.x * normal.z;
        A11 += normal.y * normal.y + s2;
        A12 += normal.y * normal.z;
        A22 += normal.z * normal.z + s2;

        r += normal * d + vec3_t(pos) * s2;

        // TODO: maybe positional uncertainty
        d_sqr += d * d + distance_sqr_to_origin(pos) * s2;
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

        r += rhs.r;

        d_sqr += rhs.d_sqr;
    }

    // TODO: fix me
    template<class T>
    friend constexpr T distance_sqr(pos<3, T> const& p, quadric<3, T> const& q);
    template<class T>
    friend constexpr pos<3, T> closest_point(quadric<3, T> const& q);
};

}
