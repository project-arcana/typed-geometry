#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>
#include <typed-geometry/feature/vector.hh>

namespace
{
enum class EnumTest
{
    A,
    B,
    C,
    D
};
}

TEST("Uniform API")
{
    tg::rng rng;

    CHECK(uniform(rng, 0.0f, 1.0f) >= 0);
    CHECK(uniform(rng, 0, 1) >= 0);
    CHECK(uniform(rng, {0, 1, 2}) >= 0);
    CHECK(uniform(rng, {EnumTest::A, EnumTest::B, EnumTest::C}) != EnumTest::D);
}

FUZZ_TEST("Uniform")(tg::rng& rng)
{
    auto a = uniform(rng, -10.0f, -1.0f);
    auto b = uniform(rng, 1.0f, 10.0f);
    auto r = uniform(rng, a, b);

    CHECK(a >= -10.0f);
    CHECK(a <= -1.0f);

    CHECK(b >= 1.0f);
    CHECK(b <= 10.0f);

    CHECK(r >= a);
    CHECK(r <= b);
}

FUZZ_TEST("UniformGeneralProperties")(tg::rng& rng)
{
    auto const tolerance = 0.05f;
    tg::u64 const sampleSize = 32;

    auto const test_obj = [&rng, tolerance](auto samples, auto const& o)
    {
        for (tg::u64 i = 0; i < samples.size(); ++i)
        {
            auto p = uniform(rng, o);
            CHECK(contains(o, p, tolerance));

            samples[i] = p;
        }
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


    auto samples1 = tg::array<tg::pos1, sampleSize>();
    auto samples2 = tg::array<tg::pos2, sampleSize>();
    auto samples3 = tg::array<tg::pos3, sampleSize>();
    auto samples4 = tg::array<tg::pos4, sampleSize>();

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
    test_obj(samples1, aabb_of(pos10, pos11 + 0.1f)); // Give some space to place different samples // aabb1_boundary consists of only 2 points and therefore collisions in the samples are unavoidable
    test_obj_and_boundary(samples2, aabb_of(pos20, pos21));
    test_obj_and_boundary(samples3, aabb_of(pos30, pos31));
    test_obj_and_boundary(samples4, aabb_of(pos40, pos41));
    // box
    test_obj(samples1, tg::box1(pos10, m1)); // box1_boundary consists of only 2 points and therefore collisions in the samples are unavoidable
    test_obj_and_boundary(samples2, tg::box2(pos20, m2));
    test_obj_and_boundary(samples3, tg::box3(pos30, m3));
    // TODO: box4
    test_obj_and_boundary(samples3, tg::box2in3(pos30, m23));

    // capsule
    test_obj_and_boundary(samples3, tg::capsule3(axis0, r));
    // cylinder
    test_obj_and_boundary_no_caps(samples3, tg::cylinder3(axis0, r));
    // ellipse
    test_obj(samples1, tg::ellipse1(pos10, m1)); // ellipse1_boundary consists of only 2 points and therefore collisions in the samples are unavoidable
    test_obj_and_boundary(samples2, tg::ellipse2(pos20, m2));
    test_obj_and_boundary(samples3, tg::ellipse3(pos30, m3));
    // TODO: ellipse4
    test_obj_and_boundary(samples3, tg::ellipse2in3(pos30, m23));
    // hemisphere
    // TODO: And boundary no caps
    // test_obj_and_boundary_no_caps(samples1, tg::hemisphere1(pos10, r, n1));
    test_obj_and_boundary_no_caps(samples2, tg::hemisphere2(pos20, r, n2));
    test_obj_and_boundary_no_caps(samples3, tg::hemisphere3(pos30, r, n3));
    // test_obj_and_boundary_no_caps(samples4, tg::hemisphere4(pos40, r, n4));
    // pyramid
    // FIXME: flaky right now
    // test_obj_and_boundary_no_caps(samples3, tg::pyramid<tg::box2in3>(tg::box2in3(pos30, m23), h));
    // FIXME: flaky right now
    // test_obj_and_boundary_no_caps(samples3, tg::pyramid<tg::sphere2in3>(disk0, h)); // == cone
    // FIXME: flaky right now
    // test_obj_and_boundary_no_caps(samples3, tg::pyramid<tg::triangle3>(tg::triangle3(pos30, pos31, pos32), h));
    // test_obj_and_boundary_no_caps(samples3, tg::pyramid<tg::quad3>(tg::quad3(pos30, pos31, pos32, pos32 + (pos30 - pos31)), h)); // TODO: uniform(quad) missing
    test_obj(samples3, tg::pyramid_boundary_no_caps<tg::quad3>(tg::quad3(pos30, pos31, pos32, pos32 + (pos30 - pos31)), h));
    // TODO: quad
    // segment
    test_obj(samples1, tg::segment1(pos10, pos11));
    test_obj(samples2, tg::segment2(pos20, pos21));
    test_obj(samples3, tg::segment3(pos30, pos31));
    test_obj(samples4, tg::segment4(pos40, pos41));
    // sphere
    test_obj(samples1, tg::sphere1(pos10, r)); // sphere1_boundary consists of only 2 points and therefore collisions in the samples are unavoidable
    test_obj_and_boundary(samples2, tg::sphere2(pos20, r));
    test_obj_and_boundary(samples3, tg::sphere3(pos30, r));
    test_obj_and_boundary(samples4, tg::sphere4(pos40, r));
    test_obj(samples2, tg::sphere<1, float, 2>(pos20, r, n2)); // sphere1in2_boundary consists of only 2 points and therefore collisions in the samples are unavoidable
    test_obj_and_boundary(samples3, tg::sphere2in3(pos30, r, n3));
    // triangle
    test_obj(samples2, tg::triangle2(pos20, pos21, pos22));
    test_obj(samples3, tg::triangle3(pos30, pos31, pos32));
    // test_obj(samples4, tg::triangle4(pos40, pos41, pos42));
}

FUZZ_TEST("UniformObjects")(tg::rng& rng)
{
    auto range2 = tg::aabb2(tg::pos2(-10), tg::pos2(10));
    auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));

    auto c2 = uniform(rng, range2);
    auto c3 = uniform(rng, range3);
    auto r = uniform(rng, 0.0f, 10.0f);
    auto n = tg::uniform<tg::dir3>(rng);
    auto n2 = tg::uniform<tg::dir2>(rng);

    {
        auto v = uniform_vec(rng, tg::sphere_boundary<3, float>::unit);
        CHECK(is_normalized(v));
    }
    {
        auto v = uniform_vec(rng, tg::sphere_boundary<3, float>::unit);
        CHECK(length(v) <= 1.00001f);
    }
    {
        auto circle = tg::sphere_boundary<2, float, 3>(c3, r, n);
        auto p1 = uniform(rng, circle);
        CHECK(distance(c3, p1) == nx::approx(r));
    }
    {
        auto circle = tg::sphere_boundary<2, float>(c2, r);
        auto p1 = uniform(rng, circle);
        CHECK(distance(c2, p1) == nx::approx(r));
    }
    {
        auto disk = tg::sphere2in3(c3, r, n);
        auto p1 = uniform(rng, disk);
        auto p2 = uniform(rng, disk);
        CHECK(distance(c3, p1) <= r);
        CHECK(distance(c3, p2) <= r);
        auto someInside = distance(c3, p1) < r || distance(c3, p2) < r;
        CHECK(someInside);
    }
    {
        auto disk = tg::sphere2(c2, r);
        auto p1 = uniform(rng, disk);
        auto p2 = uniform(rng, disk);
        CHECK(distance(c2, p1) <= r);
        CHECK(distance(c2, p2) <= r);
        auto someInside = distance(c2, p1) < r || distance(c2, p2) < r;
        CHECK(someInside);
    }
    {
        auto pos0 = uniform(rng, range3);
        auto pos1 = uniform(rng, range3);
        auto tube = tg::tube3(pos0, pos1, r);
        auto p1 = uniform(rng, tube);
        CHECK(distance(tube.axis, p1) == nx::approx(r));
    }
    {
        auto hemi = tg::hemisphere_boundary_no_caps<3, float>(c3, r, n);
        auto p1 = uniform(rng, hemi); // no caps
        CHECK(distance(hemi.center, p1) == nx::approx(r));
        CHECK(dot(p1 - c3, n) >= 0);
    }
    {
        auto hemi = tg::hemisphere_boundary_no_caps<2, float>(c2, r, n2);
        auto p1 = uniform(rng, hemi); // no caps
        CHECK(distance(hemi.center, p1) == nx::approx(r));
        CHECK(dot(p1 - c2, n2) >= 0);
    }
}

