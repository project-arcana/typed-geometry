#pragma once

#include <typed-geometry/detail/macros.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/objects/triangle.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/quadric.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/functions/matrix/outer_product.hh>
#include <typed-geometry/functions/matrix/trace.hh>
#include <typed-geometry/functions/vector/cross.hh>
#include <typed-geometry/functions/vector/dot.hh>

namespace tg
{
template <int D, class ScalarT>
[[nodiscard]] constexpr quadric<D, ScalarT> point_quadric(pos<D, ScalarT> const& p)
{
    auto const v = tg::vec<D, ScalarT>(p);
    return quadric<D, ScalarT>::from_coefficients(mat<D, D, ScalarT>::identity, v, dot(v, v));
}

template <int D, class ScalarT>
[[nodiscard]] constexpr quadric<D, ScalarT> plane_quadric(pos<D, ScalarT> const& p, vec_or_dir<D, ScalarT> const& n)
{
    auto const d = dot(p, n);
    return quadric<D, ScalarT>::from_coefficients(self_outer_product(n), n * d, d * d);
}

template <int D, class ScalarT>
[[nodiscard]] constexpr quadric<D, ScalarT> probabilistic_plane_quadric(pos<D, ScalarT> const& mean_p,
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
[[nodiscard]] constexpr quadric<D, ScalarT> probabilistic_plane_quadric(pos<D, ScalarT> const& mean_p,
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
[[nodiscard]] constexpr quadric<D, ScalarT> triangle_quadric(pos<D, ScalarT> const& p, pos<D, ScalarT> const& q, pos<D, ScalarT> const& r)
{
    auto const pxq = cross(vec(p), vec(q));
    auto const qxr = cross(vec(q), vec(r));
    auto const rxp = cross(vec(r), vec(p));

    auto const xsum = pxq + qxr + rxp;
    auto const det = dot(pxq, r);

    return quadric<D, ScalarT>::from_coefficients(self_outer_product(xsum), xsum * det, det * det);
}
template <int D, class ScalarT>
[[nodiscard]] constexpr quadric<D, ScalarT> triangle_quadric(triangle<D, ScalarT> const& t)
{
    return triangle_quadric(t.pos0, t.pos1, t.pos2);
}

namespace detail
{
template <class ScalarT>
mat<3, 3, ScalarT> cross_product_squared_transpose(vec<3, ScalarT> const& v)
{
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
}
template <class ScalarT>
mat<3, 3, ScalarT> cross_interference_matrix(mat<3, 3, ScalarT> const& A, mat<3, 3, ScalarT> const& B)
{
    mat<3, 3, ScalarT> m;

    auto constexpr x = 0;
    auto constexpr y = 1;
    auto constexpr z = 2;

    auto const cxx = A[y][z] * B[y][z];
    auto const cyy = A[x][z] * B[x][z];
    auto const czz = A[x][y] * B[x][y];

    m[x][x] = A[y][y] * B[z][z] - cxx - cxx + A[z][z] * B[y][y];
    m[y][y] = A[x][x] * B[z][z] - cyy - cyy + A[z][z] * B[x][x];
    m[z][z] = A[x][x] * B[y][y] - czz - czz + A[y][y] * B[x][x];

    m[x][y] = -A[x][y] * B[z][z] + A[x][z] * B[y][z] + A[y][z] * B[x][z] - A[z][z] * B[x][y];
    m[x][z] = A[x][y] * B[y][z] - A[x][z] * B[y][y] - A[y][y] * B[x][z] + A[y][z] * B[x][y];
    m[y][z] = -A[x][x] * B[y][z] + A[x][y] * B[x][z] + A[x][z] * B[x][y] - A[y][z] * B[x][x];

    m[y][x] = m[x][y];
    m[z][x] = m[x][z];
    m[z][y] = m[y][z];

    return m;
}

template <class T>
mat<3, 3, T> first_order_tri_quad(vec<3, T> const& a, mat<3, 3, T> const& sigma)
{
    mat<3, 3, T> M;

    auto const xx = a.x * a.x;
    auto const xy = a.x * a.y;
    auto const xz = a.x * a.z;
    auto const yy = a.y * a.y;
    auto const yz = a.y * a.z;
    auto const zz = a.z * a.z;

    M[0][0] = -sigma[1][1] * zz + 2 * sigma[1][2] * yz - sigma[2][2] * yy;
    M[0][1] = sigma[0][1] * zz - sigma[0][2] * yz - sigma[1][2] * xz + sigma[2][2] * xy;
    M[0][2] = -sigma[0][1] * yz + sigma[0][2] * yy + sigma[1][1] * xz - sigma[1][2] * xy;
    M[1][1] = -sigma[0][0] * zz + 2 * sigma[0][2] * xz - sigma[2][2] * xx;
    M[1][2] = sigma[0][0] * yz - sigma[0][1] * xz - sigma[0][2] * xy + sigma[1][2] * xx;
    M[2][2] = -sigma[0][0] * yy + 2 * sigma[0][1] * xy - sigma[1][1] * xx;

    M[1][0] = M[0][1];
    M[2][0] = M[0][2];
    M[2][1] = M[1][2];

    return M;
}
}

template <int D, class ScalarT>
[[nodiscard]] constexpr quadric<D, ScalarT> probabilistic_triangle_quadric(pos<D, ScalarT> const& mean_p,
                                                                           pos<D, ScalarT> const& mean_q,
                                                                           pos<D, ScalarT> const& mean_r,
                                                                           dont_deduce<ScalarT> stddev)
{
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

    A += (tg::detail::cross_product_squared_transpose(pmq) + //
          tg::detail::cross_product_squared_transpose(qmr) + //
          tg::detail::cross_product_squared_transpose(rmp))
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
[[nodiscard]] constexpr quadric<D, ScalarT> probabilistic_triangle_quadric(triangle<D, ScalarT> const& t, dont_deduce<ScalarT> stddev)
{
    return probabilistic_triangle_quadric(t.pos0, t.pos1, t.pos2, stddev);
}

template <class ScalarT>
[[nodiscard]] constexpr quadric<3, ScalarT> probabilistic_triangle_quadric(pos<3, ScalarT> const& mean_p,
                                                                           pos<3, ScalarT> const& mean_q,
                                                                           pos<3, ScalarT> const& mean_r,
                                                                           mat<3, 3, ScalarT> const& sigma_p,
                                                                           mat<3, 3, ScalarT> const& sigma_q,
                                                                           mat<3, 3, ScalarT> const& sigma_r)
{
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

    auto const ci_pq = tg::detail::cross_interference_matrix(sigma_p, sigma_q);
    auto const ci_qr = tg::detail::cross_interference_matrix(sigma_q, sigma_r);
    auto const ci_rp = tg::detail::cross_interference_matrix(sigma_r, sigma_p);

    mat<3, 3, ScalarT> A;

    A += self_outer_product(cross_pqr);

    A -= tg::detail::first_order_tri_quad(pmq, sigma_r);
    A -= tg::detail::first_order_tri_quad(qmr, sigma_p);
    A -= tg::detail::first_order_tri_quad(rmp, sigma_q);

    A += ci_pq + ci_qr + ci_rp;

    A[1][0] = A[0][1];
    A[2][0] = A[0][2];
    A[2][1] = A[1][2];

    vec<3, ScalarT> b;

    b += det_pqr * cross_pqr;

    b -= cross(pmq, sigma_r * pxq);
    b -= cross(qmr, sigma_p * qxr);
    b -= cross(rmp, sigma_q * rxp);

    b += ci_pq * r;
    b += ci_qr * p;
    b += ci_rp * q;

    ScalarT c = det_pqr * det_pqr;

    c += dot(pxq, sigma_r * pxq);
    c += dot(qxr, sigma_p * qxr);
    c += dot(rxp, sigma_q * rxp);

    c += dot(p, ci_qr * p);
    c += dot(q, ci_rp * q);
    c += dot(r, ci_pq * r);

    c += trace_of_product(sigma_r, ci_pq);

    return quadric<3, ScalarT>::from_coefficients(A, b, c);
}
}
