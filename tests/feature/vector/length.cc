#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/vector.hh>

FUZZ_TEST("Length")(tg::rng& rng)
{
    auto box1 = tg::aabb1(tg::pos1(-1.0f), tg::pos1(1.0f));
    auto box2 = tg::aabb2(tg::pos2(-1.0f), tg::pos2(1.0f));
    auto box3 = tg::aabb3(tg::pos3(-1.0f), tg::pos3(1.0f));
    auto box4 = tg::aabb4(tg::pos4(-1.0f), tg::pos4(1.0f));

    // length and length_sqr tests

    // check consistency of length and length_sqr
    // vecs
    {
        auto v = tg::vec1(uniform(rng, box1));
        CHECK(tg::length(v) == nx::approx(tg::sqrt(tg::length_sqr(v))));
    }
    {
        auto v = tg::vec2(uniform(rng, box2));
        CHECK(tg::length(v) == nx::approx(tg::sqrt(tg::length_sqr(v))));
    }
    {
        auto v = tg::vec3(uniform(rng, box3));
        CHECK(tg::length(v) == nx::approx(tg::sqrt(tg::length_sqr(v))));
    }
    {
        auto v = tg::vec4(uniform(rng, box4));
        CHECK(tg::length(v) == nx::approx(tg::sqrt(tg::length_sqr(v))));
    }

    // segments
    {
        auto s = tg::segment1(uniform(rng, box1), uniform(rng, box1));
        CHECK(tg::length(s) == nx::approx(tg::sqrt(tg::length_sqr(s))));
    }
    {
        auto s = tg::segment2(uniform(rng, box2), uniform(rng, box2));
        CHECK(tg::length(s) == nx::approx(tg::sqrt(tg::length_sqr(s))));
    }
    {
        auto s = tg::segment3(uniform(rng, box3), uniform(rng, box3));
        CHECK(tg::length(s) == nx::approx(tg::sqrt(tg::length_sqr(s))));
    }
    {
        auto s = tg::segment4(uniform(rng, box4), uniform(rng, box4));
        CHECK(tg::length(s) == nx::approx(tg::sqrt(tg::length_sqr(s))));
    }

    // only one component non-zero, length is simply abs of that component
    {
        auto v = tg::vec1(uniform(rng, box1).x);
        CHECK(length(v) == tg::abs(v.x));
        CHECK(tg::length(v) == nx::approx(tg::sqrt(tg::length_sqr(v))));
    }
    {
        auto v = tg::vec2(uniform(rng, box1).x, 0.0f);
        CHECK(length(v) == tg::abs(v.x));
        CHECK(tg::length(v) == nx::approx(tg::sqrt(tg::length_sqr(v))));
    }
    {
        auto v = tg::vec3(uniform(rng, box1).x, 0.0f, 0.0f);
        CHECK(length(v) == tg::abs(v.x));
        CHECK(tg::length(v) == nx::approx(tg::sqrt(tg::length_sqr(v))));
    }
    {
        auto v = tg::vec4(uniform(rng, box1).x, 0.0f, 0.0f, 0.0f);
        CHECK(length(v) == tg::abs(v.x));
        CHECK(tg::length(v) == nx::approx(tg::sqrt(tg::length_sqr(v))));
    }

    // only one component non-zero, length is simply abs of that component
    {
        auto s = tg::segment1(tg::pos1::zero, uniform(rng, box1));
        CHECK(length(s) == tg::abs(s.pos1.x));
        CHECK(tg::length(s) == nx::approx(tg::sqrt(tg::length_sqr(s))));
    }
    {
        auto p = tg::pos2(uniform(rng, box1).x, 0.0f);
        auto s = tg::segment2(tg::pos2::zero, p);
        CHECK(length(s) == tg::abs(p.x));
        CHECK(tg::length(s) == nx::approx(tg::sqrt(tg::length_sqr(s))));
    }
    {
        auto p = tg::pos3(uniform(rng, box1).x, 0.0f, 0.0f);
        auto s = tg::segment3(tg::pos3::zero, p);
        CHECK(length(s) == tg::abs(p.x));
        CHECK(tg::length(s) == nx::approx(tg::sqrt(tg::length_sqr(s))));
    }
    {
        auto p = tg::pos4(uniform(rng, box1).x, 0.0f, 0.0f, 0.0f);
        auto s = tg::segment4(tg::pos4::zero, p);
        CHECK(length(s) == tg::abs(p.x));
        CHECK(tg::length(s) == nx::approx(tg::sqrt(tg::length_sqr(s))));
    }
}
