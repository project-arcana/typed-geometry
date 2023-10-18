#include <nexus/ext/tg-approx.hh>
#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>
#include <typed-geometry/feature/quadric.hh>
#include <typed-geometry/feature/vector.hh>

FUZZ_TEST("Quadrics - PointQuadric")(tg::rng& rng)
{
    auto const bb = tg::aabb3(-10, 10);

    auto const p = uniform(rng, bb);
    auto const Q = tg::point_quadric(p);

    CHECK(Q(p) == nx::approx(0.f).abs(0.001f));

    for (auto i = 0; i < 10; ++i)
    {
        auto const p2 = uniform(rng, bb);
        CHECK(Q(p2) == nx::approx(distance_sqr(p, p2)).abs(0.01f));
    }

    auto const q = closest_point(Q);
    for (auto i : {0, 1, 2})
        CHECK(q[i] == nx::approx(p[i]));
}

FUZZ_TEST("Quadrics - PlaneQuadric")(tg::rng& rng)
{
    auto const bb = tg::aabb3(-10, 10);

    auto const p = uniform(rng, bb);
    auto const n = uniform_vec(rng, bb);
    auto const Q = tg::plane_quadric(p, n);
    auto const P = tg::plane(normalize(n), p);

    CHECK(Q(p) == nx::approx(0.f).abs(0.1f));

    for (auto i = 0; i < 10; ++i)
    {
        auto const p2 = uniform(rng, bb);
        CHECK(Q(p2) == nx::approx(distance_sqr(P, p2) * length_sqr(n)).abs(0.01f));
    }
}

FUZZ_TEST("Quadrics - ProbPlaneQuadricZero")(tg::rng& rng)
{
    auto const bb = tg::aabb3(-10, 10);

    auto const p = uniform(rng, bb);
    auto const n = uniform_vec(rng, bb);
    auto const Q = tg::probabilistic_plane_quadric(p, n, 0, 0);
    auto const P = tg::plane(normalize(n), p);

    CHECK(Q(p) == nx::approx(0.f).abs(0.1f));

    for (auto i = 0; i < 10; ++i)
    {
        auto const p2 = uniform(rng, bb);
        CHECK(Q(p2) == nx::approx(distance_sqr(P, p2) * length_sqr(n)).abs(0.01f));
    }
}

FUZZ_TEST("Quadrics - ProbPlaneQuadricZeroMat")(tg::rng& rng)
{
    auto const bb = tg::aabb3(-10, 10);

    auto const p = uniform(rng, bb);
    auto const n = uniform_vec(rng, bb);
    auto const Q = tg::probabilistic_plane_quadric(p, n, tg::mat3::zero, tg::mat3::zero);
    auto const P = tg::plane(normalize(n), p);

    CHECK(Q(p) == nx::approx(0.f).abs(0.1f));

    for (auto i = 0; i < 10; ++i)
    {
        auto const p2 = uniform(rng, bb);
        CHECK(Q(p2) == nx::approx(distance_sqr(P, p2) * length_sqr(n)).abs(0.01f));
    }
}

FUZZ_TEST("Quadrics - ProbPlaneQuadricNonZero")(tg::rng& rng)
{
    auto const bb = tg::aabb3(-10, 10);

    auto const p = uniform(rng, bb);
    auto const n = uniform_vec(rng, bb);
    auto const Q = tg::probabilistic_plane_quadric(p, n, 0.05f, 0.05f);
    auto const P = tg::plane(normalize(n), p);

    CHECK(Q(p) >= 0);

    for (auto i = 0; i < 10; ++i)
    {
        auto const p2 = uniform(rng, bb);
        CHECK(Q(p2) >= distance_sqr(P, p2) * length_sqr(n) - 0.1);
    }
}

