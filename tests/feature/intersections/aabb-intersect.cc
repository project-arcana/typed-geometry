#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/intersections.hh>
#include <typed-geometry/feature/objects.hh>

FUZZ_TEST("AABB - Intersect")(tg::rng& rng)
{
    auto const bounds = tg::aabb3(-10, 10);

    {
        auto const min_a = uniform(rng, bounds);
        auto const max_a = uniform(rng, tg::aabb3(min_a, bounds.max));

        auto const aabb_a = tg::aabb3(min_a, max_a);

        auto const inside = uniform(rng, aabb_a);
        auto const other = uniform(rng, bounds);

        const tg::pos3 min_b = {
            tg::min(inside.x, other.x),
            tg::min(inside.y, other.y),
            tg::min(inside.z, other.z),
        };

        const tg::pos3 max_b = {
            tg::max(inside.x, other.x),
            tg::max(inside.y, other.y),
            tg::max(inside.z, other.z),
        };

        auto const aabb_b = tg::aabb3(min_b, max_b);

        CHECK(tg::intersects(aabb_a, aabb_b));
    }
}


FUZZ_TEST("AABB - NonIntersect")(tg::rng& rng)
{
    auto const bounds = tg::aabb3(-10, 10);

    {
        auto const min_a = uniform(rng, bounds);
        auto const max_a = uniform(rng, tg::aabb3(min_a, bounds.max));

        auto const aabb_a = tg::aabb3(min_a, max_a);

        auto min_b = uniform(rng, bounds);
        auto max_b = uniform(rng, tg::aabb3(min_b, bounds.max));

        // generate one separating axis
        auto const dim = uniform(rng, {0, 1, 2});
        auto const dir = uniform(rng, {true, false});
        if (dir)
        {
            min_b[dim] = uniform(rng, max_a[dim], bounds.max[dim]);
            max_b[dim] = uniform(rng, min_b[dim], bounds.max[dim]);
        }
        else
        {
            max_b[dim] = uniform(rng, bounds.min[dim], min_a[dim]);
            min_b[dim] = uniform(rng, bounds.min[dim], max_b[dim]);
        }

        auto const aabb_b = tg::aabb3(min_b, max_b);

        CHECK(!tg::intersects(aabb_a, aabb_b));
    }
}

FUZZ_TEST("AABB - SamplingTest")(tg::rng& rng)
{
    auto const bounds = tg::aabb3(-10, 10);
    {
        auto const min_a = uniform(rng, bounds);
        auto const max_a = uniform(rng, tg::aabb3(min_a, bounds.max));

        auto const min_b = uniform(rng, bounds);
        auto const max_b = uniform(rng, tg::aabb3(min_b, bounds.max));

        auto const aabb_a = tg::aabb3(min_a, max_a);
        auto const aabb_b = tg::aabb3(min_b, max_b);

        auto const res = intersection(aabb_a, aabb_b);

        if (res.has_value())
        {
            auto const intsct = res.value();

            for (auto i = 0; i < 100; ++i)
            {
                auto const v = uniform(rng, bounds);

                if (contains(intsct, v))
                {
                    CHECK(contains(aabb_a, v));
                    CHECK(contains(aabb_b, v));
                }

                if (contains(aabb_a, v) && contains(aabb_b, v))
                {
                    CHECK(contains(intsct, v));
                }

                if (!contains(aabb_a, v))
                {
                    CHECK(!contains(intsct, v));
                }

                if (!contains(aabb_b, v))
                {
                    CHECK(!contains(intsct, v));
                }
            }
        }
        else
        {
            // no intersection
            for (auto i = 0; i < 50; ++i)
            {
                auto const v = uniform(rng, aabb_a);
                CHECK(!contains(aabb_b, v));
            }

            for (auto i = 0; i < 50; ++i)
            {
                auto const v = uniform(rng, aabb_b);
                CHECK(!contains(aabb_a, v));
            }
        }
    }
}

FUZZ_TEST("AABB - SphereIntersection")(tg::rng& rng)
{
    auto const bounds = tg::aabb3(-10, 10);

    auto const bb = aabb_of(uniform(rng, bounds), uniform(rng, bounds));
    auto const s = tg::sphere3(uniform(rng, bounds), uniform(rng, 1.0f, 10.0f));

    if (intersects(bb, s)) // intersection?
    {
        // aabb of sphere must intersect original aabb
        auto const sbb = aabb_of(s);
        CHECK(intersects(sbb, bb));
    }
    else // no intersection?
    {
        // no aabb point must be contained in the sphere
        for (auto i = 0; i < 100; ++i)
        {
            auto const p = uniform(rng, bb);
            CHECK(!contains(s, p));
        }

        // no sphere point must be contained in the aabb
        for (auto i = 0; i < 100; ++i)
        {
            auto const p = uniform(rng, s);
            CHECK(!contains(bb, p));
        }

        // no point must be contained in both
        for (auto i = 0; i < 100; ++i)
        {
            auto const p = uniform(rng, bb);
            CHECK(!(contains(bb, p) && contains(s, p)));
        }
    }
}

