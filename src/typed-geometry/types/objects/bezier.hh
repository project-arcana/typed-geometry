#pragma once

#include <typed-geometry/functions/mix.hh>
#include <typed-geometry/types/array.hh>

namespace tg
{
// fwd
template <int Degree, class ControlPointT>
struct bezier;
// namespace detail
//{
// template <int Degree, class ControlPointT, class ScalarT>
// TG_NODISCARD constexpr ControlPointT deCastlejau(bezier<Degree, ControlPointT> const& bezier, ScalarT const& t);
//}

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

template <int Degree, class ControlPointT, class ScalarT, class MixT = default_mix_t>
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
struct bezier
{
    ControlPointT control_points[Degree + 1];

    static constexpr int degree = Degree;

    //    template <int OtherDegree>
    //    constexpr explicit bezier(bezier<OtherDegree, ControlPointT> const& /*other*/)
    //    {
    //        static_assert(Degree >= OtherDegree, "must have at least the same degree");
    //        // todo
    //    }

    template <class ScalarT>
    TG_NODISCARD constexpr ControlPointT operator()(ScalarT const& t) const
    {
        return detail::deCastlejau(*this, t);
    }

    template <class ScalarT>
    TG_NODISCARD constexpr ControlPointT operator[](ScalarT const& t) const
    {
        return detail::deCastlejau(*this, t);
    }
};

template <class ControlPointT, class... ControlPoints>
TG_NODISCARD constexpr auto make_bezier(ControlPointT const& p0, ControlPoints const&... pts) -> bezier<sizeof...(ControlPoints), ControlPointT>
{
    static_assert((std::is_convertible_v<ControlPoints, ControlPointT> && ...), "incompatible control points");
    return {{p0, pts...}};
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
