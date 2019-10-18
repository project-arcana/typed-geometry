#pragma once

#include <typed-geometry/functions/minmax.hh>
#include <typed-geometry/types/bezier.hh>
#include <typed-geometry/types/capped_array.hh>

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

/// simpsons integration
namespace tg::detail
{
/// http://steve.hollasch.net/cgindex/curves/cbezarclen.html
template <class ScalarT, class F>
ScalarT integrate_simpson(F&& f, ScalarT start, ScalarT end, i32 n_limit, ScalarT max_error)
{
    static_assert(is_same<decltype(f(ScalarT())), ScalarT>, "f must map from ScalarT to ScalarT!");

    i32 n = 1;
    ScalarT multiplier = (end - start) / ScalarT(6.0);
    ScalarT endsum = f(start) + f(end);
    ScalarT interval = (end - start) / ScalarT(2.0);
    ScalarT asum = 0;
    ScalarT bsum = f(start + interval);
    ScalarT est1 = multiplier * (endsum + 2 * asum + 4 * bsum);
    ScalarT est0 = 2 * est1;

    while (n < n_limit && (abs(est1) > 0 && abs((est1 - est0) / est1) > max_error))
    {
        n *= 2;
        multiplier /= 2;
        interval /= 2;
        asum += bsum;
        bsum = 0;
        est0 = est1;
        ScalarT interval_div_2n = interval / (ScalarT(2.0) * n);

        for (auto i = 1; i < 2 * n; i += 2)
        {
            ScalarT t = start + i * interval_div_2n;
            bsum += f(t);
        }

        est1 = multiplier * (endsum + 2 * asum + 4 * bsum);
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

template <int Degree, class ControlPointT, class F>
TG_NODISCARD auto map(bezier<Degree, ControlPointT> const& c, F&& f) -> bezier<Degree, decltype(f(c.control_points[0]))>
{
    bezier<Degree, decltype(f(c.control_points[0]))> res;
    for (auto i = 0; i <= Degree; ++i)
        res.control_points[i] = f(c.control_points[i]);
    return res;
}

template <int Degree, class ControlPointT, class... ControlPoints>
TG_NODISCARD constexpr auto fit_bezier(ControlPointT const& p0, ControlPoints const&... /*pts*/) -> bezier<sizeof...(ControlPoints), ControlPointT>
{
    // todo:
    // if number of control points == Degree -1 : perfect fit
    // if number of control points > Degree -1 : mse fit
    // if number of control points < Degree -a : ??? fit polynomial of fitting degree?

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

template <int Degree, int Dim, class ScalarT>
TG_NODISCARD constexpr auto normal_at(bezier<Degree, pos<Dim, ScalarT>> const& c, ScalarT t)
{
    return normal_f(c)(t);
}

template <int Degree, class ControlPointT, class ScalarT>
TG_NODISCARD constexpr auto tangent_at(bezier<Degree, ControlPointT> const& c, ScalarT t)
{
    return tangent_f(c)(t);
}

template <int Degree, class ScalarT>
TG_NODISCARD constexpr auto binormal_at(bezier<Degree, pos<3, ScalarT>> const& c, ScalarT t)
{
    return binormal_f(c)(t);
}

template <int Degree, class ScalarT>
TG_NODISCARD constexpr auto curvature_at(bezier<Degree, pos<3, ScalarT>> const& c, ScalarT t)
{
    return curvature_f(c)(t);
}

template <int Degree, class ScalarT>
TG_NODISCARD constexpr auto torsion_at(bezier<Degree, pos<3, ScalarT>> const& c, ScalarT t)
{
    return torsion_f(c)(t);
}

template <int Degree, class ControlPointT, class ScalarT>
TG_NODISCARD constexpr auto archlength_at(bezier<Degree, ControlPointT> const& c, ScalarT t)
{
    return length_f(c)(t);
}

template <int Degree, class ScalarT>
TG_NODISCARD constexpr auto normal_f(bezier<Degree, pos<3, ScalarT>> const& c)
{
    auto const d1 = derivative(c);
    auto const d2 = derivative(d1);
    return [d = d1, dd = d2](auto t) {
        auto const a = normalize(d1(t));
        auto const b = normalize(a + dd(t));
        auto const r = normalize(cross(a, b));
        return normalize(cross(r, a));
    };
}

template <int Degree, class ScalarT>
TG_NODISCARD constexpr auto normal_f(bezier<Degree, pos<2, ScalarT>> const& c)
{
    return [tang = tangent_f(c)](auto t) {
        auto const tangent = tang(t);
        return tg::dir<2, ScalarT>{tangent.y, -tangent.x};
    };
}

template <int Degree, class ControlPointT>
TG_NODISCARD constexpr auto tangent_f(bezier<Degree, ControlPointT> const& c)
{
    return [d = derivative(c)](auto t) { return normalize(d(t)); };
}

template <int Degree, class ScalarT>
TG_NODISCARD constexpr auto binormal_f(bezier<Degree, pos<3, ScalarT>> const& c)
{
    auto const d1 = derivative(c);
    auto const d2 = derivative(d1);
    return [d = d1, dd = d2](auto t) {
        auto const a = normalize(d1(t));
        auto const b = normalize(a + dd(t));
        return normalize(cross(a, b));
    };
}

template <int Degree, class ScalarT>
TG_NODISCARD constexpr auto curvature_f(bezier<Degree, pos<2, ScalarT>> const& c)
{
    auto const d1 = derivative(c);
    auto const d2 = derivative(d1);
    return [d = d1, dd = d2](auto t) {
        auto const dt = d(t);
        auto const ddt = dd(t);
        auto const numerator = dt.x * ddt.y + ddt.x * dt.y;
        auto const denominator = pow(dt.x * dt.x + dt.y * dt.y, ScalarT(1.5));
        return numerator / denominator;
    };
}

template <int Degree, class ScalarT>
TG_NODISCARD constexpr auto curvature_f(bezier<Degree, pos<3, ScalarT>> const& c)
{
    auto const d1 = derivative(c);
    auto const d2 = derivative(d1);
    return [d = d1, dd = d2](auto t) {
        auto const dt = d(t);
        auto const ddt = dd(t);
        auto const numerator = length(cross(dt, ddt));
        // todo: performance compare: pow vs 3 mult and 1 sqrt
        //        auto const x = sqrt(dt.x * dt.x + dt.y * dt.y + dt.z * dt.z);
        //        auto const denominator = x * x * x;
        auto const denominator = pow(dt.x * dt.x + dt.y * dt.y + dt.z * dt.z, ScalarT(1.5));
        return numerator / denominator;
    };
}

template <int Degree, class ScalarT>
TG_NODISCARD constexpr auto torsion_f(bezier<Degree, pos<3, ScalarT>> const& c)
{
    auto const d1 = derivative(c);
    auto const d2 = derivative(d1);
    auto const d3 = derivative(d2);
    return [d = d1, dd = d2, ddd = d3](auto t) {
        auto const v0 = d(t);
        auto const v1 = dd(t);
        auto const v2 = ddd(t);
        auto const m = from_cols(v0, v1, v2);
        auto const numerator = determinant(m);
        auto const denominator = length(cross(v1, v2));
        return numerator / denominator;
    };
}


template <class ScalarT>
TG_NODISCARD constexpr auto extremeties_t(bezier<1, comp<1, ScalarT>> const& c)
{
    // not actually useful
}

template <class ScalarT>
TG_NODISCARD constexpr capped_array<ScalarT, 1> extremeties_t(bezier<2, comp<1, ScalarT>> const& c)
{
    // up to one extremety
    auto const d = derivative(c);
}


template <int Degree, class ControlPointT>
TG_NODISCARD constexpr auto archlength_f(bezier<Degree, ControlPointT> const& c) -> decltype(length(c.control_points[1] - c.control_points[0]))
{
    // implement this maybe
    //    http://steve.hollasch.net/cgindex/curves/cbezarclen.html

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
