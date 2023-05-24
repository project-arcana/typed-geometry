#include <nexus/fuzz_test.hh>

FUZZ_TEST("clamp fuzzer")(tg::rng& rng)
{
    auto range2 = tg::aabb2(-10, 10);

    {
        auto b = uniform(rng, range2);
        auto minVal = tg::min(b.x, b.y);
        auto maxVal = tg::max(b.x, b.y);

        auto val = uniform(rng, -10.0f, 10.0f);
        auto clamped = tg::clamp(val, minVal, maxVal);

        CHECK(clamped >= minVal);
        CHECK(clamped <= maxVal);
    }
}
