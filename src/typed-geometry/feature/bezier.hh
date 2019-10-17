#pragma once

#include <typed-geometry/functions/minmax.hh>
#include <typed-geometry/types/bezier.hh>

/*
    TODO:
    * [x] arithmetic operations if ControlPointT allows them
    * [x] automatically promote bezier type
    * [ ] extremeties up to degree 3 or 4
    * [ ] aabb_of up to degree 3 or 4
    * [-] derivative_at
    * [-] split -> pair<bezier>
    * [-] merge?
    * [-] normal
    * [x] tangent
    * [-] curvature
    * [-] bitangent
    * [-] torsion
    * [-] length
    * [-] at and _f(captured lambda)
    * [x] map
    * [ ] arc_length_curve
    * [ ] project
    * [ ] bezier::from_fit(...)
    * [-] fit_bezier(...)
    * [ ] splines
    * [ ] derivative_at better performance
    * [ ] rotation minimizing frame
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
TG_NODISCARD constexpr auto deCastlejau(bezier<Degree, ControlPointT> const& b, ScalarT const& t)
{
    TG_CONTRACT(ScalarT(0) <= t && t <= ScalarT(1));
    using T = std::decay_t<decltype(MixT::mix(std::declval<ControlPointT>(), std::declval<ControlPointT>(), t))>;

    if constexpr (Degree < 0)
        return T{};
    else if constexpr (Degree == 0)
        return T(b.p[0]);
    else
    {
        T controlpoints[Degree + 1];
        for (auto i = 0; i <= Degree; ++i)
            controlpoints[i] = T(b.control_points[i]);
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
    if constexpr (Degree == 0)
        return bezier<0, ControlPointT>(); // always zero
    else
    {
        bezier<Degree - 1, decltype(Degree * (c.control_points[1] - c.control_points[0]))> res;
        for (auto i = 0; i < Degree; ++i)
            res.control_points[i] = Degree * (c.control_points[i + 1] - c.control_points[i]);
        return res;
    }
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

template <int DegreeA, class ControlPoinAT, int DegreeB, class ControlPoinBT>
TG_NODISCARD constexpr auto operator+(bezier<DegreeA, ControlPoinAT> const& a, bezier<DegreeB, ControlPoinBT> const& b)
    -> bezier<max(DegreeA, DegreeB), decltype(a.control_points[0] + b.control_points[0])>
{
    using T = decltype(a.control_points[0] + b.control_points[0]);
    bezier<max(DegreeA, DegreeB), T> res;

    if constexpr (DegreeA == DegreeB)
    {
        for (auto i = 0; i <= DegreeA; ++i)
            res.control_points[i] = a.control_points[i] + b.control_points[i];
    }
    else if constexpr (DegreeA < DegreeB)
    {
        auto const promoted_a = bezier<DegreeB, ControlPoinAT>(a);
        for (auto i = 0; i <= DegreeB; ++i)
            res.control_points[i] = promoted_a.control_points[i] + b.control_points[i];
    }
    else // DegreeB < DegreeA
    {
        auto const promoted_b = bezier<DegreeA, ControlPoinBT>(b);
        for (auto i = 0; i <= DegreeA; ++i)
            res.control_points[i] = a.control_points[i] + promoted_b.control_points[i];
    }
    return res;
}

template <int DegreeA, class ControlPoinAT, int DegreeB, class ControlPoinBT>
TG_NODISCARD constexpr auto operator-(bezier<DegreeA, ControlPoinAT> const& a, bezier<DegreeB, ControlPoinBT> const& b)
    -> bezier<max(DegreeA, DegreeB), decltype(a.control_points[0] - b.control_points[0])>
{
    using T = decltype(a.control_points[0] - b.control_points[0]);
    bezier<max(DegreeA, DegreeB), T> res;

    if constexpr (DegreeA == DegreeB)
    {
        for (auto i = 0; i <= DegreeA; ++i)
            res.control_points[i] = a.control_points[i] - b.control_points[i];
    }
    else if constexpr (DegreeA < DegreeB)
    {
        auto const promoted_a = bezier<DegreeB, ControlPoinAT>(a);
        for (auto i = 0; i <= DegreeB; ++i)
            res.control_points[i] = promoted_a.control_points[i] - b.control_points[i];
    }
    else // DegreeB < DegreeA
    {
        auto const promoted_b = bezier<DegreeA, ControlPoinBT>(b);
        for (auto i = 0; i <= DegreeA; ++i)
            res.control_points[i] = a.control_points[i] - promoted_b.control_points[i];
    }
    return res;
}

template <int DegreeA, class ControlPoinAT, int DegreeB, class ControlPoinBT>
TG_NODISCARD constexpr auto operator*(bezier<DegreeA, ControlPoinAT> const& a, bezier<DegreeB, ControlPoinBT> const& b)
    -> bezier<max(DegreeA, DegreeB), decltype(a.control_points[0] * b.control_points[0])>
{
    using T = decltype(a.control_points[0] * b.control_points[0]);
    bezier<max(DegreeA, DegreeB), T> res;

    if constexpr (DegreeA == DegreeB)
    {
        for (auto i = 0; i <= DegreeA; ++i)
            res.control_points[i] = a.control_points[i] * b.control_points[i];
    }
    else if constexpr (DegreeA < DegreeB)
    {
        auto const promoted_a = bezier<DegreeB, ControlPoinAT>(a);
        for (auto i = 0; i <= DegreeB; ++i)
            res.control_points[i] = promoted_a.control_points[i] * b.control_points[i];
    }
    else // DegreeB < DegreeA
    {
        auto const promoted_b = bezier<DegreeA, ControlPoinBT>(b);
        for (auto i = 0; i <= DegreeA; ++i)
            res.control_points[i] = a.control_points[i] * promoted_b.control_points[i];
    }
    return res;
}

template <int DegreeA, class ControlPoinAT, int DegreeB, class ControlPoinBT>
TG_NODISCARD constexpr auto operator/(bezier<DegreeA, ControlPoinAT> const& a, bezier<DegreeB, ControlPoinBT> const& b)
    -> bezier<max(DegreeA, DegreeB), decltype(a.control_points[0] / b.control_points[0])>
{
    using T = decltype(a.control_points[0] / b.control_points[0]);
    bezier<max(DegreeA, DegreeB), T> res;

    if constexpr (DegreeA == DegreeB)
    {
        for (auto i = 0; i <= DegreeA; ++i)
            res.control_points[i] = a.control_points[i] / b.control_points[i];
    }
    else if constexpr (DegreeA < DegreeB)
    {
        auto const promoted_a = bezier<DegreeB, ControlPoinAT>(a);
        for (auto i = 0; i <= DegreeB; ++i)
            res.control_points[i] = promoted_a.control_points[i] / b.control_points[i];
    }
    else // DegreeB < DegreeA
    {
        auto const promoted_b = bezier<DegreeA, ControlPoinBT>(b);
        for (auto i = 0; i <= DegreeA; ++i)
            res.control_points[i] = a.control_points[i] / promoted_b.control_points[i];
    }
    return res;
}

template <int Degree, class ControlPointT, class F>
TG_NODISCARD auto map(bezier<Degree, ControlPointT> const& c, F&& f) -> bezier<Degree, decltype(f(c.control_points[0]))>
{
    bezier<Degree, decltype(f(c.control_points[0]))> res;
    for (auto i = 0; i <= Degree; ++i)
        res.control_points[i] = f(c.control_points[i]);
    return res;
}

template <class ControlPointT, class... ControlPoints>
TG_NODISCARD constexpr auto fit_bezier(ControlPointT const& p0, ControlPoints const&... /*pts*/) -> bezier<sizeof...(ControlPoints), ControlPointT>
{
    (void)p0;
    // (void)pts;
    // todo
}

