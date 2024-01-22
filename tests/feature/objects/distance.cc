#include <nexus/fuzz_test.hh>

#include <iostream>

#include <typed-geometry/tg.hh>

FUZZ_TEST("Distance")(tg::rng& rng)
{
    auto rBox1 = tg::aabb1(tg::pos1(-1.0f), tg::pos1(1.0f));
    auto rBox2 = tg::aabb2(tg::pos2(-1.0f, -1.0f), tg::pos2(1.0f, 1.0f));
    auto rBox3 = tg::aabb3(tg::pos3(-1.0f, -1.0f, -1.0f), tg::pos3(1.0f, 1.0f, 1.0f));

    // distance_sqr compared with distance
    {
        // 3D
        auto a = uniform(rng, rBox3);
        auto b = uniform(rng, rBox3);

        auto d2 = distance_sqr(a, b);
        auto d = distance(a, b);
        CHECK(d * d == nx::approx(d2));
        CHECK(d == nx::approx(tg::sqrt(d2)));
        // symmetry
        CHECK(distance(b, a) == d);
        CHECK(distance_sqr(b, a) == d2);
    }
    {
        // 2D
        auto a = uniform(rng, rBox2);
        auto b = uniform(rng, rBox2);

        auto d2 = distance_sqr(a, b);
        auto d = distance(a, b);
        CHECK(d * d == nx::approx(d2));
        CHECK(d == nx::approx(tg::sqrt(d2)));
        // symmetry
        CHECK(distance(b, a) == d);
        CHECK(distance_sqr(b, a) == d2);
    }
    {
        // 1D
        auto a = uniform(rng, rBox1);
        auto b = uniform(rng, rBox1);

        auto d2 = distance_sqr(a, b);
        auto d = distance(a, b);
        CHECK(d * d == nx::approx(d2));
        CHECK(d == nx::approx(tg::sqrt(d2)));
        // symmetry
        CHECK(distance(b, a) == d);
        CHECK(distance_sqr(b, a) == d2);
    }

    // move away from point by random value
    {
        // 3D
        auto a = uniform(rng, rBox3);
        auto dir = normalize(tg::vec3(uniform(rng, rBox3)));
        auto rd = uniform(rng, rBox1).x;

        auto b = a + dir * rd;
        CHECK(distance(a, b) == nx::approx(tg::abs(rd)));

        auto d2 = distance_sqr(a, b);
        auto d = distance(a, b);
        CHECK(d * d == nx::approx(d2));
        CHECK(d == nx::approx(tg::sqrt(d2)));
        // symmetry
        CHECK(distance(b, a) == d);
        CHECK(distance_sqr(b, a) == d2);
    }
    {
        // 2D
        auto a = uniform(rng, rBox2);
        auto dir = normalize(tg::vec2(uniform(rng, rBox2)));
        auto rd = uniform(rng, rBox1).x;

        auto b = a + dir * rd;
        CHECK(distance(a, b) == nx::approx(tg::abs(rd)));

        auto d2 = distance_sqr(a, b);
        auto d = distance(a, b);
        CHECK(d * d == nx::approx(d2));
        CHECK(d == nx::approx(tg::sqrt(d2)));
        // symmetry
        CHECK(distance(b, a) == d);
        CHECK(distance_sqr(b, a) == d2);
    }
    {
        // 1D
        auto a = uniform(rng, rBox1);
        auto dir = normalize(tg::vec1(uniform(rng, rBox1)));
        auto rd = uniform(rng, rBox1).x;

        auto b = a + dir * rd;
        CHECK(distance(a, b) == nx::approx(tg::abs(rd)));

        auto d2 = distance_sqr(a, b);
        auto d = distance(a, b);
        CHECK(d * d == nx::approx(d2));
        CHECK(d == nx::approx(tg::sqrt(d2)));
        // symmetry
        CHECK(distance(b, a) == d);
        CHECK(distance_sqr(b, a) == d2);
    }

    // distance to origin
    {
        auto p = uniform(rng, rBox1);
        auto d = distance_to_origin(p);
        CHECK(d == nx::approx(tg::length(tg::vec1(p))));
    }
    {
        auto p = uniform(rng, rBox2);
        auto d = distance_to_origin(p);
        CHECK(d == nx::approx(tg::length(tg::vec2(p))));
    }
    {
        auto p = uniform(rng, rBox3);
        auto d = distance_to_origin(p);
        CHECK(d == nx::approx(tg::length(tg::vec3(p))));
    }


    // plane and pos
    {
        // random plane
        auto pl = tg::plane(normalize(tg::vec3(uniform(rng, rBox3))), uniform(rng, rBox1).x);
        // random point
        auto pt = uniform(rng, rBox3);

        // symmetry
        auto d2 = distance_sqr(pt, pl);
        auto d = distance(pt, pl);
        CHECK(d == nx::approx(distance(pl, pt)));
        CHECK(d2 == nx::approx(distance_sqr(pl, pt)));

        CHECK(d * d == nx::approx(d2));
        CHECK(d == nx::approx(tg::sqrt(d2)));

        // move point on plane
        auto o = tg::pos3(pl.dis * pl.normal);
        auto t = pl.normal;
        while (dot(t, pl.normal) != 0.0f)
        {
            auto r = normalize(tg::vec3(uniform(rng, rBox3)));
            t = normalize(cross(pl.normal, r));
        }
        pt = o + t; // origin + tangent
        CHECK(distance(pt, pl) == nx::approx(0.0f).abs(0.001f));

        auto l = uniform(rng, rBox1).x;
        pt += pl.normal * l; // move along normal
        CHECK(distance(pt, pl) == nx::approx(tg::abs(l)));
    }


    // inf_cone and pos
    {
        auto icone = tg::inf_cone_boundary<3, float>(tg::pos3(uniform(rng, rBox3)), normalize(tg::vec3(uniform(rng, rBox3))),
                                                     uniform(rng, tg::angle::from_degree(1.f), tg::angle::from_degree(179.f)));
        auto l = uniform(rng, 0.1f, 2.f);
        auto pt = icone.apex - l * icone.opening_dir;
        auto d2 = distance_sqr(pt, icone);
        auto d = distance(pt, icone);
        CHECK(d == nx::approx(l));
        CHECK(d * d == nx::approx(d2));
        CHECK(d == nx::approx(tg::sqrt(d2)));
        l = uniform(rng, 0.f, 10.f);
        pt = icone.apex + l * icone.opening_dir;
        d = distance(pt, icone);
        d2 = distance_sqr(pt, icone);
        CHECK(d > 0);
        CHECK(d2 > 0);
        l = uniform(rng, 0.f, 10.f);
        auto r = tg::tan(icone.opening_angle / 2) * l;
        auto rand_dir = normalize(tg::vec3(uniform(rng, rBox3)));
        auto rand_n = normalize(cross(rand_dir, icone.opening_dir));
        auto ortho_dir = normalize(cross(rand_n, icone.opening_dir));
        pt = icone.apex + l * icone.opening_dir + r * ortho_dir;
        d = distance(pt, icone);
        d2 = distance_sqr(pt, icone);
        CHECK(d == nx::approx(0.f).abs(0.1));
        CHECK(d * d == nx::approx(d2));
        CHECK(d == nx::approx(tg::sqrt(d2)));
        d = distance(icone.apex, icone);
        CHECK(d == nx::approx(0.f).abs(0.1));
        pt = {2, -4, 0};
        icone = tg::inf_cone_boundary<3, float>({0, 0, 0}, {0, 0, 1}, tg::angle::from_degree(90));
        d2 = distance_sqr(pt, icone);
        CHECK(d2 == nx::approx(10.f));
        l = uniform(rng, 0.f, 10.f);
        pt = icone.apex + l * icone.opening_dir;
        d = distance(pt, icone);
        CHECK(d == nx::approx(l / tg::sqrt(2.f)));
    }


    // sphere and pos
    {
        auto sp = tg::sphere_boundary<3, tg::f32>(tg::pos3(uniform(rng, rBox3)), uniform(rng, 0.f, 10.f));
        auto l = uniform(rng, 0.f, 10.f);
        auto rand_dir = normalize(tg::vec3(uniform(rng, rBox3)));
        auto pt = sp.center + l * rand_dir;
        auto d2 = distance_sqr(pt, sp);
        auto d = distance(pt, sp);
        CHECK(d == nx::approx(tg::abs(l - sp.radius)));
        CHECK(d * d == nx::approx(d2));
        CHECK(d == nx::approx(tg::sqrt(d2)));
        sp = tg::sphere_boundary<3, tg::f32>(tg::pos3(uniform(rng, rBox3)), uniform(rng, 0.f, 10.f));
        pt = sp.center + sp.radius * rand_dir;
        d = distance(pt, sp);
        CHECK(d == nx::approx(0.f).abs(0.1));
        auto sp2 = tg::sphere_boundary<2, tg::f32>(tg::pos2(uniform(rng, rBox2)), uniform(rng, 0.f, 10.f));
        l = uniform(rng, 0.f, 10.f);
        auto rand_dir2 = normalize(tg::vec2(uniform(rng, rBox2)));
        auto pt2 = sp2.center + l * rand_dir2;
        d2 = distance_sqr(pt2, sp2);
        d = distance(pt2, sp2);
        CHECK(d == nx::approx(tg::abs(l - sp2.radius)));
        CHECK(d * d == nx::approx(d2));
        CHECK(d == nx::approx(tg::sqrt(d2)));
    }


    // inf_cylinder and pos
    {
        auto itube = tg::inf_cylinder_boundary<3, tg::f32>({tg::pos3(uniform(rng, rBox3)), normalize(tg::vec3(uniform(rng, rBox3)))}, uniform(rng, 0.f, 10.f));
        auto rand_dir = normalize(tg::vec3(uniform(rng, rBox3)));
        auto rand_n = normalize(cross(rand_dir, itube.axis.dir));
        auto ortho_dir = tg::normalize(tg::cross(rand_n, itube.axis.dir));
        auto l = uniform(rng, -10.f, 10.f);
        auto r = uniform(rng, 0.f, 10.f);
        auto pt = itube.axis[l] + r * ortho_dir;
        auto d2 = distance_sqr(pt, itube);
        auto d = distance(pt, itube);
        CHECK(d == nx::approx(tg::abs(r - itube.radius)));
        CHECK(d * d == nx::approx(d2));
        CHECK(d == nx::approx(tg::sqrt(d2)));

        auto itube2 = tg::inf_cylinder_boundary<2, tg::f32>({tg::pos2(uniform(rng, rBox2)), normalize(tg::vec2(uniform(rng, rBox2)))}, uniform(rng, 0.f, 10.f));
        tg::vec2 ortho_dir2 = {-itube2.axis.dir[1], itube2.axis.dir[0]};
        l = uniform(rng, -10.f, 10.f);
        r = uniform(rng, 0.f, 10.f);
        auto pt2 = itube2.axis[l] + r * ortho_dir2;
        d2 = distance_sqr(pt2, itube2);
        d = distance(pt2, itube2);
        CHECK(d == nx::approx(tg::abs(r - itube2.radius)));
        CHECK(d * d == nx::approx(d2));
        CHECK(d == nx::approx(tg::sqrt(d2)));
    }

    // cylinder and pos
    {
        auto c = tg::cylinder_boundary<3, float>({-1, -2, -3}, {5, -2, -3}, 2);
        CHECK(distance(tg::pos3(-1, -2, -3), c) == nx::approx(0.f).abs(0.001f));
        CHECK(distance(tg::pos3(5, -2, -3), c) == nx::approx(0.f).abs(0.001f));
        CHECK(distance(tg::pos3(0, -2, -3), c) == nx::approx(1.f));
        CHECK(distance(tg::pos3(2, -2, -3), c) == nx::approx(2.f));
        CHECK(distance(tg::pos3(-1, -3, -3), c) == nx::approx(0.f).abs(0.001f));
    }
}

