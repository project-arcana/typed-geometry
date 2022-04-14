#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/quadric.hh>

TG_FUZZ_TEST(Quadrics, PointQuadric)
{
    auto const bb = tg::aabb3(-10, 10);

    auto const p = uniform(rng, bb);
    auto const Q = tg::point_quadric(p);

    CHECK(Q(p) == approx(0));

    for (auto i = 0; i < 10; ++i)
    {
        auto const p2 = uniform(rng, bb);
        CHECK(Q(p2) == approx(distance_sqr(p, p2)));
    }

    auto const q = closest_point(Q);
    for (auto i : {0, 1, 2})
        CHECK(q[i] == approx(p[i]));
}

TG_FUZZ_TEST(Quadrics, PlaneQuadric)
{
    auto const bb = tg::aabb3(-10, 10);

    auto const p = uniform(rng, bb);
    auto const n = uniform_vec(rng, bb);
    auto const Q = tg::plane_quadric(p, n);
    auto const P = tg::plane(normalize(n), p);

    CHECK(Q(p) == approx(0).epsilon(0.01));

    for (auto i = 0; i < 10; ++i)
    {
        auto const p2 = uniform(rng, bb);
        CHECK(Q(p2) == approx(distance_sqr(P, p2) * length_sqr(n)).epsilon(0.01));
    }
}

TG_FUZZ_TEST(Quadrics, ProbPlaneQuadricZero)
{
    auto const bb = tg::aabb3(-10, 10);

    auto const p = uniform(rng, bb);
    auto const n = uniform_vec(rng, bb);
    auto const Q = tg::probabilistic_plane_quadric(p, n, 0, 0);
    auto const P = tg::plane(normalize(n), p);

    CHECK(Q(p) == approx(0).epsilon(0.01));

    for (auto i = 0; i < 10; ++i)
    {
        auto const p2 = uniform(rng, bb);
        CHECK(Q(p2) == approx(distance_sqr(P, p2) * length_sqr(n)).epsilon(0.01));
    }
}

TG_FUZZ_TEST(Quadrics, ProbPlaneQuadricZeroMat)
{
    auto const bb = tg::aabb3(-10, 10);

    auto const p = uniform(rng, bb);
    auto const n = uniform_vec(rng, bb);
    auto const Q = tg::probabilistic_plane_quadric(p, n, tg::mat3::zero, tg::mat3::zero);
    auto const P = tg::plane(normalize(n), p);

    CHECK(Q(p) == approx(0).epsilon(0.01));

    for (auto i = 0; i < 10; ++i)
    {
        auto const p2 = uniform(rng, bb);
        CHECK(Q(p2) == approx(distance_sqr(P, p2) * length_sqr(n)).epsilon(0.01));
    }
}

TG_FUZZ_TEST(Quadrics, ProbPlaneQuadricNonZero)
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
        CHECK(Q(p2) >= distance_sqr(P, p2) * length_sqr(n));
    }
}

TG_FUZZ_TEST(Quadrics, ProbPlaneQuadricNonZeroMat)
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
        CHECK(Q(p2) >= distance_sqr(P, p2) * length_sqr(n));
    }
}

TG_FUZZ_TEST(Quadrics, TriangleQuadric)
{
    auto const bb = tg::aabb3(-3, 3);

    auto const p0 = uniform(rng, bb);
    auto const p1 = uniform(rng, bb);
    auto const p2 = uniform(rng, bb);
    auto const T = tg::triangle(p0, p1, p2);
    auto const Q = tg::triangle_quadric(p0, p1, p2);
    auto const P = tg::plane(normal_of(T), p0);

    CHECK(Q(p0) == approx(0).epsilon(0.1));
    CHECK(Q(p1) == approx(0).epsilon(0.1));
    CHECK(Q(p2) == approx(0).epsilon(0.1));
    CHECK(Q(T[{uniform(rng, 0.0f, 1.0f), uniform(rng, 0.0f, 1.0f)}]) == approx(0).epsilon(0.1));

    for (auto i = 0; i < 10; ++i)
    {
        auto const p2 = uniform(rng, bb);
        CHECK(Q(p2) == approx(distance_sqr(P, p2) * tg::pow2(area_of(T)) * 4).epsilon(0.01));
    }
}

TG_FUZZ_TEST(Quadrics, ProbTriangleQuadricZero)
{
    auto const bb = tg::aabb3(-3, 3);

    auto const p0 = uniform(rng, bb);
    auto const p1 = uniform(rng, bb);
    auto const p2 = uniform(rng, bb);
    auto const T = tg::triangle(p0, p1, p2);
    auto const Q = tg::probabilistic_triangle_quadric(p0, p1, p2, 0);
    auto const P = tg::plane(normal_of(T), p0);

    CHECK(Q(p0) == approx(0).epsilon(0.1));
    CHECK(Q(p1) == approx(0).epsilon(0.1));
    CHECK(Q(p2) == approx(0).epsilon(0.1));
    CHECK(Q(T[{uniform(rng, 0.0f, 1.0f), uniform(rng, 0.0f, 1.0f)}]) == approx(0).epsilon(0.1));

    for (auto i = 0; i < 10; ++i)
    {
        auto const p2 = uniform(rng, bb);
        CHECK(Q(p2) == approx(distance_sqr(P, p2) * tg::pow2(area_of(T)) * 4).epsilon(0.01));
    }
}

TG_FUZZ_TEST(Quadrics, ProbTriangleQuadricNonZero)
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
