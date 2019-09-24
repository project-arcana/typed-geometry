#pragma once

#include <typed-geometry/detail/macros.hh>
#include <typed-geometry/types/mat.hh>
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
TG_NODISCARD constexpr quadric<D, ScalarT> plane_quadric(pos<D, ScalarT> const& p, vec<D, ScalarT> const& n)
{
    auto const d = dot(p, n);
    return quadric<D, ScalarT>::from_coefficients(self_outer_product(n), n * d, d * d);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr quadric<D, ScalarT> probabilistic_plane_quadric(pos<D, ScalarT> const& mean_p,
                                                                       vec<D, ScalarT> const& mean_n,
                                                                       dont_deduce<ScalarT> sigma_p,
                                                                       dont_deduce<ScalarT> sigma_n)
{
    auto const p = vec(mean_p);
    auto const sn2 = sigma_n * sigma_n;
    auto const sp2 = sigma_p * sigma_p;
    auto const d = dot(mean_p, mean_n);

    auto A = self_outer_product(mean_n);
    A[0][0] += sn2;
    A[1][1] += sn2;
    A[2][2] += sn2;

    auto const b = mean_n * d + sn2 * p;
    auto const c = d * d + sn2 * dot(p, p) + sp2 * dot(mean_n, mean_n) + sp2 * sn2;

    return quadric<D, ScalarT>::from_coefficients(A, b, c);
}

template <int D, class ScalarT>
TG_NODISCARD constexpr quadric<D, ScalarT> probabilistic_plane_quadric(pos<D, ScalarT> const& mean_p,
                                                                       vec<D, ScalarT> const& mean_n,
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
}
