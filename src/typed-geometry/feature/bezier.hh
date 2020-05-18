#pragma once

#include <type_traits> // std::is_invocable_r_t, std::decay_t

#include <typed-geometry/feature/basic.hh>
#include <typed-geometry/functions/basic/minmax.hh>
#include <typed-geometry/types/bezier.hh>
#include <typed-geometry/types/capped_vector.hh>

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
    * [-] binormal
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
    * [ ] number of extremeties depends on the number of dimension of the control points
*/


namespace tg::detail
{
/// simpsons integration
/// http://steve.hollasch.net/cgindex/curves/cbezarclen.html
template <class ScalarT, class F>
ScalarT integrate_simpson(F&& f, ScalarT start, ScalarT end, i32 n_limit, ScalarT max_error)
{
    static_assert(std::is_invocable_r_v<ScalarT, F, ScalarT>, "f must map from ScalarT to ScalarT!");

    i32 n = 1;
    ScalarT multiplier = (end - start) / ScalarT(6.0);
    ScalarT endsum = f(start) + f(end);
    ScalarT interval = (end - start) / ScalarT(2.0);
    ScalarT asum = ScalarT(0);
    ScalarT bsum = f(start + interval);
    ScalarT est1 = multiplier * (endsum + ScalarT(2) * asum + ScalarT(4) * bsum);
    ScalarT est0 = ScalarT(2) * est1;

    while (n < n_limit && (abs(est1) > ScalarT(0) && abs((est1 - est0) / est1) > max_error))
    {
        n *= 2;
        multiplier /= ScalarT(2);
        interval /= ScalarT(2);
        asum += bsum;
        bsum = ScalarT(0);
        est0 = est1;
        ScalarT interval_div_2n = interval / (ScalarT(2.0) * n);

        for (auto i = 1; i < 2 * n; i += 2)
        {
            ScalarT t = start + i * interval_div_2n;
            bsum += f(t);
        }

        est1 = multiplier * (endsum + ScalarT(2) * asum + ScalarT(4) * bsum);
    }

    return est1;
}
}

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
[[nodiscard]] constexpr auto deCastlejau(bezier<Degree, ControlPointT> const& b, ScalarT const& t)
{
    TG_CONTRACT(ScalarT(0) <= t && t <= ScalarT(1));
    using T = std::decay_t<decltype(MixT::mix(std::declval<ControlPointT>(), std::declval<ControlPointT>(), t))>;

    if constexpr (Degree < 0)
        return T{};
    else if constexpr (Degree == 0)
        return T(b.control_points[0]);
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
[[nodiscard]] constexpr auto bezier<Degree, ControlPointT>::operator()(ScalarT const& t) const
{
    return detail::deCastlejau<MixT>(*this, t);
}

template <int Degree, class ControlPointT>
template <class ScalarT, class MixT>
[[nodiscard]] constexpr auto bezier<Degree, ControlPointT>::operator[](ScalarT const& t) const
{
    return detail::deCastlejau<MixT>(*this, t);
}

template <int Degree, class ControlPointT>
[[nodiscard]] constexpr auto derivative(bezier<Degree, ControlPointT> const& c)
{
    using T = std::decay_t<decltype(Degree * (c.control_points[1] - c.control_points[0]))>;

    if constexpr (Degree == 0)
        return bezier<0, T>(); // always zero
    else
    {
        bezier<Degree - 1, T> res;
        for (auto i = 0; i < Degree; ++i)
            res.control_points[i] = Degree * (c.control_points[i + 1] - c.control_points[i]);
        return res;
    }
}

template <int Degree, class ControlPointT>
[[nodiscard]] constexpr bezier<Degree + 1, ControlPointT> elevate(bezier<Degree, ControlPointT> const& c)
{
    bezier<Degree + 1, ControlPointT> res;

    auto const new_degree = Degree + 1;
    res.control_points[0] = c.control_points[0];

    for (auto i = 1; i <= new_degree; ++i)
        res.control_points[i] = ((new_degree - i) * c.control_points[i] + i * c.control_points[i - 1]) / new_degree;

    return res;
}

template <int DegreeA, class ControlPoinAT, int DegreeB, class ControlPoinBT>
[[nodiscard]] constexpr auto operator+(bezier<DegreeA, ControlPoinAT> const& a, bezier<DegreeB, ControlPoinBT> const& b)
    -> bezier<max(DegreeA, DegreeB), std::decay_t<decltype(a.control_points[0] + b.control_points[0])>>
{
    using T = std::decay_t<decltype(a.control_points[0] + b.control_points[0])>;
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
[[nodiscard]] constexpr auto operator-(bezier<DegreeA, ControlPoinAT> const& a, bezier<DegreeB, ControlPoinBT> const& b)
    -> bezier<max(DegreeA, DegreeB), std::decay_t<decltype(a.control_points[0] - b.control_points[0])>>
{
    using T = std::decay_t<decltype(a.control_points[0] - b.control_points[0])>;
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
[[nodiscard]] constexpr auto operator*(bezier<DegreeA, ControlPoinAT> const& a, bezier<DegreeB, ControlPoinBT> const& b)
    -> bezier<max(DegreeA, DegreeB), std::decay_t<decltype(a.control_points[0] * b.control_points[0])>>
{
    using T = std::decay_t<decltype(a.control_points[0] * b.control_points[0])>;
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
[[nodiscard]] constexpr auto operator/(bezier<DegreeA, ControlPoinAT> const& a, bezier<DegreeB, ControlPoinBT> const& b)
    -> bezier<max(DegreeA, DegreeB), std::decay_t<decltype(a.control_points[0] / b.control_points[0])>>
{
    using T = std::decay_t<decltype(a.control_points[0] / b.control_points[0])>;
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

template <class MixT = detail::default_mix_t, int Degree, class ControlPointT, class ScalarT>
[[nodiscard]] constexpr auto split_at(bezier<Degree, ControlPointT> const& c, ScalarT t)
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

template <int Degree, class ControlPointT, class F>
[[nodiscard]] auto map(bezier<Degree, ControlPointT> const& c, F&& f) -> bezier<Degree, decltype(f(c.control_points[0]))>
{
    bezier<Degree, decltype(f(c.control_points[0]))> res;
    for (auto i = 0; i <= Degree; ++i)
        res.control_points[i] = f(c.control_points[i]);
    return res;
}

template <int Degree, class ControlPointT, class... ControlPoints>
[[nodiscard]] constexpr auto fit_bezier(ControlPointT const& p0, ControlPoints const&... /*pts*/) -> bezier<sizeof...(ControlPoints), ControlPointT>
{
    // todo:
    // if number of control points == Degree -1 : perfect fit
    // if number of control points > Degree -1 : mse fit
    // if number of control points < Degree -a : ??? fit polynomial of fitting degree?

    (void)p0;
    // (void)pts;
    // todo
    return {}; // because constexpr return
}

template <int Degree, class ControlPointT, class ScalarT>
[[nodiscard]] constexpr auto derivative_at(bezier<Degree, ControlPointT> const& c, ScalarT t)
{
    // todo: more performant version
    return derivative(c)(t);
}

template <int Degree, int Dim, class ScalarT>
[[nodiscard]] constexpr auto normal_at(bezier<Degree, pos<Dim, ScalarT>> const& c, ScalarT t)
{
    return normal_f(c)(t);
}

template <int Degree, class ControlPointT, class ScalarT>
[[nodiscard]] constexpr auto tangent_at(bezier<Degree, ControlPointT> const& c, ScalarT t)
{
    return tangent_f(c)(t);
}

template <int Degree, class ScalarT>
[[nodiscard]] constexpr auto binormal_at(bezier<Degree, pos<3, ScalarT>> const& c, ScalarT t)
{
    return binormal_f(c)(t);
}

template <int Degree, class ScalarT, int D>
[[nodiscard]] constexpr auto curvature_at(bezier<Degree, pos<D, ScalarT>> const& c, ScalarT t)
{
    return curvature_f(c)(t);
}

template <int Degree, class ScalarT>
[[nodiscard]] constexpr auto torsion_at(bezier<Degree, pos<3, ScalarT>> const& c, ScalarT t)
{
    return torsion_f(c)(t);
}

template <int Degree, class ControlPointT, class ScalarT>
[[nodiscard]] constexpr auto archlength_at(bezier<Degree, ControlPointT> const& c, ScalarT t)
{
    return length_f(c)(t);
}

template <int Degree, class ScalarT>
[[nodiscard]] constexpr auto normal_f(bezier<Degree, pos<3, ScalarT>> const& c)
{
    auto const d1 = derivative(c);
    auto const d2 = derivative(d1);
    return [d1, d2](auto t) {
        auto const a = normalize(d1(t));
        auto const b = normalize(a + d2(t));
        auto const r = normalize(cross(a, b));
        return tg::dir<3, ScalarT>(cross(r, a)); // is normalized
    };
}

template <int Degree, class ScalarT>
[[nodiscard]] constexpr auto normal_f(bezier<Degree, pos<2, ScalarT>> const& c)
{
    return [tang = tangent_f(c)](auto t) { return perpendicular(tang(t)); };
}

template <int Degree, class ControlPointT>
[[nodiscard]] constexpr auto tangent_f(bezier<Degree, ControlPointT> const& c)
{
    return [d = derivative(c)](auto t) { return normalize(d(t)); };
}

template <int Degree, class ScalarT>
[[nodiscard]] constexpr auto binormal_f(bezier<Degree, pos<3, ScalarT>> const& c)
{
    auto const d1 = derivative(c);
    auto const d2 = derivative(d1);
    return [d1, d2](auto t) {
        auto const a = normalize(d1(t));
        auto const b = normalize(a + d2(t));
        return normalize(cross(a, b));
    };
}

template <int Degree, class ScalarT>
[[nodiscard]] constexpr auto curvature_f(bezier<Degree, pos<2, ScalarT>> const& c)
{
    auto const d1 = derivative(c);
    auto const d2 = derivative(d1);
    return [d1, d2](auto t) {
        auto const dt = d1(t);
        auto const ddt = d2(t);
        auto const numerator = cross(dt, ddt);
        auto const x = length_sqr(dt);
        auto const denominator = x * sqrt(x);
        return numerator / denominator;
    };
}

template <int Degree, class ScalarT>
[[nodiscard]] constexpr auto curvature_f(bezier<Degree, pos<3, ScalarT>> const& c)
{
    auto const d1 = derivative(c);
    auto const d2 = derivative(d1);
    return [d1, d2](auto t) {
        auto const dt = d1(t);
        auto const ddt = d2(t);
        auto const numerator = length(cross(dt, ddt));
        auto const x = length_sqr(dt);
        auto const denominator = x * sqrt(x);
        return numerator / denominator;
    };
}

template <int Degree, class ScalarT>
[[nodiscard]] constexpr auto torsion_f(bezier<Degree, pos<3, ScalarT>> const& c)
{
    auto const d1 = derivative(c);
    auto const d2 = derivative(d1);
    auto const d3 = derivative(d2);
    return [d1, d2, d3](auto t) {
        auto const v0 = d1(t);
        auto const v1 = d2(t);
        auto const v2 = d3(t);
        auto const m = from_cols(v0, v1, v2);
        auto const numerator = determinant(m);
        auto const denominator = length(cross(v1, v2));
        return numerator / denominator;
    };
}

template <class ScalarT>
[[nodiscard]] constexpr capped_vector<ScalarT, 1> extremeties_t(bezier<2, comp<1, ScalarT>> const& c)
{
    // up to one extremety
    auto const d = derivative(c);
    // todo
    return {}; // because constexpr return
}

template <int Degree, class ControlPointT>
[[nodiscard]] constexpr auto archlength_f(bezier<Degree, ControlPointT> const& c) -> decltype(length(c.control_points[1] - c.control_points[0]))
{
    // implement this maybe
    //    http://steve.hollasch.net/cgindex/curves/cbezarclen.html

    //    static_assert(Degree < 2, "not yet implemented");

    if constexpr (Degree == 0)
        return [](auto) { return 0; };
    if constexpr (Degree == 1)
        return [=](auto t) { return length(t * c.control_points[1] - (1 - t) * c.control_points[0]); };
    else
    {
        auto const d = derivative(c);
        auto const dl = [d](auto t) { return length(d(t)); };
        // todo: change these magic values to something useful and/or make configurable
        return [dl](auto t) { return detail::integrate_simpson(dl, 0.0, t, 100, 0.00001); };
    }

    // there are still closed forms for Degree == 2, possibly implement them later
    //    if constexpr (Degree == 2)
    //    {
    //        // 2D:
    //        //     2/3 x^(3/2) (4 (-a (1 - x) - 2 b x + b + c x)^2 + 4 (-d (1 - x) - 2 e x + e + f x)^2)
    //    }
    //    else
    //    {
    //        // todo: do numerical integration sceme of your choice
    //    }
    return {}; // because constexpr return
}
template <int Degree, class ControlPointT>
[[nodiscard]] constexpr auto archlength(bezier<Degree, ControlPointT> const& c) -> decltype(length(c.control_points[1] - c.control_points[0]))
{
    using T = decltype(length(c.control_points[1] - c.control_points[0]));
    if constexpr (Degree == 0)
        return T(0);
    if constexpr (Degree == 1)
        return length(c.control_points[1] - c.control_points[0]);
    else
    {
        auto const d = derivative(c);
        auto const dl = [d](auto t) { return length(d(t)); };
        // todo: change these magic values to something useful and/or make configurable
        return detail::integrate_simpson(dl, 0.0, 1.0, 100, 0.00001);
    }
}
}
