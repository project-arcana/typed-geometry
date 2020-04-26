#pragma once

#include <typed-geometry/types/dir.hh>
#include <typed-geometry/types/mat.hh>
#include <typed-geometry/types/pos.hh>
#include <typed-geometry/types/vec.hh>

#include <typed-geometry/functions/vector/cross.hh>
#include <typed-geometry/functions/vector/dot.hh>
#include <typed-geometry/functions/vector/normalize.hh>

namespace tg
{
template <class ScalarT>
[[nodiscard]] constexpr mat<4, 4, ScalarT> look_at_opengl(pos<3, ScalarT> const& eye, dir<3, ScalarT> const& fwd, vec<3, ScalarT> const& ref_up)
{
    auto const right = normalize(cross(vec(fwd), ref_up));
    auto const up = vec(cross(right, fwd));

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
[[nodiscard]] constexpr mat<4, 4, ScalarT> look_at_directx(pos<3, ScalarT> const& eye, dir<3, ScalarT> const& fwd, vec<3, ScalarT> const& ref_up)
{
    auto const right = normalize(cross(ref_up, vec(fwd)));
    auto const up = vec(cross(fwd, right));

    auto m = mat<4, 4, ScalarT>::zero;

    m[0][0] = right.x;
    m[1][0] = right.y;
    m[2][0] = right.z;

    m[0][1] = up.x;
    m[1][1] = up.y;
    m[2][1] = up.z;

    m[0][2] = fwd.x;
    m[1][2] = fwd.y;
    m[2][2] = fwd.z;

    m[3][0] = -dot(right, eye);
    m[3][1] = -dot(up, eye);
    m[3][2] = -dot(fwd, eye);

    m[3][3] = ScalarT(1);

    return m;
}
template <class ScalarT>
[[nodiscard]] constexpr mat<4, 4, ScalarT> look_at_opengl(pos<3, ScalarT> const& eye, pos<3, ScalarT> const& target, vec<3, ScalarT> const& ref_up)
{
    return look_at_opengl(eye, normalize(target - eye), ref_up);
}
template <class ScalarT>
[[nodiscard]] constexpr mat<4, 4, ScalarT> look_at_opengl(pos<3, ScalarT> const& eye, vec<3, ScalarT> const& fwd, vec<3, ScalarT> const& ref_up)
{
    return look_at_opengl(eye, normalize(fwd), ref_up);
}
template <class ScalarT>
[[nodiscard]] constexpr mat<4, 4, ScalarT> look_at_directx(pos<3, ScalarT> const& eye, pos<3, ScalarT> const& target, vec<3, ScalarT> const& ref_up)
{
    return look_at_directx(eye, normalize(target - eye), ref_up);
}
template <class ScalarT>
[[nodiscard]] constexpr mat<4, 4, ScalarT> look_at_directx(pos<3, ScalarT> const& eye, vec<3, ScalarT> const& fwd, vec<3, ScalarT> const& ref_up)
{
    return look_at_directx(eye, normalize(fwd), ref_up);
}

template <class ScalarT>
[[deprecated("use explicit _opengl or _directx version")]] [[nodiscard]] constexpr mat<4, 4, ScalarT> look_at(pos<3, ScalarT> const& eye,
                                                                                                              dir<3, ScalarT> const& fwd,
                                                                                                              vec<3, ScalarT> const& ref_up)
{
    return look_at_opengl(eye, fwd, ref_up);
}
template <class ScalarT>
[[deprecated("use explicit _opengl or _directx version")]] [[nodiscard]] constexpr mat<4, 4, ScalarT> look_at(pos<3, ScalarT> const& eye,
                                                                                                              pos<3, ScalarT> const& target,
                                                                                                              vec<3, ScalarT> const& ref_up)
{
    return look_at_opengl(eye, normalize(target - eye), ref_up);
}
template <class ScalarT>
[[deprecated("use explicit _opengl or _directx version")]] [[nodiscard]] constexpr mat<4, 4, ScalarT> look_at(pos<3, ScalarT> const& eye,
                                                                                                              vec<3, ScalarT> const& fwd,
                                                                                                              vec<3, ScalarT> const& ref_up)
{
    return look_at_opengl(eye, normalize(fwd), ref_up);
}
}