FUZZ_TEST("AABB - TriangleIntersection3")(tg::rng& rng)
{
    auto const bounds = tg::aabb3(-10, 10);

    auto const bb = aabb_of(uniform(rng, bounds), uniform(rng, bounds));
    auto const tri = tg::triangle3(uniform(rng, bounds), uniform(rng, bounds), uniform(rng, bounds));

    auto p = uniform(rng, tri);
    if (contains(bb, p))
        CHECK(intersects(bb, tri));

    if (distance(centroid_of(bb), tri) > distance(centroid_of(bb), bb.min))
        CHECK(!intersects(bb, tri));

    if (!intersects(bb, tri))
        CHECK(distance(p, bb) > 0);
}

FUZZ_TEST("AABB - LineIntersection")(tg::rng& rng)
{
    auto const bounds = tg::aabb2(-10, 10);
    auto const bb = aabb_of(uniform(rng, bounds), uniform(rng, bounds));
    auto const line = tg::line2(uniform(rng, bounds), tg::uniform<tg::dir2>(rng));

    CHECK(intersects(line, bb) == tg::intersection_parameter(line, bb).has_value());
}

FUZZ_TEST("AABB - RayIntersection")(tg::rng& rng)
{
    auto const bounds = tg::aabb2(-10, 10);
    auto const bb = aabb_of(uniform(rng, bounds), uniform(rng, bounds));
    auto const ray = tg::ray2(uniform(rng, bounds), tg::uniform<tg::dir2>(rng));

    CHECK(intersects(ray, bb) == tg::intersection_parameter(ray, bb).has_value());
}

