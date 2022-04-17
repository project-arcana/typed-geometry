#include <nexus/fuzz_test.hh>

#include <typed-geometry/tg-std.hh>

FUZZ_TEST("Project")(tg::rng& rng)
{
    auto const test_obj = [&rng](auto const& p, auto const& o)
    {
        auto proj = project(p, o);

        // Projected point lies in the object
        auto dist = distance_sqr(proj, o);
        CHECK(dist == nx::approx(0.0f).abs(0.001f));
        CHECK(contains(o, proj, 0.01f));

        dist = distance_sqr(proj, p);
        if (contains(o, p)) // If inside by chance, the projection does not change anything
            CHECK(dist == nx::approx(0.0f).abs(0.001f));
        else // Otherwise all other points inside are not closer than the projection
            for (auto i = 0; i < 256; i++)
            {
                auto q = uniform(rng, o);
                CHECK(distance_sqr(q, p) >= nx::approx(dist).abs(0.01f));
            }

        // Projection of points already sampled from inside changes nothing
        auto pInside = uniform(rng, o);
        proj = project(pInside, o);
        dist = distance_sqr(proj, pInside);
        CHECK(dist == nx::approx(0.0f).abs(0.001f));
    };

    auto const test_obj_and_boundary = [&test_obj](auto const& p, auto const& o)
    {
        test_obj(p, o);
        test_obj(p, boundary_of(o));
    };

    auto const test_obj_and_boundary_no_caps = [&test_obj](auto const& p, auto const& o)
    {
        test_obj(p, o);
        test_obj(p, boundary_of(o));
        test_obj(p, boundary_no_caps_of(o));
    };


    auto const r = uniform(rng, 0.0f, 10.0f);
    auto const h = uniform(rng, 0.0f, 10.0f);
    auto const n2 = tg::uniform<tg::dir2>(rng);
    auto const n3 = tg::uniform<tg::dir3>(rng);

    auto const range1 = tg::aabb1(tg::pos1(-10), tg::pos1(10));
    auto const range2 = tg::aabb2(tg::pos2(-10), tg::pos2(10));
    auto const range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));
    auto const range4 = tg::aabb4(tg::pos4(-10), tg::pos4(10));

    auto const p1 = uniform(rng, range1);
    auto const p2 = uniform(rng, range2);
    auto const p3 = uniform(rng, range3);
    auto const p4 = uniform(rng, range4);

    auto const pos10 = uniform(rng, range1);
    auto const pos11 = uniform(rng, range1);

    auto const pos20 = uniform(rng, range2);
    auto const pos21 = uniform(rng, range2);
    auto const pos22 = uniform(rng, range2);

    auto const pos30 = uniform(rng, range3);
    auto const pos31 = uniform(rng, range3);
    auto const pos32 = uniform(rng, range3);

    auto const pos40 = uniform(rng, range4);
    auto const pos41 = uniform(rng, range4);

    auto const axis0 = tg::segment3(pos30, pos31);
    auto const disk0 = tg::sphere2in3(pos30, r, n3);

    auto const d1 = tg::uniform<tg::dir1>(rng);
    auto m1 = tg::mat1();
    m1[0] = d1 * uniform(rng, 1.0f, 3.0f);

    auto const d20 = tg::uniform<tg::dir2>(rng);
    auto const d21 = perpendicular(d20);
    auto m2 = tg::mat2();
    m2[0] = d20 * uniform(rng, 1.0f, 3.0f);
    m2[1] = d21 * uniform(rng, 1.0f, 3.0f);

    auto const d30 = tg::uniform<tg::dir3>(rng);
    auto const d31 = any_normal(d30);
    auto const d32 = normalize(cross(d30, d31));
    auto m3 = tg::mat3();
    m3[0] = d30 * uniform(rng, 1.0f, 3.0f);
    m3[1] = d31 * uniform(rng, 1.0f, 3.0f);
    m3[2] = d32 * uniform(rng, 1.0f, 3.0f);

    auto m23 = tg::mat2x3();
    m23[0] = d30 * uniform(rng, 1.0f, 3.0f);
    m23[1] = d31 * uniform(rng, 1.0f, 3.0f);

    // aabb
    test_obj_and_boundary(p1, aabb_of(pos10, pos11));
    test_obj_and_boundary(p2, aabb_of(pos20, pos21));
    test_obj_and_boundary(p3, aabb_of(pos30, pos31));
    test_obj_and_boundary(p4, aabb_of(pos40, pos41));
    // box
    test_obj_and_boundary(p1, tg::box1(pos10, m1));
    test_obj_and_boundary(p2, tg::box2(pos20, m2));
    test_obj_and_boundary(p3, tg::box3(pos30, m3));
    // TODO: box4
    test_obj_and_boundary(p3, tg::box2in3(pos30, m23));

    // capsule
    test_obj_and_boundary(p3, tg::capsule3(axis0, r));
    // cylinder
    test_obj_and_boundary_no_caps(p3, tg::cylinder3(axis0, r));
    // hemisphere
    // TODO: And boundary no caps
    // test_obj_and_boundary_no_caps(p1, tg::hemisphere1(pos10, r, n1));
    test_obj_and_boundary_no_caps(p2, tg::hemisphere2(pos20, r, n2));
    test_obj_and_boundary_no_caps(p3, tg::hemisphere3(pos30, r, n3));
    // test_obj_and_boundary_no_caps(p4, tg::hemisphere4(pos40, r, n4));
    // pyramid
    test_obj_and_boundary_no_caps(p3, tg::pyramid<tg::box2in3>(tg::box2in3(pos30, m23), h));
    test_obj_and_boundary_no_caps(p3, tg::pyramid<tg::sphere2in3>(disk0, h)); // == cone
    test_obj_and_boundary_no_caps(p3, tg::pyramid<tg::triangle3>(tg::triangle3(pos30, pos31, pos32), h));
    // test_obj_and_boundary_no_caps(p3, tg::pyramid<tg::quad3>(tg::quad3(pos30, pos31, pos32, pos32 + (pos30 - pos31)), h)); // TODO: project(quad) missing
    test_obj(p3, tg::pyramid_boundary_no_caps<tg::quad3>(tg::quad3(pos30, pos31, pos32, pos32 + (pos30 - pos31)), h));
    // TODO: quad
    // segment
    test_obj(p1, tg::segment1(pos10, pos11));
    test_obj(p2, tg::segment2(pos20, pos21));
    test_obj(p3, tg::segment3(pos30, pos31));
    test_obj(p4, tg::segment4(pos40, pos41));
    // sphere
    test_obj_and_boundary(p1, tg::sphere1(pos10, r));
    test_obj_and_boundary(p2, tg::sphere2(pos20, r));
    test_obj_and_boundary(p3, tg::sphere3(pos30, r));
    test_obj_and_boundary(p4, tg::sphere4(pos40, r));
    test_obj_and_boundary(p2, tg::sphere<1, float, 2>(pos20, r, n2));
    test_obj_and_boundary(p3, tg::sphere2in3(pos30, r, n3));
    // triangle
    test_obj(p2, tg::triangle2(pos20, pos21, pos22));
    test_obj(p3, tg::triangle3(pos30, pos31, pos32));
    // test_obj(p4, tg::triangle4(pos40, pos41, pos42));
}