FUZZ_TEST("UniformBasic")(tg::rng& rng)
{
    auto c = uniform(rng, -2.0f, 2.0f);

    auto any_smaller = false;
    auto any_bigger = false;

    for (auto i = 0; i < 256; ++i)
    {
        auto v = uniform(rng, c - 3, c + 3);
        any_smaller |= v < c;
        any_bigger |= v > c;
    }

    CHECK(any_smaller);
    CHECK(any_bigger);
}

FUZZ_TEST("UniformBox")(tg::rng& rng)
{
    auto c = tg::pos3(2, 3, 4);
    auto b = tg::aabb3(c - 10.0f, c + 10.0f);

    auto any_smaller_cx = false;
    auto any_smaller_cy = false;
    auto any_smaller_cz = false;
    auto any_bigger_cx = false;
    auto any_bigger_cy = false;
    auto any_bigger_cz = false;

    for (auto i = 0; i < 256; ++i)
    {
        auto p = uniform(rng, b);
        any_smaller_cx |= p.x < c.x;
        any_smaller_cy |= p.y < c.y;
        any_smaller_cz |= p.z < c.z;
        any_bigger_cx |= p.x > c.x;
        any_bigger_cy |= p.y > c.y;
        any_bigger_cz |= p.z > c.z;
    }

    CHECK(any_bigger_cx);
    CHECK(any_bigger_cy);
    CHECK(any_bigger_cz);
    CHECK(any_smaller_cx);
    CHECK(any_smaller_cy);
    CHECK(any_smaller_cz);
}

