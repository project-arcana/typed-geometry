#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/vector.hh>
#include <typed-geometry/feature/objects.hh>

FUZZ_TEST("AreaRelations")(tg::rng& rng)
{
    const auto r = uniform(rng, 0.0f, 10.0f);
    const auto r2 = uniform(rng, 0.0f, 10.0f);
    const auto h = uniform(rng, 0.0f, 10.0f);
    const auto n2 = tg::uniform<tg::dir2>(rng);
    const auto n3 = tg::uniform<tg::dir3>(rng);
    const auto range2 = tg::aabb2(tg::pos2(-10), tg::pos2(10));
    const auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));

    const auto pos20 = uniform(rng, range2);
    const auto pos21 = uniform(rng, range2);

    const auto pos30 = uniform(rng, range3);
    const auto pos31 = uniform(rng, range3);

    const auto d20 = tg::uniform<tg::dir2>(rng);
    const auto d21 = perpendicular(d20);
    const auto d30 = tg::uniform<tg::dir3>(rng);
    const auto d31 = any_normal(d30);
    const auto d32 = normalize(cross(d30, d31));


    const auto aabb = aabb_of(pos20, pos21);
    const auto aAabb = area_of(aabb);
    const auto aBox = area_of(tg::box2(aabb));
    CHECK(aAabb == nx::approx(aBox)); // conversion from aabb to box does not affect the area

    const auto aDisk = area_of(tg::disk2(pos20, r));
    const auto aDiskIn3 = area_of(tg::disk3(pos30, r, n3));
    const auto aHemisphere2 = area_of(tg::hemisphere2(pos20, r, n2));
    const auto aEllipse2 = area_of(tg::ellipse2(pos20, tg::mat2x2::from_cols(r * d20, r2 * d21)));
    const auto aEllipse23 = area_of(tg::ellipse2in3(pos30, tg::mat2x3::from_cols(r * d30, r2 * d31)));
    CHECK(aDisk == nx::approx(aDiskIn3)); // 2D and 3D disks have the same area
    CHECK(aDisk == nx::approx(2.f * aHemisphere2)); // sphere == 2 * hemisphere (2D)
    CHECK(aEllipse2 == nx::approx(aDisk * r2 / r));
    CHECK(aEllipse23 == nx::approx(aEllipse2));

    const auto hemisphere3 = tg::hemisphere3(pos30, r, n3);
    const auto aSphere = area_of(tg::sphere3(pos30, r));
    const auto aHemisphere3 = area_of(hemisphere3);
    const auto aHemisphereNoCaps = area_of(boundary_no_caps_of(hemisphere3));
    const auto aHemisphereCaps = area_of(caps_of(hemisphere3));
    CHECK(aSphere == nx::approx(2.f * aHemisphereNoCaps)); // sphere == 2 * hemisphere (3D)
    CHECK(aHemisphere3 == nx::approx(aHemisphereNoCaps + aHemisphereCaps)); // hemisphere = round half sphere + flat base
    CHECK(aHemisphereCaps == nx::approx(aDiskIn3)); // round base is the same as a disk2in3
    CHECK(aHemisphereNoCaps == nx::approx(2.f * aHemisphereCaps)); // ratio of areas for hemispheres

    const auto aEllipsoid3 = area_of(tg::ellipse3(pos30, tg::mat3::from_cols(r * d30, r * d31, r * d32)));
    CHECK(aEllipsoid3 == nx::approx(aSphere)); // sphere == ellipsoid if all 3 axes have equal length

    const auto cylinder = tg::cylinder3(pos30, pos31, r);
    const auto aCylinder = area_of(cylinder);
    const auto aCylinderMantle = area_of(boundary_no_caps_of(cylinder));
    const auto aCapsule = area_of(tg::capsule3(pos30, pos31, r));
    CHECK(aCylinder == nx::approx(aCylinderMantle + 2.f * aDiskIn3)); // cylinder == mantle + 2 caps
    CHECK(aCapsule == nx::approx(aCylinderMantle + aSphere)); // capsule == mantle + 2 caps
    CHECK(aCapsule == nx::approx(aCylinder + aHemisphereNoCaps)); // relation between capsule and cylinder

    const auto cone = tg::cone3(tg::sphere2in3(pos30, r, n3), h);
    CHECK(area_of(cone) == nx::approx(aHemisphereCaps + area_of(boundary_no_caps_of(cone)))); // cone = base + mantle
}

