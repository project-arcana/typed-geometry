#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/fwd_diff.hh>
#include <typed-geometry/feature/vector.hh>

namespace
{
template <class F, class T>
void test_fwd_vs_finite_diff(F&& f, T val, T eps = T(1e-4))
{
    auto fd_result = f(tg::fwd_diff<T>::input(val));
    auto v0 = f(val);
    auto v1 = f(val + eps);
    auto g = (v1 - v0) / eps;
    CHECK(v0 == nx::approx(fd_result.value).abs(0.01f));
    CHECK(g == nx::approx(fd_result.derivative).abs(0.1f).rel(0.1f));
}
}

FUZZ_TEST("FwdDiff - FiniteDiffFuzz")(tg::rng& rng)
{
    // basics
    test_fwd_vs_finite_diff([](auto v) { return v; }, uniform(rng, -10.f, 10.f));

    test_fwd_vs_finite_diff([](auto v) { return v + v; }, uniform(rng, -10.f, 10.f));
    test_fwd_vs_finite_diff([](auto v) { return v + 2.3f; }, uniform(rng, -10.f, 10.f));
    test_fwd_vs_finite_diff([](auto v) { return -1.7f + v; }, uniform(rng, -10.f, 10.f));

    test_fwd_vs_finite_diff([](auto v) { return v - v; }, uniform(rng, -10.f, 10.f));
    test_fwd_vs_finite_diff([](auto v) { return v - 2.3f; }, uniform(rng, -10.f, 10.f));
    test_fwd_vs_finite_diff([](auto v) { return -1.7f - v; }, uniform(rng, -10.f, 10.f));

    test_fwd_vs_finite_diff([](auto v) { return v * v; }, uniform(rng, -10.f, 10.f));
    test_fwd_vs_finite_diff([](auto v) { return v * 2.3f; }, uniform(rng, -10.f, 10.f));
    test_fwd_vs_finite_diff([](auto v) { return -1.7f * v; }, uniform(rng, -10.f, 10.f));

    test_fwd_vs_finite_diff([](auto v) { return v / v; }, uniform(rng, 1.f, 10.f) * uniform(rng, {-1, 1}));
    test_fwd_vs_finite_diff([](auto v) { return v / 2.3f; }, uniform(rng, -10.f, 10.f));
    test_fwd_vs_finite_diff([](auto v) { return -1.7f / v; }, uniform(rng, 1.f, 10.f) * uniform(rng, {-1, 1}));

    test_fwd_vs_finite_diff([](auto v) { return tg::abs(v); }, uniform(rng, 1.f, 10.f) * uniform(rng, {-1, 1}));
    test_fwd_vs_finite_diff([](auto v) { return tg::sqrt(v); }, uniform(rng, 0.1f, 10.f));
    test_fwd_vs_finite_diff([](auto v) { return tg::exp(v); }, uniform(rng, -10.f, 10.f));
    test_fwd_vs_finite_diff([](auto v) { return tg::log(v); }, uniform(rng, 0.1f, 10.f));

    // composite
    test_fwd_vs_finite_diff([](auto v) { return 1 / v; }, uniform(rng, 1.f, 10.f) * uniform(rng, {-1, 1}));
    test_fwd_vs_finite_diff([](auto v) { return v + 5 + v * v; }, uniform(rng, -10.f, 10.f));
    test_fwd_vs_finite_diff([](auto v) { return (2.3f * v + 5 - v * v) / (3 + v * v); }, uniform(rng, -10.f, 10.f));
    test_fwd_vs_finite_diff([](auto v) { return tg::sqrt(1 + v * v); }, uniform(rng, -10.f, 10.f));
    test_fwd_vs_finite_diff([](auto v) { return 2 - v * v; }, uniform(rng, -10.f, 10.f));
    test_fwd_vs_finite_diff([](auto v) { return length(tg::vec(v, 2 - v * v, v / (1 + v * v))); }, uniform(rng, -10., 10.), 1e-7);
}