template <int Degree, class ControlPointT, class ScalarT>
TG_NODISCARD constexpr auto derivative_at(bezier<Degree, ControlPointT> const& c, ScalarT t)
{
    // todo: more performant version
    return derivative(c)(t);
}

template <int Degree, class ControlPointT, class ScalarT>
TG_NODISCARD constexpr auto normal_at(bezier<Degree, ControlPointT> const& c, ScalarT t)
{
    (void)t;
    (void)c;
    // todo
}

template <int Degree, class ControlPointT, class ScalarT>
TG_NODISCARD constexpr auto tangent_at(bezier<Degree, ControlPointT> const& c, ScalarT t)
{
    return tangent_f(c)(t);
}

template <int Degree, class ControlPointT, class ScalarT>
TG_NODISCARD constexpr auto bitangent_at(bezier<Degree, ControlPointT> const& c, ScalarT t)
{
    (void)t;
    (void)c;
    // todo
}

template <int Degree, class ControlPointT, class ScalarT>
TG_NODISCARD constexpr auto curvature_at(bezier<Degree, ControlPointT> const& c, ScalarT t)
{
    (void)t;
    (void)c;
    // todo
}

template <int Degree, class ControlPointT, class ScalarT>
TG_NODISCARD constexpr auto torsion_at(bezier<Degree, ControlPointT> const& c, ScalarT t)
{
    (void)t;
    (void)c;
    // todo
}