namespace
{
// NOTE: f is (float&, float&) -> float
//       and might clamp its inputs
template <class F>
float minimize_f1(tg::rng& rng, float step_size, F&& f, int steps = 1500)
{
    auto d_min = tg::max<float>();
    auto t0 = uniform(rng, -10.f, 10.f);
    auto t1 = uniform(rng, -10.f, 10.f);

    for (auto i = 0; i < steps; ++i)
    {
        auto tt0 = t0 + uniform(rng, -step_size, step_size);
        auto tt1 = t1 + uniform(rng, -step_size, step_size);

        auto dd = f(tt0, tt1);
        if (dd < d_min)
        {
            t0 = tt0;
            t1 = tt1;
            d_min = dd;
        }
    }

    return d_min;
}
}

// TODO: is flaky
FUZZ_TEST("Distance - LineLine3", disabled)(tg::rng& rng)
{
    auto bb = tg::aabb3(-10, 10);

    auto l0 = tg::line3::from_points(uniform(rng, bb), uniform(rng, bb));
    auto l1 = tg::line3::from_points(uniform(rng, bb), uniform(rng, bb));

    auto d = distance(l0, l1);

    auto dd = minimize_f1(rng, 2.0f, [&](float a, float b) { return distance(l0[a], l1[b]); });

    CHECK(d == nx::approx(dd).abs(0.7f));
}

