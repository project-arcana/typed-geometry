#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>

FUZZ_TEST("Perimeter")(tg::rng& rng)
{
    const auto n2 = tg::uniform<tg::dir2>(rng);
    const auto n3 = tg::uniform<tg::dir3>(rng);

    const auto range2 = tg::aabb2(tg::pos2(-10), tg::pos2(10));
    const auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));

    const auto pos20 = uniform(rng, range2);
    const auto pos21 = uniform(rng, range2);

    const auto pos30 = uniform(rng, range3);

    const auto d20 = tg::uniform<tg::dir2>(rng);
    const auto d21 = perpendicular(d20);
    const auto d30 = tg::uniform<tg::dir3>(rng);
    const auto d31 = any_normal(d30);

    // === disk, disk2in3, circle, circle2in3, hemisphere2, (ellipse) ===

    // random radius
    auto r = uniform(rng, 0.0f, 10.0f);
    {
        auto disk = tg::sphere<2,float>(pos20, r);
        auto circle = tg::sphere_boundary<2, float>(pos20, r);

        auto disk3 = tg::sphere2in3(pos30, r, n3);
        auto circle3 = tg::sphere_boundary<2, float, 3>(pos30, r, n3);

        auto ellipse = tg::ellipse2(pos20, tg::mat2::from_cols(r * d20, r * d21));
        auto ellipse3 = tg::ellipse2in3(pos30, tg::mat2x3::from_cols(r * d30, r * d31));

        auto pd = perimeter_of(disk);
        auto pc = perimeter_of(circle);
        auto pd3 = perimeter_of(disk3);
        auto pc3 = perimeter_of(circle3);
        auto pe = perimeter_of(ellipse);
        auto pe3 = perimeter_of(ellipse3);

        // must all be the same
        CHECK(pd == nx::approx(pc));
        CHECK(pc == nx::approx(pd3));
        CHECK(pd3 == nx::approx(pc3));
        CHECK(pc3 == nx::approx(pe));
        CHECK(pe == nx::approx(pe3));

        auto hemisphere = tg::hemisphere2(pos20, r, n2);
        auto hemisphereBound = boundary_of(hemisphere);
        auto hemisphereNoCaps = boundary_no_caps_of(hemisphere);

        auto ph = perimeter_of(hemisphere);
        auto phb = perimeter_of(hemisphereBound);
        auto phc = perimeter_of(hemisphereNoCaps);

        CHECK(ph == nx::approx(phb)); // must be the same
        CHECK(ph == nx::approx(phc + 2.f * r)); // is no_caps + diameter
        CHECK(pd == nx::approx(2.f * phc)); // sphere == 2 * hemisphere
    }

    // known radius
    r = 1.0f / (2.0f * tg::pi<tg::f32>.radians());
    {
        auto disk2 = tg::sphere<2, float>(pos20, r);
        auto circle2 = tg::sphere_boundary<2, float>(pos20, r);

        auto disk3 = tg::sphere2in3(pos30, r, n3);
        auto circle3 = tg::sphere_boundary<2, float, 3>(pos30, r, n3);

        auto ellipse = tg::ellipse2(pos20, tg::mat2::from_cols(r * d20, r * d21));
        auto ellipse23 = tg::ellipse2in3(pos30, tg::mat2x3::from_cols(r * d30, r * d31));

        auto hemisphere = tg::hemisphere_boundary_no_caps<2, float>(pos20, r, n2);

        auto pd = perimeter_of(disk2);
        auto pc = perimeter_of(circle2);
        auto pd3 = perimeter_of(disk3);
        auto pc3 = perimeter_of(circle3);
        auto pe = perimeter_of(ellipse);
        auto pe23 = perimeter_of(ellipse23);
        auto phc = perimeter_of(hemisphere);

        CHECK(pd == nx::approx(1.0f));
        CHECK(pc == nx::approx(1.0f));
        CHECK(pd3 == nx::approx(1.0f));
        CHECK(pc3 == nx::approx(1.0f));
        CHECK(pe == nx::approx(1.0f));
        CHECK(pe23 == nx::approx(1.0f));
        CHECK(phc == nx::approx(0.5f));
    }

    // === aabb and box ===

    const auto aabb = aabb_of(pos20, pos21);
    const auto box = tg::box2(aabb);

    const auto pAabb = perimeter_of(aabb);
    const auto pBox = perimeter_of(box);

    CHECK(pAabb == nx::approx(pBox));
}
