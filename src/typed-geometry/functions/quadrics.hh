#pragma once

#include <typed-geometry/detail/macros.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/objects/triangle.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/quadric.hh>
#include <typed-geometry/types/vec.hh>

#include "cross.hh"
#include "dot.hh"
#include "outer_product.hh"
#include "trace.hh"

namespace tg
{
template <int D, class ScalarT>
TG_NODISCARD constexpr quadric<D, ScalarT> point_quadric(pos<D, ScalarT> const& p)
{
    auto const v = tg::vec<D, ScalarT>(p);
    return quadric<D, ScalarT>::from_coefficients(mat<D, D, ScalarT>::identity, v, dot(v, v));
}

template <int D, class ScalarT>
TG_NODISCARD constexpr quadric<D, ScalarT> plane_quadric(pos<D, ScalarT> const& p, vec_or_dir<D, ScalarT> const& n)
{
    auto const d = dot(p, n);
    return quadric<D, ScalarT>::from_coefficients(self_outer_product(n), n * d, d * d);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr quadric<D, ScalarT> probabilistic_plane_quadric(pos<D, ScalarT> const& mean_p,
                                                                       vec_or_dir<D, ScalarT> const& mean_n,
                                                                       dont_deduce<ScalarT> stddev_p,
                                                                       dont_deduce<ScalarT> stddev_n)
{
    auto const p = vec(mean_p);
    auto const sn2 = stddev_n * stddev_n;
    auto const sp2 = stddev_p * stddev_p;
    auto const d = dot(mean_p, mean_n);

    auto A = self_outer_product(mean_n);
    A[0][0] += sn2;
    A[1][1] += sn2;
    A[2][2] += sn2;

    auto const b = mean_n * d + sn2 * p;
    auto const c = d * d + sn2 * dot(p, p) + sp2 * dot(mean_n, mean_n) + 3 * sp2 * sn2;

    return quadric<D, ScalarT>::from_coefficients(A, b, c);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr quadric<D, ScalarT> probabilistic_plane_quadric(pos<D, ScalarT> const& mean_p,
                                                                       vec_or_dir<D, ScalarT> const& mean_n,
                                                                       mat<D, D, ScalarT> const& sigma_p,
                                                                       mat<D, D, ScalarT> const& sigma_n)
{
    auto const p = vec(mean_p);
    auto const d = dot(mean_p, mean_n);

    auto const A = self_outer_product(mean_n) + sigma_n;
    auto const b = mean_n * d + sigma_n * p;
    auto const c = d * d + dot(p, sigma_n * p) + dot(mean_n, sigma_p * mean_n) + trace_of_product(sigma_n, sigma_p);

    return quadric<D, ScalarT>::from_coefficients(A, b, c);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr quadric<D, ScalarT> triangle_quadric(pos<D, ScalarT> const& p, pos<D, ScalarT> const& q, pos<D, ScalarT> const& r)
{
    auto const pxq = cross(vec(p), vec(q));
    auto const qxr = cross(vec(q), vec(r));
    auto const rxp = cross(vec(r), vec(p));

    auto const xsum = pxq + qxr + rxp;
    auto const det = dot(pxq, r);

    return quadric<D, ScalarT>::from_coefficients(self_outer_product(xsum), xsum * det, det * det);
}
template <int D, class ScalarT>
TG_NODISCARD constexpr quadric<D, ScalarT> triangle_quadric(triangle<D, ScalarT> const& t)
{
    return triangle_quadric(t.pos0, t.pos1, t.pos2);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr quadric<D, ScalarT> probabilistic_triangle_quadric(pos<D, ScalarT> const& mean_p,
                                                                          pos<D, ScalarT> const& mean_q,
                                                                          pos<D, ScalarT> const& mean_r,
                                                                          dont_deduce<ScalarT> stddev)
{
    auto const cross_product_squared_transpose = [](vec<3, ScalarT> const& v) {
        auto const a = v.x;
        auto const b = v.y;
        auto const c = v.z;
        auto const a2 = a * a;
        auto const b2 = b * b;
        auto const c2 = c * c;

        mat<3, 3, ScalarT> M;

        M[0][0] = b2 + c2;
        M[1][1] = a2 + c2;
        M[2][2] = a2 + b2;

        M[1][0] = M[0][1] = -a * b;
        M[2][0] = M[0][2] = -a * c;
        M[2][1] = M[1][2] = -b * c;

        return M;
    };

    auto const sigma = stddev * stddev;
    auto const p = tg::vec(mean_p);
    auto const q = tg::vec(mean_q);
    auto const r = tg::vec(mean_r);

    auto const pxq = cross(p, q);
    auto const qxr = cross(q, r);
    auto const rxp = cross(r, p);

    auto const det_pqr = dot(pxq, r);

    auto const cross_pqr = pxq + qxr + rxp;

    auto const pmq = p - q;
    auto const qmr = q - r;
    auto const rmp = r - p;

    mat<3, 3, ScalarT> A;

    A += self_outer_product(cross_pqr);

    A += (cross_product_squared_transpose(pmq) + //
          cross_product_squared_transpose(qmr) + //
          cross_product_squared_transpose(rmp))
         * sigma;

    auto ss = sigma * sigma;
    auto ss6 = 6 * ss;
    auto ss2 = 2 * ss;
    A[0][0] += ss6;
    A[1][1] += ss6;
    A[2][2] += ss6;

    tg::vec<3, ScalarT> b;

    b += det_pqr * cross_pqr;

    b -= sigma * (cross(pmq, pxq) + cross(qmr, qxr) + cross(rmp, rxp));

    b += ss2 * (p + q + r);

    ScalarT c = det_pqr * det_pqr;

    c += sigma * (dot(pxq, pxq) + dot(qxr, qxr) + dot(rxp, rxp)); // 3x (a x b)^T M_c (a x b)

    c += ss2 * (dot(p, p) + dot(q, q) + dot(r, r)); // 3x a^T Ci[S_b, S_c] a

    c += ss6 * sigma; // Tr[S_r Ci[S_p, S_q]]

    return quadric<D, ScalarT>::from_coefficients(A, b, c);
}
template <int D, class ScalarT>
TG_NODISCARD constexpr quadric<D, ScalarT> probabilistic_triangle_quadric(triangle<D, ScalarT> const& t, dont_deduce<ScalarT> stddev)
{
    return probabilistic_triangle_quadric(t.pos0, t.pos1, t.pos2, stddev);
}
}
