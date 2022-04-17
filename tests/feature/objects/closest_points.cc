#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>

FUZZ_TEST("ClosestPoints - PosPos")(tg::rng& rng)
{
    auto range1 = tg::aabb1(tg::pos1(-10), tg::pos1(10));
    auto range2 = tg::aabb2(tg::pos2(-10), tg::pos2(10));
    auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));
    auto range4 = tg::aabb4(tg::pos4(-10), tg::pos4(10));

    {
        auto a = uniform(rng, range1);
        auto b = uniform(rng, range1);

        auto cp = tg::closest_points(a, b);
        CHECK(a == cp.first);
        CHECK(b == cp.second);
    }
    {
        auto a = uniform(rng, range2);
        auto b = uniform(rng, range2);

        auto cp = tg::closest_points(a, b);
        CHECK(a == cp.first);
        CHECK(b == cp.second);
    }
    {
        auto a = uniform(rng, range3);
        auto b = uniform(rng, range3);

        auto cp = tg::closest_points(a, b);
        CHECK(a == cp.first);
        CHECK(b == cp.second);
    }
    {
        auto a = uniform(rng, range4);
        auto b = uniform(rng, range4);

        auto cp = tg::closest_points(a, b);
        CHECK(a == cp.first);
        CHECK(b == cp.second);
    }
}


FUZZ_TEST("ClosestPoints - SegmentPos")(tg::rng& rng)
{
    auto range1 = tg::aabb1(tg::pos1(-10), tg::pos1(10));
    auto range2 = tg::aabb2(tg::pos2(-10), tg::pos2(10));
    auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));
    auto range4 = tg::aabb4(tg::pos4(-10), tg::pos4(10));

    auto eps = 1e-5f;

    {
        auto a = uniform(rng, range1);
        auto b = uniform(rng, range1);

        int attempts = 0;
        int maxAttempts = 1000;
        while (tg::distance(a, b) < eps && attempts < maxAttempts)
        {
            b = uniform(rng, range1);
            attempts++;
        }
        CHECK(attempts < maxAttempts);

        auto p = uniform(rng, range1);

        auto seg = tg::segment1(a, b);

        auto cp = tg::closest_points(seg, p).first;
        auto p_a2 = tg::distance(p, a);
        auto p_b2 = tg::distance(p, b);
        auto cp_a2 = tg::distance(cp, a);
        auto cp_b2 = tg::distance(cp, b);

        CHECK(cp_a2 <= p_a2 + eps);
        CHECK(cp_b2 <= p_b2 + eps);
    }

    {
        auto a = uniform(rng, range2);
        auto b = uniform(rng, range2);

        int attempts = 0;
        int maxAttempts = 1000;
        while (tg::distance(a, b) < eps && attempts < maxAttempts)
        {
            b = uniform(rng, range2);
            attempts++;
        }
        CHECK(attempts < maxAttempts);

        auto p = uniform(rng, range2);

        auto seg = tg::segment2(a, b);

        auto cp = tg::closest_points(seg, p).first;
        auto p_a2 = tg::distance(p, a);
        auto p_b2 = tg::distance(p, b);
        auto cp_a2 = tg::distance(cp, a);
        auto cp_b2 = tg::distance(cp, b);

        CHECK(cp_a2 <= p_a2 + eps);
        CHECK(cp_b2 <= p_b2 + eps);
    }

    {
        auto a = uniform(rng, range3);
        auto b = uniform(rng, range3);

        int attempts = 0;
        int maxAttempts = 1000;
        while (tg::distance(a, b) < eps && attempts < maxAttempts)
        {
            b = uniform(rng, range3);
            attempts++;
        }
        CHECK(attempts < maxAttempts);

        auto p = uniform(rng, range3);

        auto seg = tg::segment3(a, b);

        auto cp = tg::closest_points(seg, p).first;
        auto p_a2 = tg::distance(p, a);
        auto p_b2 = tg::distance(p, b);
        auto cp_a2 = tg::distance(cp, a);
        auto cp_b2 = tg::distance(cp, b);

        CHECK(cp_a2 <= p_a2 + eps);
        CHECK(cp_b2 <= p_b2 + eps);
    }

    {
        auto a = uniform(rng, range4);
        auto b = uniform(rng, range4);

        int attempts = 0;
        int maxAttempts = 1000;
        while (tg::distance(a, b) < eps && attempts < maxAttempts)
        {
            b = uniform(rng, range4);
            attempts++;
        }
        CHECK(attempts < maxAttempts);

        auto p = uniform(rng, range4);

        auto seg = tg::segment4(a, b);

        auto cp = tg::closest_points(seg, p).first;
        auto p_a2 = tg::distance(p, a);
        auto p_b2 = tg::distance(p, b);
        auto cp_a2 = tg::distance(cp, a);
        auto cp_b2 = tg::distance(cp, b);

        CHECK(cp_a2 <= p_a2 + eps);
        CHECK(cp_b2 <= p_b2 + eps);
    }
}

FUZZ_TEST("ClosestPoints - LineLine3")(tg::rng& rng)
{
    auto bb = tg::aabb3(-10, 10);
    auto l0 = tg::line3::from_points(uniform(rng, bb), uniform(rng, bb));
    auto l1 = tg::line3::from_points(uniform(rng, bb), uniform(rng, bb));

    auto d = distance(l0, l1);
    auto [p0, p1] = tg::closest_points(l0, l1);
    auto [t0, t1] = tg::closest_points_parameters(l0, l1);

    CHECK(distance(p0, p1) == nx::approx(d).abs(0.001f));
    CHECK(distance(l0[t0], l1[t1]) == nx::approx(d).abs(0.001f));
}
