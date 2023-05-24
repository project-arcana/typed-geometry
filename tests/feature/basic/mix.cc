#include <nexus/test.hh>

#include <typed-geometry/feature/basic.hh>

TEST("MixBasics")
{
    CHECK(tg::mix(2.0f, 10.0f, 0.0f) == 2.0f);
    CHECK(tg::mix(2.0f, 10.0f, 1.0f) == 10.0f);
    CHECK(tg::mix(2.0f, 10.0f, 0.5f) == 6.0f);
}