FUZZ_TEST("AreaObjects")(tg::rng& rng)
{
    const auto r = uniform(rng, 0.0f, 10.0f);
    const auto h = uniform(rng, 0.0f, 10.0f);
    const auto n3 = tg::uniform<tg::dir3>(rng);
    const auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));
    const auto pos30 = uniform(rng, range3);

    auto box1 = tg::aabb1(-1.0f, 1.0f);
    auto box2 = tg::aabb2(tg::pos2(-1.0f), tg::pos2(1.0f));
    auto box3 = tg::aabb3(tg::pos3(-1.0f), tg::pos3(1.0f));

    // triangle
    {
        // 2D
        tg::f32 x = tg::abs(uniform(rng, box1).x) * 9 + 1;
        tg::pos2 vertices[] = {tg::pos2(0, 0), tg::pos2(x, 0), tg::pos2(x, x)};

        // random 2d transformation
        auto alpha = tg::radians(tg::abs(uniform(rng, box1).x) * tg::pi_scalar<tg::f32>);
        auto rotation = tg::mat2::from_cols(tg::vec2(tg::cos(alpha), -sin(alpha)), tg::vec2(sin(alpha), cos(alpha)));
        tg::f32 randomScale = 0;
        auto max = 10.f;
        while (randomScale == 0)
            randomScale = max * uniform(rng, box1).x;
        auto scale = tg::mat2::from_cols(tg::vec2(randomScale, 0), tg::vec2(0, randomScale));
        auto translation = tg::translation(max * uniform(rng, box2) - tg::pos2::zero);

        // apply transformations on vertices
        for (tg::pos2& p : vertices)
        {
            p = translation * tg::pos2(rotation * scale * tg::vec2(p));
        }

        auto triangle = tg::triangle2(vertices[0], vertices[1], vertices[2]);

        // if triangle was scaled respect that
        x *= randomScale;
        const auto triangleArea = x * x / 2;
        {
            CHECK(triangleArea == nx::approx(area_of(triangle)));
        }

        // definitely smaller than aabb
        CHECK(area_of(triangle) < area_of(aabb_of(triangle)));

        {
            // 2d equilateral triangle
            tg::f32 y = tg::sqrt(tg::pow(x, 2) - tg::pow(x / 2, 2));
            tg::pos2 equiVerts[] = {tg::pos2(0, 0), tg::pos2(x, 0), tg::pos2(x / 2, y)};

            // scale and translate vertices (rotation would make below equation invalid, as aabb is not rotated)
            for (auto& p : equiVerts)
            {
                p = translation * tg::pos2(scale * tg::vec2(p));
            }

            auto equiTriangle = tg::triangle2(equiVerts[0], equiVerts[1], equiVerts[2]);

            auto equiTriangleArea = area_of(equiTriangle);
            // the area of a nonrotated triangle is half the area of its aabb
            CHECK(equiTriangleArea * 2.0f == nx::approx(area_of(aabb_of(equiTriangle))));
        }
    }
    {
        // 3D
        tg::f32 x = tg::abs(uniform(rng, box1)).x * 9 + 1;
        tg::pos3 vertices[] = {tg::pos3(0, 0, 0), tg::pos3(x, 0, 0), tg::pos3(x, x, 0)};

        auto alpha = tg::radians(tg::abs(uniform(rng, box1).x) * tg::pi_scalar<tg::f32>);
        auto rotation = rotation_x(alpha) * rotation_y(alpha) * rotation_z(alpha);

        tg::f32 randomScale = 0;
        auto max = 10.f;
        while (randomScale == 0)
            randomScale = max * uniform(rng, box1).x;

        auto rand_size = tg::size3(randomScale, randomScale, randomScale);
        auto scale = scaling(rand_size);

        auto translation = tg::translation(max * uniform(rng, box3) - tg::pos3::zero);

        for (auto& p : vertices)
        {
            tg::vec4 v = {p.x, p.y, p.z, 1};
            v = translation * rotation * scale * v;
            p = tg::pos3(v.x, v.y, v.z);
        }

        auto triangle = tg::triangle3(vertices[0], vertices[1], vertices[2]);

        x *= randomScale;
        const auto triangleArea = x * x / 2.0f;

        CHECK(triangleArea == nx::approx(area_of(triangle)));
    }

    // cylinder
    {
        const auto c = tg::cylinder3({0, 0, 0}, {0, h, 0}, r);
        CHECK(area_of(c) == nx::approx(h * 2 * r * tg::pi<tg::f32>.radians() + 2 * tg::pow2(r) * tg::pi<tg::f32>.radians()));
    }

    // disk
    {
        const auto d = tg::sphere2in3(pos30, r, n3);
        CHECK(area_of(d) == nx::approx(tg::pi_scalar<tg::f32> * tg::pow2(r)));
    }

    // sphere
    {
        const auto s = tg::sphere3(pos30, r);
        CHECK(area_of(s) == nx::approx(4 * tg::pi_scalar<tg::f32> * tg::pow2(r)));
    }

    // regular pyramid
    {
        const auto check_pyramid_area = [](const auto& py, auto slantHeight)
        {
            const auto perimeter = perimeter_of(py.base);
            const auto baseArea = area_of(py.base);

            CHECK(area_of(py) == nx::approx(perimeter * slantHeight / 2.f + baseArea));
            CHECK(area_of(boundary_no_caps_of(py)) == nx::approx(perimeter * slantHeight / 2.f));
        };

        const auto base = tg::triangle3(tg::pos3(r, 0, 0), tg::pos3(0, r, 0), tg::pos3(0, 0, r)); // equilateral triangle
        const auto pyTri = tg::pyramid<tg::triangle3>(base, h);
        const auto slantHeightTri = tg::sqrt(tg::pow2(h) + distance_sqr(base.pos0, base.pos1) / 12.f);

        const auto d30 = tg::uniform<tg::dir3>(rng);
        const auto d31 = any_normal(d30);
        const auto m23 = tg::mat2x3(r * d30, r * d31);
        const auto pyBox = tg::pyramid<tg::box2in3>(tg::box2in3(pos30, m23), h);
        const auto slantHeightBox = tg::sqrt(tg::pow2(h) + tg::pow2(r));

        const auto cone = tg::cone3(tg::sphere2in3(pos30, r, n3), h);
        const auto slantHeightCone = slantHeightBox;

        check_pyramid_area(pyTri, slantHeightTri);
        check_pyramid_area(pyBox, slantHeightBox);
        check_pyramid_area(cone, slantHeightCone);
    }
}
