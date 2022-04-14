#include <nexus/fuzz_test.hh>

FUZZ_TEST("Minmax")(tg::rng& rng)
{
    // only values > 0
    auto box1 = tg::aabb1(tg::pos1(0.1f), tg::pos1(1.0f));

    // tests for vec, size and pos
    {
        // gets bigger
        auto a = uniform(rng, box1).x;
        auto b = a + uniform(rng, box1).x;
        auto c = b + uniform(rng, box1).x;

        // 1D
        {
            auto p = tg::pos1(a);
            auto v = tg::vec1(a);
            auto s = tg::size1(a);

            // min
            CHECK(tg::min_element(p) == a);
            CHECK(tg::min_element(v) == a);
            CHECK(tg::min_element(s) == a);
            // max
            CHECK(tg::max_element(p) == a);
            CHECK(tg::max_element(v) == a);
            CHECK(tg::max_element(s) == a);

            CHECK(tg::max(a) == a);
            CHECK(tg::min(a) == a);
        }

        // 2D
        {
            auto p = tg::pos2(a, b);
            auto v = tg::vec2(a, b);
            auto s = tg::size2(a, b);

            // min
            CHECK(tg::min_element(p) == a);
            CHECK(tg::min_element(v) == a);
            CHECK(tg::min_element(s) == a);
            // max
            CHECK(tg::max_element(p) == b);
            CHECK(tg::max_element(v) == b);
            CHECK(tg::max_element(s) == b);

            CHECK(tg::max(a, b) == b);
            CHECK(tg::min(a, b) == a);
        }

        // 3D
        {
            auto p = tg::pos3(a, b, c);
            auto v = tg::vec3(a, b, c);
            auto s = tg::size3(a, b, c);

            // min
            CHECK(tg::min_element(p) == a);
            CHECK(tg::min_element(v) == a);
            CHECK(tg::min_element(s) == a);
            // max
            CHECK(tg::max_element(p) == c);
            CHECK(tg::max_element(v) == c);
            CHECK(tg::max_element(s) == c);

            CHECK(tg::max(a, b, c) == c);
            CHECK(tg::min(a, b, c) == a);
        }
    }
}
