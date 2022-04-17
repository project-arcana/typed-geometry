#include <nexus/test.hh>

#include <typed-geometry/feature/basic.hh>
#include <typed-geometry/feature/random.hh>

TEST("Uniform Conversion API")
{
    tg::rng rng;

    bool b = uniform(rng);
    (void)b;

    tg::angle a = uniform(rng);
    CHECK(a >= 0_deg);
    CHECK(a <= 360_deg);

    tg::color3 c = uniform(rng);
    CHECK(c.r >= 0);
    CHECK(c.g >= 0);
    CHECK(c.b >= 0);
    CHECK(c.r <= 1);
    CHECK(c.g <= 1);
    CHECK(c.b <= 1);

    tg::dir3 d = uniform(rng);
    (void)d;

    if (uniform(rng))
    {
        // on coin flip
    }
}
