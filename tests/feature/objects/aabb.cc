#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>

FUZZ_TEST("AABB")(tg::rng& rng)
{
    auto p0 = uniform(rng, tg::aabb3(tg::pos3(-10.0f), tg::pos3(10.0f)));
    auto p1 = uniform(rng, tg::aabb3(tg::pos3(-10.0f), tg::pos3(10.0f)));

    auto b = aabb_of(p0, p1);

    CHECK(b.max.x >= p0.x);
    CHECK(b.max.y >= p0.y);
    CHECK(b.max.z >= p0.z);
    CHECK(b.max.x >= p1.x);
    CHECK(b.max.y >= p1.y);
    CHECK(b.max.z >= p1.z);

    CHECK(b.min.x <= p0.x);
    CHECK(b.min.y <= p0.y);
    CHECK(b.min.z <= p0.z);
    CHECK(b.min.x <= p1.x);
    CHECK(b.min.y <= p1.y);
    CHECK(b.min.z <= p1.z);
}

FUZZ_TEST("ObjectAABB")(tg::rng& rng)
{;
    auto const test_obj = [&](auto const& obj) {
        auto bb = aabb_of(obj);
        auto p = uniform(rng, obj);
        CHECK(contains(bb, p));
    };


    auto const r = uniform(rng, 0.0f, 10.0f);
    auto const h = uniform(rng, 0.0f, 10.0f);
    auto const n1 = tg::dir(uniform(rng, tg::sphere_boundary<1, float>::unit));
    auto const n2 = tg::dir(uniform(rng, tg::sphere_boundary<2, float>::unit));
    auto const n3 = tg::dir(uniform(rng, tg::sphere_boundary<3, float>::unit));

    auto const range1 = tg::aabb1(-10,10);
    auto const range2 = tg::aabb2(-10,10);
    auto const range3 = tg::aabb3(-10,10);
    auto const range4 = tg::aabb4(-10,10);

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

    auto d1 = tg::uniform<tg::dir1>(rng);
    auto m1 = tg::mat1();
    m1[0] = d1 * uniform(rng, 1.0f, 3.0f);

    auto d20 = tg::uniform<tg::dir2>(rng);
    auto d21 = perpendicular(d20);
    auto m2 = tg::mat2();
    m2[0] = d20 * uniform(rng, 1.0f, 3.0f);
    m2[1] = d21 * uniform(rng, 1.0f, 3.0f);

    auto d30 = tg::uniform<tg::dir3>(rng);
    auto d31 = any_normal(d30);
    auto d32 = normalize(cross(d30, d31));
    auto m3 = tg::mat3();
    m3[0] = d30 * uniform(rng, 1.0f, 3.0f);
    m3[1] = d31 * uniform(rng, 1.0f, 3.0f);
    m3[2] = d32 * uniform(rng, 1.0f, 3.0f);

    auto m23 = tg::mat2x3();
    m23[0] = d30 * uniform(rng, 1.0f, 3.0f);
    m23[1] = d31 * uniform(rng, 1.0f, 3.0f);

    // aabb
    test_obj(aabb_of(pos10, pos11));
    test_obj(aabb_of(pos20, pos21));
    test_obj(aabb_of(pos30, pos31));
    test_obj(aabb_of(pos40, pos41));

    // box
    test_obj(tg::box1(pos10, m1));
    test_obj(tg::box2(pos20, m2));
    test_obj(tg::box3(pos30, m3));
    // TODO: box4
    test_obj(tg::box2in3(pos30, m23));

    // capsule
    test_obj(tg::capsule3(axis0, r));
    // cylinder
    test_obj(tg::cylinder3(axis0, r));
    // ellipse
    test_obj(tg::ellipse1(pos10, m1));
    test_obj(tg::ellipse2(pos20, m2));
    test_obj(tg::ellipse3(pos30, m3));
    // TODO: ellipse4
    test_obj(tg::ellipse2in3(pos30, m23));
    // hemisphere
    test_obj(tg::hemisphere1(pos10, r, n1));
    test_obj(tg::hemisphere2(pos20, r, n2));
    test_obj(tg::hemisphere3(pos30, r, n3));
    // test_obj(p4, tg::hemisphere4(pos40, r, n4));
    // pyramid
    test_obj(tg::pyramid<tg::box2in3>(tg::box2in3(pos30, m23), h));
    //test_obj(tg::pyramid<tg::quad3>(tg::quad3(pos30, pos31, pos32, pos32 + (pos30 - pos31)), h)); // TODO: uniform(quad) missing
    test_obj(tg::pyramid<tg::sphere2in3>(tg::sphere2in3(pos30, r, n3), h)); // == cone
    test_obj(tg::pyramid<tg::triangle3>(tg::triangle3(pos30, pos31, pos32), h));
    // TODO: test for quad require uniform(quad)
    //test_obj(tg::quad2(pos20, pos21, pos22, pos23));
    //test_obj(tg::quad3(pos30, pos31, pos32, pos33));
    //test_obj(tg::quad4(pos40, pos41, pos42, pos43));
    // segment
    test_obj(tg::segment1(pos10, pos11));
    test_obj(tg::segment2(pos20, pos21));
    test_obj(tg::segment3(pos30, pos31));
    test_obj(tg::segment4(pos40, pos41));
    // sphere
    test_obj(tg::sphere1(pos10, r));
    test_obj(tg::sphere2(pos20, r));
    test_obj(tg::sphere3(pos30, r));
    test_obj(tg::sphere4(pos40, r));
    test_obj(tg::sphere<1, float, 2>(pos20, r, n2));
    test_obj(tg::sphere2in3(pos30, r, n3));
    // triangle
    test_obj(tg::triangle2(pos20, pos21, pos22));
    test_obj(tg::triangle3(pos30, pos31, pos32));
    test_obj(tg::triangle4(pos40, pos41, pos42));
}
