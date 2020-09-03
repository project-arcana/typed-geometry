#pragma once

#include <typed-geometry/types/angle.hh>
#include <typed-geometry/types/mat.hh>

#include <typed-geometry/detail/operators/ops_angle.hh>
#include <typed-geometry/feature/assert.hh>
#include <typed-geometry/functions/basic/scalar_math.hh>

namespace tg
{
/// aspect ratio is width / height
template <class ScalarT>
[[nodiscard]] mat<4, 4, ScalarT> perspective_reverse_z_opengl(angle_t<ScalarT> horizontal_fov, ScalarT aspect_ratio, ScalarT near_plane)
{
    TG_CONTRACT(near_plane > 0);
    TG_CONTRACT(aspect_ratio > 0);
    TG_CONTRACT(horizontal_fov > degree(0));
    TG_CONTRACT(horizontal_fov < degree(180));

    // adapted from glm for reverse Z and infinite far
    // see https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/

    auto const tan_half_hfov = tan(horizontal_fov / ScalarT(2));

    auto m = mat<4, 4, ScalarT>::zero;
    m[0][0] = ScalarT(1) / (aspect_ratio * tan_half_hfov);
    m[1][1] = ScalarT(1) / tan_half_hfov;
    m[2][3] = -ScalarT(1);
    m[3][2] = near_plane;

    return m;
}

template <class ScalarT>
[[nodiscard]] mat<4, 4, ScalarT> perspective_reverse_z_directx(angle_t<ScalarT> horizontal_fov, ScalarT aspect_ratio, ScalarT near_plane)
{
    TG_CONTRACT(near_plane > 0);
    TG_CONTRACT(aspect_ratio > 0);
    TG_CONTRACT(horizontal_fov > degree(0));
    TG_CONTRACT(horizontal_fov < degree(180));

    auto const tan_half_hfov = tan(horizontal_fov / ScalarT(2));

    auto m = mat<4, 4, ScalarT>::zero;
    m[0][0] = ScalarT(1) / (aspect_ratio * tan_half_hfov);
    m[1][1] = ScalarT(1) / tan_half_hfov;
    m[2][3] = ScalarT(1);
    m[3][2] = near_plane;

    return m;
}

template <class ScalarT>
[[nodiscard]] mat<4, 4, ScalarT> perspective_directx(angle_t<ScalarT> horizontal_fov, ScalarT aspect_ratio, ScalarT near_plane, ScalarT far_plane)
{
    TG_CONTRACT(near_plane > 0);
    TG_CONTRACT(far_plane > 0);
    TG_CONTRACT(aspect_ratio > 0);
    TG_CONTRACT(horizontal_fov > degree(0));
    TG_CONTRACT(horizontal_fov < degree(180));

    auto const tan_half_hfov = tan(horizontal_fov / ScalarT(2));
    auto const y_scale = ScalarT(1) / tan_half_hfov;
    auto const x_scale = y_scale / aspect_ratio;

    auto m = mat<4, 4, ScalarT>::zero;
    m[0][0] = x_scale;
    m[1][1] = y_scale;
    m[2][2] = far_plane / (far_plane - near_plane);
    m[3][2] = -near_plane * far_plane / (far_plane - near_plane);
    m[2][3] = ScalarT(1);

    return m;
}

template <class ScalarT>
[[nodiscard]] mat<4, 4, ScalarT> perspective_opengl(angle_t<ScalarT> horizontal_fov, ScalarT aspect_ratio, ScalarT near_plane, ScalarT far_plane)
{
    TG_CONTRACT(near_plane > 0);
    TG_CONTRACT(far_plane > 0);
    TG_CONTRACT(aspect_ratio > 0);
    TG_CONTRACT(horizontal_fov > degree(0));
    TG_CONTRACT(horizontal_fov < degree(180));

    auto const tan_half_hfov = tan(horizontal_fov / ScalarT(2));
    auto const y_scale = ScalarT(1) / tan_half_hfov;
    auto const x_scale = y_scale / aspect_ratio;

    auto m = mat<4, 4, ScalarT>::zero;
    m[0][0] = x_scale;
    m[1][1] = y_scale;
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
[[deprecated("use explicit _opengl or _directx version")]] [[nodiscard]] mat<4, 4, ScalarT> perspective(angle_t<ScalarT> horizontal_fov,
                                                                                                        ScalarT aspect_ratio,
                                                                                                        ScalarT near_plane,
                                                                                                        ScalarT far_plane)
{
    return perspective_opengl(horizontal_fov, aspect_ratio, near_plane, far_plane);
}

template <class ScalarT>
[[deprecated("use explicit _opengl or _directx version")]] [[nodiscard]] mat<4, 4, ScalarT> perspective_reverse_z(angle_t<ScalarT> horizontal_fov,
                                                                                                                  ScalarT aspect_ratio,
                                                                                                                  ScalarT near_plane)
{
    return perspective_reverse_z_opengl(horizontal_fov, aspect_ratio, near_plane);
}
}
