#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/vector.hh>
#include <typed-geometry/feature/objects.hh>

// FIXME: flaky right now
FUZZ_TEST("AnyPoint", disabled)(tg::rng& rng)
{
    auto const tolerance = 0.015f;
    auto const test_obj = [tolerance](auto const& o) {
        auto p = any_point(o);
        CHECK(contains(o, p, tolerance));
    };

    auto const test_obj_and_boundary = [&test_obj](auto const& o) {
        test_obj(o);
        test_obj(boundary_of(o));
    };

    auto const test_obj_and_boundary_no_caps = [&test_obj](auto const& o) {
        test_obj(o);
        test_obj(boundary_of(o));
        test_obj(boundary_no_caps_of(o));
    };

    auto const r = uniform(rng, 0.0f, 10.0f);
    auto const h = uniform(rng, 0.0f, 10.0f);
    auto const a = tg::uniform<tg::angle>(rng);
    auto const n1 = tg::uniform<tg::dir1>(rng);
    auto const n2 = tg::uniform<tg::dir2>(rng);
    auto const n3 = tg::uniform<tg::dir3>(rng);
    auto const n4 = tg::uniform<tg::dir4>(rng);

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
    test_obj_and_boundary(aabb_of(pos10, pos11));
    test_obj_and_boundary(aabb_of(pos20, pos21));
    test_obj_and_boundary(aabb_of(pos30, pos31));
    test_obj_and_boundary(aabb_of(pos40, pos41));
    // box
    test_obj_and_boundary(tg::box1(pos10, m1));
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
    // halfspace
    test_obj(tg::halfspace1(n1, h));
    test_obj(tg::halfspace2(n2, h));
    test_obj(tg::halfspace3(n3, h));
    test_obj(tg::halfspace4(n4, h));
    // hemisphere
    test_obj_and_boundary_no_caps(tg::hemisphere1(pos10, r, n1));
    test_obj_and_boundary_no_caps(tg::hemisphere2(pos20, r, n2));
    test_obj_and_boundary_no_caps(tg::hemisphere3(pos30, r, n3));
    // test_obj_and_boundary_no_caps(tg::hemisphere4(pos40, r, n4));
    // inf_cone
    test_obj_and_boundary(tg::inf_cone2(pos20, n2, a));
    test_obj_and_boundary(tg::inf_cone3(pos30, n3, a));
    // inf_cylinder
    test_obj_and_boundary(tg::inf_cylinder2(tg::line2(pos20, n2), r));
    test_obj_and_boundary(tg::inf_cylinder3(tg::line3(pos30, n3), r));
    // line
    test_obj(tg::line1(pos10, n1));
    test_obj(tg::line2(pos20, n2));
    test_obj(tg::line3(pos30, n3));
    test_obj(tg::line4(pos40, n4));
    // plane
    test_obj(tg::plane1(n1, h));
    test_obj(tg::plane2(n2, h));
    test_obj(tg::plane3(n3, h));
    test_obj(tg::plane4(n4, h));
    // pyramid
    test_obj_and_boundary_no_caps(tg::pyramid<tg::box2in3>(tg::box2in3(pos30, m23), h));
    test_obj_and_boundary_no_caps(tg::pyramid<tg::sphere2in3>(disk0, h)); // == cone
    test_obj_and_boundary_no_caps(tg::pyramid<tg::triangle3>(tg::triangle3(pos30, pos31, pos32), h));
    test_obj(tg::pyramid_boundary_no_caps<tg::quad3>(tg::quad3(pos30, pos31, pos32, pos32 + (pos30 - pos31)), h));
    // test_obj_and_boundary_no_caps(tg::pyramid<tg::quad3>(tg::quad3(pos30, pos31, pos32, pos32 + (pos30 - pos31)), h));
    // TODO: quad
    // test_obj(tg::quad2(pos20, pos21, pos22, pos23));
    // test_obj(tg::quad3(pos30, pos31, pos32, pos32 + (pos31 - pos30)));
    // test_obj(tg::quad4(pos40, pos41, pos42, pos42 + (pos41 - pos40)));
    // ray
    test_obj(tg::ray1(pos10, n1));
    test_obj(tg::ray2(pos20, n2));
    test_obj(tg::ray3(pos30, n3));
    test_obj(tg::ray4(pos40, n4));
    // segment
    test_obj(tg::segment1(pos10, pos11));
    test_obj(tg::segment2(pos20, pos21));
    test_obj(tg::segment3(pos30, pos31));
    test_obj(tg::segment4(pos40, pos41));
    // sphere
    test_obj_and_boundary(tg::sphere1(pos10, r));
    test_obj_and_boundary(tg::sphere2(pos20, r));
    test_obj_and_boundary(tg::sphere3(pos30, r));
    test_obj_and_boundary(tg::sphere4(pos40, r));
    test_obj_and_boundary(tg::sphere<1, float, 2>(pos20, r, n2));
    test_obj_and_boundary(tg::sphere2in3(pos30, r, n3));
    // triangle
    test_obj(tg::triangle2(pos20, pos21, pos22));
    test_obj(tg::triangle3(pos30, pos31, pos32));
    // test_obj(tg::triangle4(pos40, pos41, pos42));
}