FUZZ_TEST("ProjectObjects")(tg::rng& rng)
{
    auto range1 = tg::aabb1(tg::pos1(-10), tg::pos1(10));
    auto range2 = tg::aabb2(tg::pos2(-10), tg::pos2(10));
    auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));

    TG_UNUSED(range2);

    {
        // plane
        auto n = normalize(uniform(rng, range3) - tg::zero<tg::pos3>());
        auto d = uniform(rng, -10.0f, 10.0f);
        auto pl = tg::plane(n, d);

        // point
        auto p = uniform(rng, range3);


        auto pp = project(p, pl);

        // distance initial point to plane
        auto dist0 = distance(p, pl);
        // distance initial point to point on plane
        auto dist1 = distance(p, pp);
        // distance point on plane to plane
        auto dist2 = distance(pp, pl);

        CHECK(dist0 == nx::approx(dist1));
        CHECK(dist2 == nx::approx(0.0f).abs(0.001f));
    }

    {
        // line
        auto pos = uniform(rng, range3);
        auto dir = tg::vec3();
        while (length_sqr(dir) == 0)
            dir = tg::vec3(uniform(rng, range3));
        auto line = tg::line3(pos, normalize(dir));

        auto s = uniform(rng, range1).x;

        // point
        auto p = pos + s * line.dir;

        // create vector which is orthogonal to line direction
        auto ortho = tg::vec3();
        while (length_sqr(ortho) == 0)
            ortho = cross(dir, tg::vec3(uniform(rng, range3)));

        // create second point "above" line
        auto pn = p + normalize(ortho);

        // project that point back, it should lie at p's position
        pn = project(pn, line);
        auto dist1 = distance_sqr(p, pn);
        CHECK(dist1 == nx::approx(0.0f).abs(0.001f));
    }

    {
        // ray
        auto pos = uniform(rng, range3);
        auto dir = tg::vec3();
        while (length_sqr(dir) == 0)
            dir = tg::vec3(uniform(rng, range3));
        auto dirN = normalize(dir);
        auto line = tg::line3(pos, dirN);
        auto ray = tg::ray3(pos, dirN);

        auto p = uniform(rng, range3);

        auto projLine = project(p, line);
        auto projRay = project(p, ray);

        // The projection onto the ray is the same as onto the line if in positive direction, otherwise at the ray origin
        auto diff = distance_sqr(projRay, dot(p - pos, dir) > 0 ? projLine : pos);
        CHECK(diff == nx::approx(0.0f).abs(0.001f));
    }
}
