#pragma once

#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

#include "cross.hh"
#include "dot.hh"
#include "normalize.hh"

namespace tg
{
template <class ScalarT>
TG_NODISCARD constexpr mat<4, 4, ScalarT> look_at(pos<3, ScalarT> const& eye, dir<3, ScalarT> const& fwd, vec<3, ScalarT> const& ref_up)
{
    auto right = normalize(cross(vec(fwd), ref_up));
    auto up = vec(cross(right, fwd));

    auto m = mat<4, 4, ScalarT>::zero;

    m[0][0] = right.x;
    m[1][0] = right.y;
    m[2][0] = right.z;

    m[0][1] = up.x;
    m[1][1] = up.y;
    m[2][1] = up.z;

    m[0][2] = -fwd.x;
    m[1][2] = -fwd.y;
    m[2][2] = -fwd.z;

    m[3][0] = -dot(right, eye);
    m[3][1] = -dot(up, eye);
    m[3][2] = dot(fwd, eye);

    m[3][3] = ScalarT(1);

    return m;
}
template <class ScalarT>
TG_NODISCARD constexpr mat<4, 4, ScalarT> look_at(pos<3, ScalarT> const& eye, pos<3, ScalarT> const& target, vec<3, ScalarT> const& ref_up)
{
    return look_at(eye, normalize(target - eye), ref_up);
}
template <class ScalarT>
TG_NODISCARD constexpr mat<4, 4, ScalarT> look_at(pos<3, ScalarT> const& eye, vec<3, ScalarT> const& fwd, vec<3, ScalarT> const& ref_up)
{
    return look_at(eye, normalize(fwd), ref_up);
}
}
