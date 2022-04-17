#include <nexus/ext/tg-approx.hh>
#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>

FUZZ_TEST("Centroid")(tg::rng& rng)
{
    auto range1 = tg::aabb1(tg::pos1(-10), tg::pos1(10));
    auto range2 = tg::aabb2(tg::pos2(-10), tg::pos2(10));
    auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));

    {
        auto b = aabb_of(uniform(rng, range3), uniform(rng, range3));
        CHECK(contains(b, centroid_of(b)));
    }
    {
        auto b = aabb_of(uniform(rng, range2), uniform(rng, range2));
        CHECK(contains(b, centroid_of(b)));
    }
    {
        auto t = tg::triangle2(uniform(rng, range2), uniform(rng, range2), uniform(rng, range2));
        auto const center = centroid_of(t);
        CHECK(contains(t, center));
        auto const transl = uniform_vec(rng, range2);

        t.pos0.x += transl.x;
        t.pos0.y += transl.y;
        t.pos1.x += transl.x;
        t.pos1.y += transl.y;
        t.pos2.x += transl.x;
        t.pos2.y += transl.y;

        auto const newCenter = center + transl;

        CHECK(contains(t, newCenter));
        CHECK(centroid_of(t) == nx::approx(newCenter));
    }


    {
        // 3d sphere
        auto radius = uniform(rng, range1).x;
        auto sphere = tg::sphere3(uniform(rng, range3), radius);
        auto fullTurn = tg::pi<float>.radians() * 2;

        auto center = centroid_of(sphere);

        for (auto s = 0.0f; s < fullTurn; s += 0.01f)
        {
            for (auto t = 0.0f; t < fullTurn; t += 0.01f)
            {
                auto surfacePoint = sphere.center + tg::vec3(tg::f32(cos(s) * sin(t) * radius), tg::f32(sin(s) * sin(t) * radius), tg::f32(cos(t) * radius));

                CHECK(tg::distance_sqr(center, surfacePoint) == nx::approx(tg::pow(radius, 2)));
            }
        }
    }

    {
        // tetrahedron
        auto const pyTri
            = tg::pyramid<tg::triangle3>(tg::triangle3(uniform(rng, range3), uniform(rng, range3), uniform(rng, range3)), uniform(rng, 0.1f, 10.f));
        auto center = apex_of(pyTri);
        for (auto const& vertex : vertices_of(pyTri.base))
            center += vertex;
        center /= 4.f;
        auto const centroid = centroid_of(pyTri);
        CHECK(centroid == nx::approx(center));
    }
}

FUZZ_TEST("CentroidByUniform")(tg::rng& rng)
{
    auto const numSamples = 1500;

    auto const test_obj = [&rng, numSamples](auto const& o)
    {
        auto center = uniform(rng, o);
        for (auto i = 1; i < numSamples; ++i)
            center += uniform(rng, o);
        center /= numSamples;
        auto const centroid = centroid_of(o);
        // NOTE: really high tolerance because number of samples is kinda low
        CHECK(centroid == nx::approx(center).abs(1.5f).rel(0.2f));
    };

    auto const test_obj_and_boundary = [&test_obj](auto const& o)
    {
        test_obj(o);
        test_obj(boundary_of(o));
    };

    auto const test_obj_and_boundary_no_caps = [&test_obj](auto const& o)
    {
        test_obj(o);
        test_obj(boundary_of(o));
        test_obj(boundary_no_caps_of(o));
    };

    auto const r = uniform(rng, 0.0f, 10.0f);
    auto const h = uniform(rng, 0.0f, 10.0f);
    auto const n2 = tg::uniform<tg::dir2>(rng);
    auto const n3 = tg::uniform<tg::dir3>(rng);

    auto const range1 = tg::aabb1(tg::pos1(-10), tg::pos1(10));
    auto const range2 = tg::aabb2(tg::pos2(-10), tg::pos2(10));
    auto const range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));
    auto const range4 = tg::aabb4(tg::pos4(-10), tg::pos4(10));

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
    auto const pos42 = uniform(rng, range4);

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
    test_obj(aabb_of(pos10, pos11));
    test_obj_and_boundary(aabb_of(pos20, pos21));
    test_obj_and_boundary(aabb_of(pos30, pos31));
    test_obj_and_boundary(aabb_of(pos40, pos41));
    // box
    test_obj(tg::box1(pos10, m1));
    test_obj_and_boundary(tg::box2(pos20, m2));
    test_obj_and_boundary(tg::box3(pos30, m3));
    // TODO: box4
    test_obj_and_boundary(tg::box2in3(pos30, m23));
    // capsule
    test_obj_and_boundary(tg::capsule3(axis0, r));
    // cylinder
    test_obj_and_boundary_no_caps(tg::cylinder3(axis0, r));
    // ellipse
    test_obj_and_boundary(tg::ellipse1(pos10, m1));
    test_obj_and_boundary(tg::ellipse2(pos20, m2));
    test_obj_and_boundary(tg::ellipse3(pos30, m3));
    // TODO: ellipse4
    test_obj_and_boundary(tg::ellipse2in3(pos30, m23));
    // hemisphere
    // test_obj_and_boundary_no_caps(tg::hemisphere1(pos10, r, n1));
    test_obj_and_boundary_no_caps(tg::hemisphere2(pos20, r, n2));
    test_obj_and_boundary_no_caps(tg::hemisphere3(pos30, r, n3));
    // test_obj_and_boundary_no_caps(tg::hemisphere4(pos40, r, n4));
    // pyramid
    test_obj_and_boundary_no_caps(tg::pyramid<tg::box2in3>(tg::box2in3(pos30, m23), h));
    test_obj_and_boundary_no_caps(tg::pyramid<tg::sphere2in3>(disk0, h)); // == cone
    test_obj_and_boundary_no_caps(tg::pyramid<tg::triangle3>(tg::triangle3(pos30, pos31, pos32), h));
    // test_obj_and_boundary_no_caps(tg::pyramid<tg::quad3>(tg::quad3(pos30, pos31, pos32, pos32 + (pos30 - pos31)), h)); // TODO: area(quad) missing
    test_obj(tg::pyramid_boundary_no_caps<tg::quad3>(tg::quad3(pos30, pos31, pos32, pos32 + (pos30 - pos31)), h));
    // TODO: quad
    // segment
    test_obj(tg::segment1(pos10, pos11));
    test_obj(tg::segment2(pos20, pos21));
    test_obj(tg::segment3(pos30, pos31));
    test_obj(tg::segment4(pos40, pos41));
    // sphere
    test_obj(tg::sphere1(pos10, r));
    test_obj_and_boundary(tg::sphere2(pos20, r));
    test_obj_and_boundary(tg::sphere3(pos30, r));
    test_obj_and_boundary(tg::sphere4(pos40, r));
    test_obj_and_boundary(tg::sphere<1, float, 2>(pos20, r, n2));
    test_obj_and_boundary(tg::sphere2in3(pos30, r, n3));
    // triangle
    test_obj(tg::triangle2(pos20, pos21, pos22));
    test_obj(tg::triangle3(pos30, pos31, pos32));
    test_obj(tg::triangle4(pos40, pos41, pos42));
}
