#include <nexus/test.hh>

#include <typed-geometry/feature/vector.hh>

TEST("Ctor")
{
    {
        tg::vec3 v;
        CHECK(v.x == 0.0f);
        CHECK(v.y == 0.0f);
        CHECK(v.z == 0.0f);
    }
    {
        tg::ivec2 v(1, 2);
        CHECK(v.x == 1);
        CHECK(v.y == 2);
    }
    {
        tg::dvec3 v{3.0};
        CHECK(v.x == 3.0);
        CHECK(v.y == 3.0);
        CHECK(v.z == 3.0);
    }
    {
        auto v = tg::uvec4{1, 2, 3, 4};
        CHECK(v.x == 1u);
        CHECK(v.y == 2u);
        CHECK(v.z == 3u);
        CHECK(v.w == 4u);
    }

    {
        tg::pos3 v;
        CHECK(v.x == 0.0f);
        CHECK(v.y == 0.0f);
        CHECK(v.z == 0.0f);
    }
    {
        tg::ipos2 v(1, 2);
        CHECK(v.x == 1);
        CHECK(v.y == 2);
    }
    {
        tg::dpos3 v{3.0};
        CHECK(v.x == 3.0);
        CHECK(v.y == 3.0);
        CHECK(v.z == 3.0);
    }
    {
        auto v = tg::upos4{1, 2, 3, 4};
        CHECK(v.x == 1u);
        CHECK(v.y == 2u);
        CHECK(v.z == 3u);
        CHECK(v.w == 4u);
    }
}

TEST("VecOps")
{
    {
        auto v = tg::vec4(2, 2, 2, 2);
        CHECK(length(v) == 4.0f);
        auto n = normalize(v);
        CHECK(n == tg::dir4(0.5f, 0.5f, 0.5f, 0.5f));
    }
}

TEST("IvecLength")
{
    auto a = tg::ivec2(16044, 0);
    auto b = tg::ivec2(28444, 12974);
    CHECK(length(a + b) <= length(a) + length(b));
}
