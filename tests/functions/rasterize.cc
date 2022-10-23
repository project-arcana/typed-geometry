#include <nexus/fuzz_test.hh>

#include <rich-log/log.hh> // debug

#include <clean-core/set.hh>

#include <typed-geometry/functions/objects/rasterize.hh>
#include <typed-geometry/types/objects/sphere.hh>

FUZZ_TEST("Rasterizeicircle2")(tg::rng& rng)
{
    auto aabb = tg::iaabb2(-10, 10);
    auto center = tg::uniform(rng, aabb);
    auto radius = tg::uniform(rng, 2, 100);
    auto circle = tg::icircle2(center, radius);

    auto const error = [&](tg::ipos2 p)
    {
        p = p - tg::ivec2(center); // move to origin
        return tg::abs(p.x * p.x + p.y * p.y - radius * radius);
    };
    cc::set<tg::ipos2> set;
    tg::rasterize(circle, [&](tg::ipos2 const& p) { set.add(p); });

    for (auto const& p : set)
    {
        auto const e = error(p);
        for (auto x : {-1, 0, 1})
            for (auto y : {-1, 0, 1})
            {
                auto const nb = p + tg::ivec2(x, y);
                if (set.contains(nb))
                    continue;

                auto const nb_error = error(nb);
                CHECK(e <= nb_error);
            }
    }
}
