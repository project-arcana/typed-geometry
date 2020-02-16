#pragma once

#include <type_traits>

namespace tg
{
// fwd
template <int Degree, class ControlPointT>
struct bezier;
namespace detail
{
struct default_mix_t;
}

template <int Degree, class ControlPointT>
struct bezier
{
    static constexpr int degree = Degree;

    ControlPointT control_points[Degree + 1];

    constexpr bezier() = default;

    template <class... ControlPoints>
    constexpr bezier(ControlPointT const& cp0, ControlPoints const&... pts)
    {
        control_points[0] = cp0;
        auto i = 1;
        ((control_points[i++] = pts), ...);
    }

    template <int OtherDegree>
    constexpr explicit bezier(bezier<OtherDegree, ControlPointT> const& rhs)
    {
        static_assert(Degree >= OtherDegree, "must have at least the same degree");
        if constexpr (Degree == OtherDegree)
            *this = rhs;
        else
            *this = bezier(elevate(rhs));
    }

    template <class OtherControlPointT>
    constexpr explicit bezier(bezier<Degree, OtherControlPointT> const& rhs)
    {
        for (auto i = 0; i <= Degree; ++i)
            control_points[i] = ControlPointT(rhs.control_points[i]);
    }

    template <class OtherControlPointT>
    constexpr explicit bezier(bezier<Degree, OtherControlPointT>&& rhs)
    {
        for (auto i = 0; i <= Degree; ++i)
            control_points[i] = ControlPointT(rhs.control_points[i]);
    }


    template <class ScalarT, class MixT = detail::default_mix_t>
    [[nodiscard]] constexpr auto operator()(ScalarT const& t) const;

    template <class ScalarT, class MixT = detail::default_mix_t>
    [[nodiscard]] constexpr auto operator[](ScalarT const& t) const;

    [[nodiscard]] constexpr bool operator==(bezier const& rhs) const noexcept
    {
        for (auto i = 0; i <= Degree; ++i)
            if (control_points[i] != rhs.control_points[i])
                return false;
        return true;
    }

    [[nodiscard]] constexpr bool operator!=(bezier const& rhs) const noexcept
    {
        for (auto i = 0; i <= Degree; ++i)
            if (control_points[i] != rhs.control_points[i])
                return true;
        return false;
    }
};

template <class ControlPointT, class... ControlPoints>
[[nodiscard]] constexpr auto make_bezier(ControlPointT const& p0, ControlPoints const&... pts) -> bezier<sizeof...(ControlPoints), ControlPointT>
{
    static_assert((std::is_convertible_v<ControlPoints, ControlPointT> && ...), "incompatible control points");
    return {p0, pts...};
}
}
