#pragma once

#include <typed-geometry/functions/basic/statistics.hh>
#include <typed-geometry/functions/matrix/outer_product.hh>
#include <typed-geometry/types/mat.hh>

namespace tg
{
template <class PosRangeT, class Transform = identity_fun>
auto covariance_matrix(PosRangeT&& r, Transform&& t = {}) -> decltype(self_outer_product(t(*tg::begin(r)) - t(*tg::begin(r))))
{
    using MatT = decltype(self_outer_product(t(*tg::begin(r)) - t(*tg::begin(r))));
    auto const avg = average(r, t);
    MatT cov;
    for (auto const& e : r)
        cov += self_outer_product(t(e) - avg);
    return cov;
}
}
