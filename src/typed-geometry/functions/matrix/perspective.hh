#pragma once

#include <typed-geometry/types/angle.hh>
#include <typed-geometry/types/mat.hh>

#include <typed-geometry/detail/operators/ops_angle.hh>
#include <typed-geometry/feature/assert.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>

namespace tg::detail
{
template <class ScalarT>
ScalarT x_scale_from_horizontal_fov(horizontal_fov_t<ScalarT> horizontal_fov)
{
    TG_CONTRACT(horizontal_fov.angle > degree(0));
    TG_CONTRACT(horizontal_fov.angle < degree(180));
    return ScalarT(1) / tg::tan(horizontal_fov.angle / ScalarT(2));
}
}

namespace tg
{
/// aspect ratio is width / height
template <class ScalarT>
[[nodiscard]] mat<4, 4, ScalarT> perspective_reverse_z_opengl(horizontal_fov_t<ScalarT> horizontal_fov, ScalarT aspect_ratio, ScalarT near_plane)
{
    TG_CONTRACT(near_plane > 0);
    TG_CONTRACT(aspect_ratio > 0);

    // adapted from glm for reverse Z and infinite far
    // see https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/

    auto m = mat<4, 4, ScalarT>::zero;

    auto const x_scale = tg::detail::x_scale_from_horizontal_fov(horizontal_fov);
    m[0][0] = x_scale;
    m[1][1] = x_scale * aspect_ratio;
    m[2][3] = -ScalarT(1);
    m[3][2] = near_plane;

    return m;
}

template <class ScalarT>
[[nodiscard]] mat<4, 4, ScalarT> perspective_reverse_z_directx(horizontal_fov_t<ScalarT> horizontal_fov, ScalarT aspect_ratio, ScalarT near_plane)
{
    TG_CONTRACT(near_plane > 0);
    TG_CONTRACT(aspect_ratio > 0);

    auto m = mat<4, 4, ScalarT>::zero;

    auto const x_scale = tg::detail::x_scale_from_horizontal_fov(horizontal_fov);
    m[0][0] = x_scale;
    m[1][1] = x_scale * aspect_ratio;
    m[2][3] = ScalarT(1);
    m[3][2] = near_plane;

    return m;
}

template <class ScalarT>
[[nodiscard]] mat<4, 4, ScalarT> perspective_directx(horizontal_fov_t<ScalarT> horizontal_fov, ScalarT aspect_ratio, ScalarT near_plane, ScalarT far_plane)
{
    TG_CONTRACT(near_plane > 0);
    TG_CONTRACT(far_plane > 0);
    TG_CONTRACT(aspect_ratio > 0);

    auto m = mat<4, 4, ScalarT>::zero;

    auto const x_scale = tg::detail::x_scale_from_horizontal_fov(horizontal_fov);
    m[0][0] = x_scale;
    m[1][1] = x_scale * aspect_ratio;
    m[2][2] = far_plane / (far_plane - near_plane);
    m[3][2] = -near_plane * far_plane / (far_plane - near_plane);
    m[2][3] = ScalarT(1);

    return m;
}

template <class ScalarT>
[[nodiscard]] mat<4, 4, ScalarT> perspective_opengl(horizontal_fov_t<ScalarT> horizontal_fov, ScalarT aspect_ratio, ScalarT near_plane, ScalarT far_plane)
{
    TG_CONTRACT(near_plane > 0);
    TG_CONTRACT(far_plane > 0);
    TG_CONTRACT(aspect_ratio > 0);

    auto m = mat<4, 4, ScalarT>::zero;

    auto const x_scale = tg::detail::x_scale_from_horizontal_fov(horizontal_fov);
    m[0][0] = x_scale;
    m[1][1] = x_scale * aspect_ratio;
    m[2][2] = -(far_plane + near_plane) / (far_plane - near_plane);
    m[3][2] = ScalarT(-2) * near_plane * far_plane / (far_plane - near_plane);
    m[2][3] = ScalarT(-1);

    return m;
}

template <class ScalarT>
[[nodiscard]] ScalarT reverse_z_to_linear_depth(ScalarT d, ScalarT near_plane)
{
    TG_CONTRACT(0 <= d && d <= 1);
    return near_plane / d;
}

template <class ScalarT>
[[deprecated("use explicit _opengl or _directx version")]] [[nodiscard]] mat<4, 4, ScalarT> perspective(horizontal_fov_t<ScalarT> horizontal_fov,
                                                                                                        ScalarT aspect_ratio,
                                                                                                        ScalarT near_plane,
                                                                                                        ScalarT far_plane)
{
    return perspective_opengl(horizontal_fov, aspect_ratio, near_plane, far_plane);
}

template <class ScalarT>
[[deprecated("use explicit _opengl or _directx version")]] [[nodiscard]] mat<4, 4, ScalarT> perspective_reverse_z(horizontal_fov_t<ScalarT> horizontal_fov,
                                                                                                                  ScalarT aspect_ratio,
                                                                                                                  ScalarT near_plane)
{
    return perspective_reverse_z_opengl(horizontal_fov, aspect_ratio, near_plane);
}
}
