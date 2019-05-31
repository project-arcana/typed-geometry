#pragma once

#include <typed-geometry/types/angle.hh>
#include <typed-geometry/types/mat.hh>

#include <typed-geometry/common/assert.hh>
#include <typed-geometry/common/scalar_math.hh>

namespace tg
{
/// aspect ratio is width / height
template <class ScalarT>
TG_NODISCARD mat<4, 4, ScalarT> perspective_reverse_z(angle_t<ScalarT> horizontal_fov, ScalarT aspect_ratio, ScalarT near_plane)
{
    TG_CONTRACT(near_plane > 0);
    TG_CONTRACT(aspect_ratio > 0);
    TG_CONTRACT(horizontal_fov > 0_deg);
    TG_CONTRACT(horizontal_fov < 180_deg);

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
TG_NODISCARD ScalarT reverse_z_to_linear_depth(ScalarT d, ScalarT near_plane)
{
    TG_CONTRACT(0 <= d && d <= 1);
    return near_plane / d;
}
}