FUZZ_TEST("Distance - PosSeg2")(tg::rng& rng)
{
    auto bb = tg::aabb2(-10, 10);

    auto s = tg::segment2(uniform(rng, bb), uniform(rng, bb));
    auto p = uniform(rng, bb);

    auto d = distance(s, p);

    auto dd = minimize_f1(rng, 0.5f,
                          [&](float& a, float)
                          {
                              a = tg::saturate(a);
                              return distance(s[a], p);
                          });

    CHECK(d == nx::approx(dd).abs(0.7f));
}

FUZZ_TEST("Distance - SegSeg2")(tg::rng& rng)
{
    auto bb = tg::aabb2(-10, 10);

    auto s0 = tg::segment2(uniform(rng, bb), uniform(rng, bb));
    auto s1 = tg::segment2(uniform(rng, bb), uniform(rng, bb));

    auto d = distance(s0, s1);

    auto dd = minimize_f1(rng, 0.5f,
                          [&](float& a, float& b)
                          {
                              a = tg::saturate(a);
                              b = tg::saturate(b);
                              return distance(s0[a], s1[b]);
                          });

    if (d > 0)
        CHECK(!intersects(s0, s1));

    CHECK(d == nx::approx(dd).abs(0.9f));
}

FUZZ_TEST("Distance - SegSeg3")(tg::rng& rng)
{
    auto bb = tg::aabb3(-10, 10);

    auto s0 = tg::segment3(uniform(rng, bb), uniform(rng, bb));
    auto s1 = tg::segment3(uniform(rng, bb), uniform(rng, bb));

    auto d = distance(s0, s1);

    auto dd = minimize_f1(rng, 0.5f,
                          [&](float& a, float& b)
                          {
                              a = tg::saturate(a);
                              b = tg::saturate(b);
                              return distance(s0[a], s1[b]);
                          });

    CHECK(d == nx::approx(dd).abs(0.9f));
}

