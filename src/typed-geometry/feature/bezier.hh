#pragma once

#include <typed-geometry/types/bezier.hh>

/*
    TODO:
    * arithmetic operations if ControlPointT allows them
    * automatically promote bezier type
    * extremeties up to degree 3 or 4
    * aabb_of up to degree 3 or 4
    * derivative_at
    * split -> pair<bezier>
    * merge?
    * normal
    * tangent
    * curvature
    * bitangent
    * torsion
    * at and _f(captured lambda)
    * map
    * length
    * arc_length_curve
    * project
    * bezier::from_fit(...)
    * fit_bezier(...)
    * splines
*/

namespace tg
{
namespace detail
{
template <class A, class T>
constexpr auto mix_helper(A const& a0, A const& a1, T const& t)
{
    return mix(a0, a1, t);
}

struct default_mix_t
{
    template <class A, class T>
    static constexpr auto mix(A const& a0, A const& a1, T const& t)
    {
        return mix_helper(a0, a1, t);
    }
};

template <class MixT, int Degree, class ControlPointT, class ScalarT>
TG_NODISCARD constexpr auto deCastlejau(bezier<Degree, ControlPointT> const& bezier, ScalarT const& t)
{
    using T = std::decay_t<decltype(MixT::mix(std::declval<ControlPointT>(), std::declval<ControlPointT>(), t))>;

    if constexpr (Degree < 0)
        return T{};
    else if constexpr (Degree == 0)
        return T(bezier.p[0]);
    else
    {
        T controlpoints[Degree + 1];
        for (auto i = 0; i <= Degree; ++i)
            controlpoints[i] = T(bezier.control_points[i]);
        for (auto d = Degree; d >= 1; --d)
            for (auto i = 0; i < d; ++i)
                controlpoints[i] = MixT::mix(controlpoints[i], controlpoints[i + 1], t);
        return controlpoints[0];
    }
}

}
template <int Degree, class ControlPointT>
template <class ScalarT, class MixT>
TG_NODISCARD constexpr auto bezier<Degree, ControlPointT>::operator()(ScalarT const& t) const
{
    return detail::deCastlejau<MixT>(*this, t);
}

template <int Degree, class ControlPointT>
template <class ScalarT, class MixT>
TG_NODISCARD constexpr auto bezier<Degree, ControlPointT>::operator[](ScalarT const& t) const
{
    return detail::deCastlejau<MixT>(*this, t);
}

template <int Degree, class ControlPointT>
TG_NODISCARD constexpr auto derivative(bezier<Degree, ControlPointT> const& c)
{
    bezier<Degree - 1, decltype(Degree * (c.control_points[1] - c.control_points[0]))> res;
    for (auto i = 0; i < Degree; ++i)
        res.control_points[i] = Degree * (c.control_points[i + 1] - c.control_points[i]);
    return res;
}

template <int Degree, class ControlPointT>
TG_NODISCARD constexpr bezier<Degree + 1, ControlPointT> elevate(bezier<Degree, ControlPointT> const& c)
{
    bezier<Degree + 1, ControlPointT> res;

    auto const new_degree = Degree + 1;
    res.control_points[0] = c.control_points[0];

    for (auto i = 1; i <= new_degree; ++i)
        res.control_points[i] = ((new_degree - i) * c.control_points[i] + i * c.control_points[i - 1]) / new_degree;

    return res;
}

}