// FIXME: flaky right now
FUZZ_TEST("AABB - ObjectIntersection", disabled)(tg::rng& rng)
{
    auto const numSamples = 250;
    auto const test_obj = [&](auto const& bb, auto const& o)
    {
        if (!intersects(o, bb))
        {
            for (auto i = 0; i < numSamples; ++i)
            {
                auto pBox = uniform(rng, bb);
                CHECK(!contains(o, pBox));
            }
        }

        for (auto i = 0; i < numSamples; ++i)
        {
            auto pBox = uniform(rng, bb);
            if (contains(o, pBox))
            {
                CHECK(intersects(o, bb));
                break;
            }
        }

        if constexpr (tg::object_traits<std::decay_t<decltype(o)>>::is_finite)
        {
#ifndef TG_COMPILER_GCC
            auto const objBounds = aabb_of(o);
            CHECK(intersects(o, objBounds));
#endif

            if (!intersects(o, bb))
            {
                for (auto i = 0; i < numSamples; ++i)
                {
                    auto pObj = uniform(rng, o);
                    CHECK(!contains(bb, pObj));
                }
            }

            for (auto i = 0; i < numSamples; ++i)
            {
                auto pObj = uniform(rng, o);
                if (contains(bb, pObj))
                {
                    CHECK(intersects(o, bb));
                    break;
                }
            }
        }
    };

    auto const test_obj_and_boundary = [&test_obj](auto const& bb, auto const& o)
    {
        test_obj(bb, o);
        test_obj(bb, boundary_of(o));
    };

    auto const test_obj_and_boundary_no_caps = [&test_obj](auto const& bb, auto const& o)
    {
        test_obj(bb, o);
        test_obj(bb, boundary_of(o));
        test_obj(bb, boundary_no_caps_of(o));
    };

    auto const r = uniform(rng, 0.0f, 10.0f);
    auto const h = uniform(rng, 0.0f, 10.0f);
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

    auto const bb1 = aabb_of(uniform(rng, range1), uniform(rng, range1));
    auto const bb2 = aabb_of(uniform(rng, range2), uniform(rng, range2));
    auto const bb3 = aabb_of(uniform(rng, range3), uniform(rng, range3));
    auto const bb4 = aabb_of(uniform(rng, range4), uniform(rng, range4));

    // aabb
    test_obj_and_boundary(bb1, aabb_of(pos10, pos11));
    test_obj_and_boundary(bb2, aabb_of(pos20, pos21));
    test_obj_and_boundary(bb3, aabb_of(pos30, pos31));
    test_obj_and_boundary(bb4, aabb_of(pos40, pos41));
    // box
    test_obj_and_boundary(bb1, tg::box1(pos10, m1));
    test_obj_and_boundary(bb2, tg::box2(pos20, m2));
    test_obj_and_boundary(bb3, tg::box3(pos30, m3));
    // TODO: box4
    test_obj_and_boundary(bb3, tg::box2in3(pos30, m23));
    // capsule
    test_obj_and_boundary(bb3, tg::capsule3(axis0, r));
    // cylinder
    test_obj_and_boundary_no_caps(bb3, tg::cylinder3(axis0, r));
    // TODO: ellipse
    //    test_obj_and_boundary(bb1, tg::ellipse1(pos10, m1));
    //    test_obj_and_boundary(bb2, tg::ellipse2(pos20, m2));
    //    test_obj_and_boundary(bb3, tg::ellipse3(pos30, m3));
    // TODO: ellipse4
    //    test_obj_and_boundary(bb3, tg::ellipse2in3(pos30, m23));
    // halfspace
    test_obj(bb1, tg::halfspace1(n1, h));
    test_obj(bb2, tg::halfspace2(n2, h));
    test_obj(bb3, tg::halfspace3(n3, h));
    test_obj(bb4, tg::halfspace4(n4, h));
    // hemisphere
    test_obj_and_boundary_no_caps(bb1, tg::hemisphere1(pos10, r, n1));
    test_obj_and_boundary_no_caps(bb2, tg::hemisphere2(pos20, r, n2));
    test_obj_and_boundary_no_caps(bb3, tg::hemisphere3(pos30, r, n3));
    // test_obj_and_boundary_no_caps(bb4, tg::hemisphere4(pos40, r, n4));
    // TODO: inf_cone
    //    test_obj_and_boundary(bb2, tg::inf_cone2(pos20, n2, a));
    //    test_obj_and_boundary(bb3, tg::inf_cone3(pos30, n3, a));
    // inf_cylinder
    test_obj_and_boundary(bb2, tg::inf_cylinder2(tg::line2(pos20, n2), r));
    test_obj_and_boundary(bb3, tg::inf_cylinder3(tg::line3(pos30, n3), r));
    // line
    test_obj(bb1, tg::line1(pos10, n1));
    test_obj(bb2, tg::line2(pos20, n2));
    test_obj(bb3, tg::line3(pos30, n3));
    test_obj(bb4, tg::line4(pos40, n4));
    // plane
    test_obj(bb1, tg::plane1(n1, h));
    test_obj(bb2, tg::plane2(n2, h));
    test_obj(bb3, tg::plane3(n3, h));
    test_obj(bb4, tg::plane4(n4, h));
    // pyramid
    test_obj_and_boundary_no_caps(bb3, tg::pyramid<tg::box2in3>(tg::box2in3(pos30, m23), h));
    //    test_obj_and_boundary_no_caps(bb3, tg::pyramid<tg::sphere2in3>(disk0, h)); // == cone
    test_obj_and_boundary_no_caps(bb3, tg::pyramid<tg::triangle3>(tg::triangle3(pos30, pos31, pos32), h));
    test_obj(bb3, tg::pyramid_boundary_no_caps<tg::quad3>(tg::quad3(pos30, pos31, pos32, pos32 + (pos30 - pos31)), h));
    //    test_obj_and_boundary_no_caps(bb3, tg::pyramid<tg::quad3>(tg::quad3(pos30, pos31, pos32, pos32 + (pos30 - pos31)), h)); // needs uniform(quad)
    // TODO: quad
    // test_obj(bb2, tg::quad2(pos20, pos21, pos22, pos23));
    // test_obj(bb3, tg::quad3(pos30, pos31, pos32, pos32 + (pos31 - pos30)));
    // test_obj(bb4, tg::quad4(pos40, pos41, pos42, pos42 + (pos41 - pos40)));
    // ray
    test_obj(bb1, tg::ray1(pos10, n1));
    test_obj(bb2, tg::ray2(pos20, n2));
    test_obj(bb3, tg::ray3(pos30, n3));
    test_obj(bb4, tg::ray4(pos40, n4));
    // segment
    test_obj(bb1, tg::segment1(pos10, pos11));
    test_obj(bb2, tg::segment2(pos20, pos21));
    test_obj(bb3, tg::segment3(pos30, pos31));
    test_obj(bb4, tg::segment4(pos40, pos41));
    // sphere
    test_obj_and_boundary(bb1, tg::sphere1(pos10, r));
    test_obj_and_boundary(bb2, tg::sphere2(pos20, r));
    test_obj_and_boundary(bb3, tg::sphere3(pos30, r));
    test_obj_and_boundary(bb4, tg::sphere4(pos40, r));
    test_obj_and_boundary(bb2, tg::sphere<1, float, 2>(pos20, r, n2));
    test_obj_and_boundary(bb3, tg::sphere2in3(pos30, r, n3));
    // triangle
    test_obj(bb2, tg::triangle2(pos20, pos21, pos22));
    test_obj(bb3, tg::triangle3(pos30, pos31, pos32));
    // test_obj(bb4, tg::triangle4(pos40, pos41, pos42));
}