FUZZ_TEST("UniformCylinder")(tg::rng& rng)
{
    auto tolerance = 0.01f;
    auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));
    auto r = uniform(rng, 3.0f, 10.0f);
    auto pos0 = uniform(rng, range3);
    auto pos1 = uniform(rng, range3);
    auto n = normalize(pos1 - pos0);
    auto cylinder = tg::cylinder_boundary<3, float>(pos0, pos1, r);
    auto tube = tg::tube3(pos0, pos1, r);
    auto cap0 = tg::sphere2in3(pos0, r, n);
    auto cap1 = tg::sphere2in3(pos1, r, n);

    auto any_on_side = false;
    auto any_on_cap_0 = false;
    auto any_on_cap_1 = false;

    for (auto i = 0; i < 256; ++i)
    {
        auto p = uniform(rng, cylinder);
        any_on_side |= contains(tube, p, tolerance);
        any_on_cap_0 |= contains(cap0, p, tolerance);
        any_on_cap_1 |= contains(cap1, p, tolerance);
    }

    CHECK(any_on_side);
    CHECK(any_on_cap_0);
    CHECK(any_on_cap_1);
}

// FIXME: the test is written flaky
FUZZ_TEST("UniformCapsule", disabled)(tg::rng& rng)
{
    auto tolerance = 0.01f;
    auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));
    auto r = uniform(rng, 1.0f, 10.0f);
    auto pos0 = uniform(rng, range3);
    auto pos1 = uniform(rng, range3);
    auto n = normalize(pos1 - pos0);
    auto capsule = tg::capsule_boundary<3, float>(pos0, pos1, r);
    auto tube = tg::tube3(pos0, pos1, r);
    auto cap0 = tg::hemisphere3(pos0, r, -n);
    auto cap1 = tg::hemisphere3(pos1, r, n);

    auto any_on_side = false;
    auto any_on_cap_0 = false;
    auto any_on_cap_1 = false;

    for (auto i = 0; i < 256; ++i)
    {
        auto p = uniform(rng, capsule);
        any_on_side |= contains(tube, p, tolerance);
        any_on_cap_0 |= contains(cap0, p, tolerance);
        any_on_cap_1 |= contains(cap1, p, tolerance);
    }

    CHECK(any_on_side);
    CHECK(any_on_cap_0);
    CHECK(any_on_cap_1);
}

FUZZ_TEST("UniformTriangle")(tg::rng& rng)
{
    auto range2 = tg::aabb2(tg::pos2(-1), tg::pos2(1));
    auto tri = tg::triangle2(uniform(rng, range2), uniform(rng, range2), uniform(rng, range2));

    for (auto i = 0; i < 100; ++i)
    {
        auto p = uniform(rng, tri);
        CHECK(tg::contains(tri, p, 1e-3f));
    }
}
