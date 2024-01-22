#include <nexus/fuzz_test.hh>

#include <set>

#include <typed-geometry/tg-std.hh>

FUZZ_TEST("Rasterize")(tg::rng& rng)
{
    auto range2 = tg::aabb2(tg::pos2(-10), tg::pos2(10));
    auto tri = tg::triangle2(tg::uniform(rng, range2), tg::uniform(rng, range2), tg::uniform(rng, range2));

    const auto samplesInTriangle = 10;
    std::set<tg::ipos2> triSamples;

    for (auto i = 0; i < samplesInTriangle; ++i)
    {
        auto sample = tg::uniform(rng, tri);
        // TODO: implement proper rounding
        auto isample = iround(tg::pos2(sample.x + 0.5f, sample.y + 0.5f));
        if (tg::contains(tri, tg::pos2(isample)))
        {
            // valid sample
            triSamples.insert(isample);
        }
    }

    // std::cout << "--" << std::endl;

    int count = 0;
    tg::rasterize(tri,
                  [&](tg::ipos2 p, float a, float b)
                  {
                      // barycentric coordinates must not exceed range [0..1] inside triangle
                      CHECK(a >= 0.0f);
                      CHECK(b >= 0.0f);
                      CHECK(a <= 1.0f);
                      CHECK(b <= 1.0f);

                      CHECK(tg::contains(tri, tg::pos2(p)));

                      triSamples.erase(p);

                      // std::cout << p << std::endl;

                      ++count;
                  });

    // std::cout << tri << std::endl;
    // for (auto s : triSamples)
    //     std::cout << s << std::endl;

    // all sampled positions must also have been rasterized
    CHECK(triSamples.size() == 0u);
}


FUZZ_TEST("Rasterize2")(tg::rng& rng)
{
    auto range2 = tg::aabb2(tg::pos2(-10), tg::pos2(10));
    auto tri = tg::triangle2(tg::uniform(rng, range2), tg::uniform(rng, range2), tg::uniform(rng, range2));

    std::set<tg::ipos2> rpts;

    tg::rasterize(tri,
                  [&](tg::ipos2 p, float a, float b)
                  {
                      rpts.insert(p);

                      auto rp = interpolate(tri, a, b);

                      CHECK(distance(tg::pos2(p), rp) < 1e-2f);
                  });

    for (auto i = 0; i < 100; ++i)
    {
        auto p = iround(uniform(rng, range2));

        CHECK(contains(tri, tg::pos2(p)) == rpts.count(p));
    }
}
