#pragma once

#include <clean-core/macros.hh>

#include <typed-geometry/detail/scalar_traits.hh>
#include <typed-geometry/detail/utility.hh>
#include <typed-geometry/types/fwd.hh>
#include <typed-geometry/types/scalars/default.hh>

namespace tg
{
enum class alpha_type
{
    /// no alpha in the type, e.g. (R, G, B)
    none,

    /// extra alpha component, e.g. (R, G, B, A)
    straight,

    /// premultiplied alpha, e.g. (aR, aG, aB, a)
    premultiplied
};

template <class T>
struct color_traits
{
    static constexpr bool is_color = false;
};

/// checks if a type is a color
/// e.g. is_color<tg::color4> == true
///  but is_color<tg::vec3> == false
template <class T>
static constexpr bool is_color = color_traits<T>::is_color;

template <class T>
static constexpr bool has_alpha = color_traits<T>::alpha != alpha_type::none;
template <class T>
static constexpr bool has_straight_alpha = color_traits<T>::alpha == alpha_type::straight;
template <class T>
static constexpr bool has_premultiplied_alpha = color_traits<T>::alpha == alpha_type::premultiplied;

template <class T>
using linear_color_t_of = typename color_traits<T>::linear_color_t;

namespace detail
{
template <class ScalarT>
struct color_scalar
{
    static_assert(always_false<ScalarT>, "this scalar type is not cleared for use with colors");
};

template <>
struct color_scalar<f32>
{
    static constexpr f32 to_float(f32 v) { return v; }
    static constexpr f32 from_float(f32 v) { return v; }

    static constexpr f32 one() { return 1.f; }
};
template <>
struct color_scalar<u8>
{
    static constexpr f32 to_float(u8 v) { return v / 255.f; }
    static constexpr u8 from_float(f32 v)
    {
        v *= 256;
        return u8(v < 0 ? 0 : v > 255 ? 255 : int(v));
    }

    static constexpr u8 one() { return 255; }
};
}

/// converts a float value to the appropriate value for storing it as a certain color scalar type
/// e.g. if ScalarT is u8, we store 255 * v
template <class ScalarT>
constexpr ScalarT color_scalar_from_float(float v)
{
    return detail::color_scalar<ScalarT>::from_float(v);
}
/// converts a color storage type back to float [0..1]
template <class ScalarT>
constexpr float color_scalar_to_float(ScalarT v)
{
    return detail::color_scalar<ScalarT>::to_float(v);
}

namespace detail
{
/// helper for implementing color traits
/// NOTE: D is WITH alpha!
template <int D, class ScalarT, alpha_type Alpha>
struct base_color_traits
{
    using scalar_t = ScalarT;

    /// the linear RGB type appropriate for this color
    /// is usually color3 or color4 (if alpha)
    /// (or double versions for larger types)
    using linear_color_t = std::conditional_t<Alpha == alpha_type::none, //
                                              color<3, fractional_result<ScalarT>>,
                                              color<4, fractional_result<ScalarT>>>;

    static constexpr alpha_type alpha = Alpha;
    static constexpr bool is_color = true;
    static constexpr int comps = D;
};
}
}