FUZZ_TEST("Distance - PosTri3")(tg::rng& rng)
{
    auto bb = tg::aabb3(-10, 10);

    auto t = tg::triangle3(uniform(rng, bb), uniform(rng, bb), uniform(rng, bb));
    auto p = uniform(rng, bb);

    auto d = distance(t, p);

    auto r = uniform(rng, 0.f, 4.f);

    auto pp = p + tg::uniform<tg::dir3>(rng) * r;

    CHECK(distance(pp, t) >= d - r);
}

FUZZ_TEST("Distance - TriangleAABB3")(tg::rng& rng)
{
    auto bounds = tg::aabb3(-10, 10);
    auto bb = tg::aabb_of(uniform(rng, bounds), uniform(rng, bounds));
    auto tri = tg::triangle3(uniform(rng, bounds), uniform(rng, bounds), uniform(rng, bounds));

    auto d = distance(bb, tri);

    if (intersects(bb, tri))
        CHECK(d == 0);

    for (auto i = 0; i < 100; ++i)
    {
        auto p = uniform(rng, bb);
        CHECK(d <= distance(p, tri));

        auto tp = uniform(rng, tri);
        CHECK(d <= distance(tp, bb));
    }

    if (d > 0)
    {
        auto p = uniform(rng, tri) + tg::uniform<tg::dir3>(rng) * uniform(rng, 0.f, d);
        CHECK(!contains(bb, p));
    }
}

FUZZ_TEST("Distance - AABBAABB3")(tg::rng& rng)
{
    auto bounds = tg::aabb3(-10, 10);
    auto bbA = tg::aabb_of(uniform(rng, bounds), uniform(rng, bounds));
    auto bbB = tg::aabb_of(uniform(rng, bounds), uniform(rng, bounds));

    auto dis = distance(bbA, bbB);
    CHECK(dis >= 0);

    // zero dis means they intersect
    if (dis == 0)
        CHECK(intersects(bbA, bbB));
    else // otherwise they dont
        CHECK(!intersects(bbA, bbB));

    // dis is a lower bound of all point-wise distances
    auto pA = uniform(rng, bbA);
    auto pB = uniform(rng, bbB);
    CHECK(distance(pA, pB) >= nx::approx(dis));
}
