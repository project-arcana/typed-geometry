#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/basic.hh>

TEST("tg::ilog2")
{
    CHECK(tg::ilog2(1.f) == 0);
    CHECK(tg::ilog2(1.5f) == 0);
    CHECK(tg::ilog2(1.999f) == 0);

    CHECK(tg::ilog2(2.0f) == 1);
    CHECK(tg::ilog2(4.0f) == 2);
    CHECK(tg::ilog2(1023.99f) == 9);
    CHECK(tg::ilog2(1024.0f) == 10);
    CHECK(tg::ilog2(1024.01f) == 10);

    CHECK(tg::ilog2(1.) == 0);
    CHECK(tg::ilog2(1.5) == 0);
    CHECK(tg::ilog2(1.999) == 0);

    CHECK(tg::ilog2(2.0) == 1);
    CHECK(tg::ilog2(4.0) == 2);
    CHECK(tg::ilog2(1023.99) == 9);
    CHECK(tg::ilog2(1024.0) == 10);
    CHECK(tg::ilog2(1024.01) == 10);
}

FUZZ_TEST("tg::ilog2 fuzzer")(tg::rng& rng)
{
    {
        auto f = uniform(rng, 0.00001f, 1.f);
        CHECK(tg::ilog2(f) == tg::ifloor(tg::log2(f)));
    }
    {
        auto f = uniform(rng, 1.f, 1000.f);
        CHECK(tg::ilog2(f) == tg::ifloor(tg::log2(f)));
    }
    {
        auto f = uniform(rng, 0.00001, 1.);
        CHECK(tg::ilog2(f) == tg::ifloor(tg::log2(f)));
    }
    {
        auto f = uniform(rng, 1., 1000.);
        CHECK(tg::ilog2(f) == tg::ifloor(tg::log2(f)));
    }
}
