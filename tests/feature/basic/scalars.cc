#include <nexus/test.hh>

#include <typed-geometry/tg.hh>

#include <cmath>

TEST("fp classification")
{
    {
        auto x = 0.0f;
        CHECK(tg::is_finite(x));
        CHECK(!tg::is_inf(x));
        CHECK(!tg::is_nan(x));
        CHECK(!tg::is_normal(x));
        CHECK(tg::is_subnormal(x));
        CHECK(tg::is_zero(x));
    }

    {
        auto x = 42.0f;
        CHECK(tg::is_finite(x));
        CHECK(!tg::is_inf(x));
        CHECK(!tg::is_nan(x));
        CHECK(tg::is_normal(x));
        CHECK(!tg::is_subnormal(x));
        CHECK(!tg::is_zero(x));
    }

    {
        auto x = 1e-38f;
        CHECK(tg::is_finite(x));
        CHECK(!tg::is_inf(x));
        CHECK(!tg::is_nan(x));
        CHECK(!tg::is_normal(x));
        CHECK(tg::is_subnormal(x));
        CHECK(!tg::is_zero(x));
    }

    {
        auto x = tg::inf<float>;
        CHECK(!tg::is_finite(x));
        CHECK(tg::is_inf(x));
        CHECK(!tg::is_nan(x));
        CHECK(!tg::is_normal(x));
        CHECK(!tg::is_subnormal(x));
        CHECK(!tg::is_zero(x));
    }

    {
        auto x = tg::nan<float>;
        CHECK(!tg::is_finite(x));
        CHECK(!tg::is_inf(x));
        CHECK(tg::is_nan(x));
        CHECK(!tg::is_normal(x));
        CHECK(!tg::is_subnormal(x));
        CHECK(!tg::is_zero(x));
    }
}
