#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>

FUZZ_TEST("ContainsBox")(tg::rng& rng)
{
    {
        auto p0 = uniform(rng, tg::aabb3(tg::pos3(-10.0f), tg::pos3(10.0f)));
        auto p1 = uniform(rng, tg::aabb3(tg::pos3(-10.0f), tg::pos3(10.0f)));

        auto b = aabb_of(p0, p1);

        auto p = uniform(rng, b);

        CHECK(contains(b, p));

        CHECK(!contains(b, {20, 10, 10}));
    }

    {
        auto p0 = uniform(rng, tg::iaabb3(tg::ipos3(-10), tg::ipos3(10)));
        auto p1 = uniform(rng, tg::iaabb3(tg::ipos3(-10), tg::ipos3(10)));

        auto b = aabb_of(p0, p1);

        auto p = uniform(rng, b);

        CHECK(contains(b, p));

        CHECK(!contains(b, {20, 10, 10}));
        CHECK(contains(b, b));
    }
}

FUZZ_TEST("ContainsSphere")(tg::rng& rng)
{
    auto c = uniform(rng, tg::aabb3::unit_centered);
    auto r = uniform(rng, 0.2f, 3.0f);

    {
        auto s = tg::sphere3(c, r);
        auto p = uniform(rng, s);
        CHECK(contains(s, p, 0.01f));
    }

    {
        auto b = tg::sphere3(c, r);
        auto p = uniform(rng, b);
        CHECK(contains(b, p, tg::epsilon<float> * 2));
    }
}

FUZZ_TEST("ContainsCylinder")(tg::rng& rng)
{
    auto box3 = tg::aabb3(tg::pos3(-1.0f), tg::pos3(1.0f));

    auto p0 = uniform(rng, box3);
    auto p1 = uniform(rng, box3);
    auto r = uniform(rng, 0.1f, 10.f);

    auto cyl = tg::cylinder3(p0, p1, r);
    auto c = centroid_of(cyl);
    CHECK(contains(cyl, c)); // cylinder contains its center

    auto n = normalize(p1 - p0);
    auto h = distance(p1, p0);
    auto t = any_normal(n);

    if (h < 0.1f || r < 0.1f)
        return; // too narrow

    auto eps = 1e-2f;
    auto below = 0.94f;
    auto above = 1.06f;

    {
        auto p = tg::pos3(c + h / 2 * n - eps * n); // slightly below top cap
        CHECK(contains(cyl, p));

        auto in = p + t * r * below; // still inside
        CHECK(contains(cyl, in));

        auto out = p + t * r * above; // outside
        CHECK(!contains(cyl, out));
    }

    {
        auto p = tg::pos3(c - h / 2 * n + eps * n); // slightly above bottom cap
        CHECK(contains(cyl, p));

        auto in = p + t * r * below; // still inside
        CHECK(contains(cyl, in));

        auto out = p + t * r * above; // outside
        CHECK(!contains(cyl, out));
    }

    // outside the caps
    CHECK(!contains(cyl, c + n * h / 2 * above));
    CHECK(!contains(cyl, c - n * h / 2 * above));
}