FUZZ_TEST("Quadrics - ProbPlaneQuadricNonZeroMat")(tg::rng& rng)
{
    auto const bb = tg::aabb3(-10, 10);

    auto const sigma = 0.05f * tg::mat3::ones;

    auto const p = uniform(rng, bb);
    auto const n = uniform_vec(rng, bb);
    auto const Q = tg::probabilistic_plane_quadric(p, n, sigma, sigma);
    auto const P = tg::plane(normalize(n), p);

    CHECK(Q(p) >= 0);

    for (auto i = 0; i < 10; ++i)
    {
        auto const p2 = uniform(rng, bb);
        CHECK(Q(p2) >= distance_sqr(P, p2) * length_sqr(n) - 0.1);
    }
}

FUZZ_TEST("Quadrics - TriangleQuadric")(tg::rng& rng)
{
    auto const bb = tg::aabb3(-3, 3);

    auto const p0 = uniform(rng, bb);
    auto const p1 = uniform(rng, bb);
    auto const p2 = uniform(rng, bb);
    auto const T = tg::triangle(p0, p1, p2);
    auto const Q = tg::triangle_quadric(p0, p1, p2);
    auto const P = tg::plane(normal_of(T), p0);

    CHECK(Q(p0) == nx::approx(0.f).abs(0.1));
    CHECK(Q(p1) == nx::approx(0.f).abs(0.1));
    CHECK(Q(p2) == nx::approx(0.f).abs(0.1));
    CHECK(Q(T[{uniform(rng, 0.0f, 1.0f), uniform(rng, 0.0f, 1.0f)}]) == nx::approx(0.f).abs(0.1));

    for (auto i = 0; i < 10; ++i)
    {
        auto const p2 = uniform(rng, bb);
        CHECK(Q(p2) == nx::approx(distance_sqr(P, p2) * tg::pow2(area_of(T)) * 4).abs(0.1f));
    }
}

FUZZ_TEST("Quadrics - ProbTriangleQuadricZero")(tg::rng& rng)
{
    auto const bb = tg::aabb3(-3, 3);

    auto const p0 = uniform(rng, bb);
    auto const p1 = uniform(rng, bb);
    auto const p2 = uniform(rng, bb);
    auto const T = tg::triangle(p0, p1, p2);
    auto const Q = tg::probabilistic_triangle_quadric(p0, p1, p2, 0);
    auto const P = tg::plane(normal_of(T), p0);

    CHECK(Q(p0) == nx::approx(0.f).abs(0.1));
    CHECK(Q(p1) == nx::approx(0.f).abs(0.1));
    CHECK(Q(p2) == nx::approx(0.f).abs(0.1));
    CHECK(Q(T[{uniform(rng, 0.0f, 1.0f), uniform(rng, 0.0f, 1.0f)}]) == nx::approx(0.f).abs(0.1f));

    for (auto i = 0; i < 10; ++i)
    {
        auto const p2 = uniform(rng, bb);
        CHECK(Q(p2) == nx::approx(distance_sqr(P, p2) * tg::pow2(area_of(T)) * 4).abs(0.1f));
    }
}

FUZZ_TEST("Quadrics - ProbTriangleQuadricNonZero")(tg::rng& rng)
{
    auto const bb = tg::aabb3(-3, 3);

    auto const p0 = uniform(rng, bb);
    auto const p1 = uniform(rng, bb);
    auto const p2 = uniform(rng, bb);
    auto const T = tg::triangle(p0, p1, p2);
    auto const Q = tg::probabilistic_triangle_quadric(p0, p1, p2, 0.05f);
    auto const P = tg::plane(normal_of(T), p0);

    CHECK(Q(p0) >= 0);
    CHECK(Q(p1) >= 0);
    CHECK(Q(p2) >= 0);
    CHECK(Q(T[{uniform(rng, 0.0f, 1.0f), uniform(rng, 0.0f, 1.0f)}]) >= 0);

    for (auto i = 0; i < 10; ++i)
    {
        auto const p2 = uniform(rng, bb);
        CHECK(Q(p2) >= distance_sqr(P, p2) * tg::pow2(area_of(T)) * 4);
    }
}