template <int Degree, class ControlPointT, class ScalarT>
TG_NODISCARD constexpr auto length_at(bezier<Degree, ControlPointT> const& c, ScalarT t)
{
    (void)t;
    (void)c;
    // todo
}

template <class MixT = detail::default_mix_t, int Degree, class ControlPointT, class ScalarT>
TG_NODISCARD constexpr auto split_at(bezier<Degree, ControlPointT> const& c, ScalarT t)
{
    TG_CONTRACT(ScalarT(0) <= t && t <= ScalarT(1));
    using T = std::decay_t<decltype(MixT::mix(std::declval<ControlPointT>(), std::declval<ControlPointT>(), t))>;

    bezier<Degree, T> left;
    bezier<Degree, T> right;

    T controlpoints[Degree + 1];
    for (auto i = 0; i <= Degree; ++i)
        controlpoints[i] = T(c.control_points[i]);

    for (auto d = Degree; d >= 1; --d)
    {
        left.control_points[Degree - d] = controlpoints[0];
        right.control_points[d] = controlpoints[d];
        for (auto i = 0; i < d; ++i)
        {
            controlpoints[i] = MixT::mix(controlpoints[i], controlpoints[i + 1], t);
        }
    }
    left.control_points[Degree] = controlpoints[0];
    right.control_points[0] = controlpoints[0];
    return pair{left, right};
}

template <int Degree, class ControlPointT>
TG_NODISCARD constexpr auto normal_f(bezier<Degree, ControlPointT> const& c)
{
    (void)c;
    // todo
}

template <int Degree, class ControlPointT>
TG_NODISCARD constexpr auto tangent_f(bezier<Degree, ControlPointT> const& c)
{
    return [d = derivative(c)](auto t) { return normalize(d(t)); };
}

template <int Degree, class ControlPointT>
TG_NODISCARD constexpr auto bitangent_f(bezier<Degree, ControlPointT> const& c)
{
    (void)c;
    // todo
}

template <int Degree, class ControlPointT>
TG_NODISCARD constexpr auto curvature_f(bezier<Degree, ControlPointT> const& c)
{
    return [d = derivative(c), dd = derivative(derivative(c))](auto t) {
        auto const dt = d(t);
        auto const ddt = dd(t);
    };
}

template <int Degree, class ControlPointT>
TG_NODISCARD constexpr auto torsion_f(bezier<Degree, ControlPointT> const& c)
{
    (void)c;
    // todo
}

template <int Degree, class ControlPointT>
TG_NODISCARD constexpr auto length_f(bezier<Degree, ControlPointT> const& c) -> decltype(length(c.control_points[1] - c.control_points[0]))
{
    static_assert(Degree < 2, "not yet implemented");

    if constexpr (Degree == 0)
        return {};
    if constexpr (Degree == 1)
        return length(c.control_points[1] - c.control_points[0]);
    if constexpr (Degree == 2)
    {
        // 2D:
        //     2/3 x^(3/2) (4 (-a (1 - x) - 2 b x + b + c x)^2 + 4 (-d (1 - x) - 2 e x + e + f x)^2)
    }
    else
    {
        // todo: do numerical integration sceme of your choice
    }
}

}
