#include <nexus/fuzz_test.hh>

#include <typed-geometry/tg.hh>

//#include <glow-extras/viewer/canvas.hh>

FUZZ_TEST("Segment3SphereSurface")(tg::rng& rng)
{
    auto const radius = tg::uniform(rng, 0.01f, 10.0f);
    auto const center = tg::uniform(rng, tg::aabb3(tg::pos3(-10), tg::pos3(10)));
    auto const sphere = tg::sphere3(center, radius * 0.9999f); // don't sample too close to the boundary
    auto const sphere_boundary = tg::sphere_boundary<3, float>(center, radius);

    auto const gen_point_inside = [&]() { return tg::uniform(rng, sphere); };
    auto const gen_point_outside = [&]() {
        auto const dir = tg::uniform<tg::dir3>(rng);
        auto const scaling = tg::uniform(rng, 1.0001f, 4.0f);
        return center + radius * scaling * dir;
    };

    // both inside -> no intersection
    for (auto i = 0; i < 1000; ++i)
    {
        auto const p0 = gen_point_inside();
        auto const p1 = gen_point_inside();
        auto const intersection = tg::intersection(sphere_boundary, tg::segment3(p0, p1));

        //        auto c = gv::canvas();
        //        c.add_line(p0, p1);
        //        c.add_sphere(center, radius, tg::color3::white);

        CHECK(!intersection.any());
    }

    // one inside, one outside -> 1 intersection
    for (auto i = 0; i < 1000; ++i)
    {
        auto const p0 = gen_point_inside();
        auto const p1 = gen_point_outside();
        auto const intersection = tg::intersection(sphere_boundary, tg::segment3(p0, p1));

        //        auto c = gv::canvas();
        //        c.add_line(p0, p1);
        //        c.add_sphere(center, radius, tg::color3::white);
        //        c.add_points(intersection.first(), tg::color3::red);

        CHECK(intersection.size() == 1);
    }
}
