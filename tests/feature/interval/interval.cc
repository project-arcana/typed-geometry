#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/interval.hh>
#include <typed-geometry/feature/vector.hh>

namespace
{
template <class F, class T>
void test_interval(tg::rng& rng, F&& f, tg::interval<T> vals)
{
    auto ri = f(vals);

    auto vmin = tg::max(-100.f, vals.min);
    auto vmax = tg::min(100.f, vals.max);

    for (auto i = 0; i < 10; ++i)
    {
        auto v = uniform(rng, vmin, vmax);
        auto r = f(v);

        if (tg::is_nan(r))
            continue;

        CHECK(!tg::is_nan(ri.min));
        CHECK(!tg::is_nan(ri.max));
        TG_ASSERT(ri.min <= r);
        TG_ASSERT(r <= ri.max);
        CHECK(ri.min <= r);
        CHECK(r <= ri.max);
    }
}
template <class F, class T>
void test_interval(tg::rng& rng, F&& f, tg::interval<T> vals0, tg::interval<T> vals1)
{
    auto ri = f(vals0, vals1);

    auto v0min = tg::max(-100.f, vals0.min);
    auto v0max = tg::min(100.f, vals0.max);
    auto v1min = tg::max(-100.f, vals1.min);
    auto v1max = tg::min(100.f, vals1.max);

    for (auto i = 0; i < 10; ++i)
    {
        auto v0 = uniform(rng, v0min, v0max);
        auto v1 = uniform(rng, v1min, v1max);
        auto r = f(v0, v1);

        if (tg::is_nan(r))
            continue;

        CHECK(!tg::is_nan(ri.min));
        CHECK(!tg::is_nan(ri.max));
        TG_ASSERT(ri.min <= r);
        TG_ASSERT(r <= ri.max);
        CHECK(ri.min <= r);
        CHECK(r <= ri.max);
    }
}
}

FUZZ_TEST("Intervals - Fuzz")(tg::rng& rng)
{
    auto const get_rand_val = [&](float s) -> float {
        switch (uniform(rng, 0, 4))
        {
        default:
        case 0:
            return uniform(rng, -s, s);
        case 1:
            return -tg::inf<float>;
        case 2:
            return tg::inf<float>;
        case 3:
            return 0;
        }
    };

    auto const random_interval = [&]() -> tg::interval<float> {
        auto s = tg::pow(2.f, uniform(rng, -3.f, 5.f));
        auto v0 = get_rand_val(s);
        auto v1 = get_rand_val(s);

        tg::interval<float> r;

        r.min = tg::min(v0, v1);
        r.max = tg::max(v0, v1);

        while (r.min == tg::inf<float>)
            r.min = get_rand_val(s);
        while (r.max == -tg::inf<float>)
            r.max = get_rand_val(s);

        return r;
    };

    auto const v0 = random_interval();
    auto const v1 = random_interval();

    auto const c = uniform(rng, -2.f, 2.f);

    // basics
    test_interval(rng, [](auto v) { return v; }, v0);

    test_interval(rng, [c](auto v) { return v + c; }, v0);
    test_interval(rng, [c](auto v) { return v - c; }, v0);
    test_interval(rng, [c](auto v) { return v * c; }, v0);
    test_interval(rng, [c](auto v) { return v / c; }, v0);

    test_interval(rng, [c](auto v) { return c + v; }, v0);
    test_interval(rng, [c](auto v) { return c - v; }, v0);
    test_interval(rng, [c](auto v) { return c * v; }, v0);
    test_interval(rng, [c](auto v) { return c / v; }, v0);

    test_interval(rng, [](auto a, auto b) { return a + b; }, v0, v1);
    test_interval(rng, [](auto a, auto b) { return a - b; }, v0, v1);
    test_interval(rng, [](auto a, auto b) { return a * b; }, v0, v1);
    test_interval(rng, [](auto a, auto b) { return a / b; }, v0, v1);

    test_interval(rng, [](auto a) { return a + a; }, v0);
    test_interval(rng, [](auto a) { return a - a; }, v0);
    test_interval(rng, [](auto a) { return a * a; }, v0);
    test_interval(rng, [](auto a) { return a / a; }, v0);

    test_interval(rng, [](auto v) { return tg::abs(v); }, v0);
    test_interval(rng, [](auto v) { return tg::sqrt(v); }, v0);
    test_interval(rng, [](auto v) { return tg::exp(v); }, v0);
    test_interval(rng, [](auto v) { return tg::log(v); }, v0);

    // composite
    test_interval(rng, [](auto a, auto b) { return length(tg::vec(a, 2 - a * b, a / (1 + b * b))); }, v0, v1);
}

TEST("special self-op intervals")
{
    auto a = tg::interval(-1.f, 2.f);
    auto b = tg::interval(-1.f, 2.f);
    CHECK(contains(a, 1));
    CHECK(contains(a, 0));
    CHECK(!contains(a, -2));
    CHECK(!contains(a, 3));
    CHECK(contains(a, b));

    CHECK(a + a == tg::interval(-2.f, 4.f));
    CHECK(a - a == tg::interval(0.f, 0.f));
    CHECK(a * a == tg::interval(0.f, 4.f));
    CHECK(a / a == tg::interval(1.f, 1.f));

    CHECK(a + b == tg::interval(-2.f, 4.f));
    CHECK(a - b == tg::interval(-3.f, 3.f));
    CHECK(a * b == tg::interval(-2.f, 4.f));
    CHECK(a / b == tg::interval<float>::complete);
}
