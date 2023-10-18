#include <nexus/fuzz_test.hh>
#include <nexus/ext/tg-approx.hh>

#include <typed-geometry/feature/intersections.hh>
#include <typed-geometry/feature/objects.hh>

FUZZ_TEST("Intersections", reproduce(42030056))(tg::rng& rng)
{
    auto const tolerance = 0.02f;
    auto const test_obj = [tolerance](auto const& ray, auto const& obj)
    {
        auto const ts = tg::intersection_parameter(ray, obj);
        for (auto const& t : ts)
        {
            auto const ip = ray[t];
            CHECK(contains(obj, ip, tolerance * tg::sqrt(t)));
        }
        auto const t = tg::closest_intersection_parameter(ray, obj);
        if (t.has_value())
        {
            CHECK(t.value() == nx::approx(ts.first()));
            CHECK(closest_intersection(ray, obj).has_value());
            CHECK(closest_intersection(ray, obj).value() == nx::approx(ray[t.value()]));
            CHECK(intersects(ray, obj));
        }
        else
            CHECK(!intersects(ray, obj));

        auto const tsLine = tg::intersection_parameter(inf_of(ray), obj);
        auto iRay = 0;
        for (auto iLine = 0; iLine < tsLine.size(); ++iLine)
            if (tsLine[iLine] >= 0)
                CHECK(tsLine[iLine] == nx::approx(ts[iRay++]));
        CHECK(iRay == ts.size());
    };

    auto const test_solid_obj = [tolerance, &rng](auto const& ray, auto const& obj)
    {
        auto const ts = tg::intersection_parameter(ray, obj);
        if (ts.has_value())
        {
            auto const interval = ts.value();
            auto const ip1 = ray[interval.start];
            auto const ip2 = ray[uniform(rng, interval.start, interval.end)];
            auto const ip3 = ray[interval.end];
            CHECK(contains(obj, ip1, tolerance));
            CHECK(contains(obj, ip2, tolerance));
            CHECK(contains(obj, ip3, tolerance * tg::sqrt(interval.end)));

            auto const t = tg::closest_intersection_parameter(ray, obj);
            CHECK(t.has_value());
            CHECK(t.value() == ts.value().start);
            CHECK(closest_intersection(ray, obj) == ip1);
            CHECK(intersects(ray, obj));

            auto const tsLine = tg::intersection_parameter(inf_of(ray), obj);
            CHECK(tsLine.has_value());
            auto const interLine = tsLine.value();
            CHECK(interLine.end == interval.end);
            if (interLine.start < 0)
                CHECK(interval.start == 0);
            else
                CHECK(interLine.start == interval.start);
        }
        else
            CHECK(!intersects(ray, obj));
    };

    auto const test_obj_and_boundary = [&](auto const& ray, auto const& obj)
    {
        auto const bounds = boundary_of(obj);
        test_solid_obj(ray, obj);
        test_obj(ray, bounds);

        auto const iObj = tg::closest_intersection_parameter(ray, obj);
        auto const iBounds = tg::closest_intersection_parameter(ray, bounds);
        if (iBounds.has_value())
        {
            CHECK(iObj.has_value());
            CHECK(iBounds.value() >= iObj.value());
            CHECK(contains(obj, ray[iBounds.value()], tolerance));
        }
    };

    auto const test_obj_and_boundary_no_caps = [&](auto const& ray, auto const& obj)
    {
        auto const bounds = boundary_of(obj);
        auto const boundsNoCaps = boundary_no_caps_of(obj);
        test_solid_obj(ray, obj);
        test_obj(ray, bounds);
        test_obj(ray, boundsNoCaps);

        auto const iObj = tg::closest_intersection_parameter(ray, obj);
        auto const iBounds = tg::closest_intersection_parameter(ray, bounds);
        auto const iBoundsNoCaps = tg::closest_intersection_parameter(ray, boundsNoCaps);
        if (iBoundsNoCaps.has_value())
        {
            CHECK(iBounds.has_value());
            CHECK(iBoundsNoCaps.value() >= iBounds.value());
            CHECK(contains(bounds, ray[iBoundsNoCaps.value()], tolerance));
        }
        if (iBounds.has_value())
        {
            CHECK(iObj.has_value());
            CHECK(iBounds.value() >= iObj.value());
            CHECK(contains(obj, ray[iBounds.value()], tolerance));
        }
    };

    auto const r = uniform(rng, 0.0f, 10.0f);
    auto const h = uniform(rng, 0.0f, 10.0f);
    auto const a = uniform(rng, 5_deg, 179_deg); // sensible range for a convex inf_cone
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


    auto const ray1 = tg::ray1(uniform(rng, range1), tg::uniform<tg::dir1>(rng));
    auto const ray2 = tg::ray2(uniform(rng, range2), tg::uniform<tg::dir2>(rng));
    auto const ray3 = tg::ray3(uniform(rng, range3), tg::uniform<tg::dir3>(rng));
    auto const ray4 = tg::ray4(uniform(rng, range4), tg::uniform<tg::dir4>(rng));

    // aabb
    test_obj_and_boundary(ray1, aabb_of(pos10, pos11));
    test_obj_and_boundary(ray2, aabb_of(pos20, pos21));
    test_obj_and_boundary(ray3, aabb_of(pos30, pos31));
    test_obj_and_boundary(ray4, aabb_of(pos40, pos41));
    // box
    test_obj_and_boundary(ray1, tg::box1(pos10, m1));
    test_obj_and_boundary(ray2, tg::box2(pos20, m2));
    test_obj_and_boundary(ray3, tg::box3(pos30, m3));
    test_obj(ray3, tg::box2in3(pos30, m23));
    // capsule
    test_obj_and_boundary(ray3, tg::capsule3(axis0, r));
    // cylinder
    test_obj_and_boundary_no_caps(ray3, tg::cylinder<3, float>(axis0, r));
    // ellipse
    test_obj_and_boundary(ray1, tg::ellipse1(pos10, m1));
    test_obj_and_boundary(ray2, tg::ellipse2(pos20, m2));
    test_obj_and_boundary(ray3, tg::ellipse3(pos30, m3));
    // TODO: ellipse4
    test_obj(ray3, tg::ellipse2in3(pos30, m23));
    // halfspace
    test_solid_obj(ray1, tg::halfspace1(n1, h));
    test_solid_obj(ray2, tg::halfspace2(n2, h));
    test_solid_obj(ray3, tg::halfspace3(n3, h));
    test_solid_obj(ray4, tg::halfspace4(n4, h));
    // hemisphere
    test_obj_and_boundary_no_caps(ray1, tg::hemisphere1(pos10, r, n1));
    test_obj_and_boundary_no_caps(ray2, tg::hemisphere2(pos20, r, n2));
    test_obj_and_boundary_no_caps(ray3, tg::hemisphere3(pos30, r, n3));
    // test_obj_and_boundary_no_caps(ray4, tg::hemisphere4(pos40, r, n4));
    // inf_cone
    test_obj_and_boundary(ray2, tg::inf_cone2(pos20, n2, a));
    // FIXME: inf_cone is not fully stable currently
    // test_obj_and_boundary(ray3, tg::inf_cone3(pos30, n3, a));
    // inf_cylinder
    test_obj_and_boundary(ray2, tg::inf_cylinder2(tg::line2(pos20, n2), r));
    test_obj_and_boundary(ray3, tg::inf_cylinder3(tg::line3(pos30, n3), r));
    // line
    test_obj(ray2, tg::line2(pos20, n2));
    // plane
    test_obj(ray1, tg::plane1(n1, h));
    test_obj(ray2, tg::plane2(n2, h));
    test_obj(ray3, tg::plane3(n3, h));
    test_obj(ray4, tg::plane4(n4, h));
    // pyramid
    test_obj_and_boundary_no_caps(ray3, tg::pyramid<tg::box2in3>(tg::box2in3(pos30, m23), h));
    test_obj_and_boundary_no_caps(ray3, tg::pyramid<tg::sphere2in3>(disk0, h)); // == cone
    test_obj_and_boundary_no_caps(ray3, tg::pyramid<tg::triangle3>(tg::triangle3(pos30, pos31, pos32), h));
    test_obj(ray3, tg::pyramid_boundary_no_caps<tg::quad3>(tg::quad3(pos30, pos31, pos32, pos32 + (pos30 - pos31)), h));
    // test_obj_and_boundary_no_caps(ray3, tg::pyramid<tg::quad3>(tg::quad3(pos30, pos31, pos32, pos32 + (pos30 - pos31)), h));
    // ray
    test_obj(ray2, tg::ray2(pos20, n2));
    // segment
    test_obj(ray2, tg::segment2(pos20, pos21));
    // sphere
    test_obj_and_boundary(ray1, tg::sphere1(pos10, r));
    test_obj_and_boundary(ray2, tg::sphere2(pos20, r));
    test_obj_and_boundary(ray3, tg::sphere3(pos30, r));
    test_obj_and_boundary(ray4, tg::sphere4(pos40, r));
    test_obj(ray3, tg::sphere2in3(pos30, r, n3));
    // triangle
    test_solid_obj(ray2, tg::triangle2(pos20, pos21, pos22));
    test_obj(ray3, tg::triangle3(pos30, pos31, pos32));
}

FUZZ_TEST("Intersect - LineLine2")(tg::rng& rng)
{
    auto bb = tg::aabb2(-10.f, 10.f);
    auto l0 = tg::line2::from_points(uniform(rng, bb), uniform(rng, bb));
    auto l1 = tg::line2::from_points(uniform(rng, bb), uniform(rng, bb));

    auto [t0, t1] = tg::intersection_parameters(l0, l1);
    CHECK(l0[t0] == nx::approx(l1[t1]).abs(0.01f));
}

FUZZ_TEST("Intersect - SegSeg2")(tg::rng& rng)
{
    auto bb = tg::aabb2(-10.f, 10.f);
    auto s0 = tg::segment2(uniform(rng, bb), uniform(rng, bb));
    auto s1 = tg::segment2(uniform(rng, bb), uniform(rng, bb));

    auto ip = intersection(s0, s1);
    if (ip.has_value())
    {
        CHECK(distance(ip.value(), s0) < 0.01f);
        CHECK(distance(ip.value(), s1) < 0.01f);
    }
}
