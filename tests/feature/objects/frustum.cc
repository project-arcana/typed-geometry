#include <nexus/test.hh>

TG_FUZZ_TEST(Frustum, BasicFuzzer)
{
    auto const bb = tg::aabb3(-10, 10);

    auto const eye = uniform(rng, bb);
    auto const target = uniform(rng, bb);

    auto const view = tg::look_at_opengl(eye, target, tg::dir3::pos_y);
    auto const proj = tg::perspective_opengl(uniform(rng, 30_deg, 80_deg), uniform(rng, 0.5f, 2.0f), uniform(rng, 0.1f, 1.f), uniform(rng, 100.f, 500.f));

    auto const view_proj = proj * view;
    auto const inv_view_proj = inverse(view_proj);

    auto const frustum = tg::frustum3::from_view_proj(view_proj);

    for (auto p : frustum.vertices)
        CHECK(contains(frustum, p, 0.01f));

    for (auto i = 0; i < 10; ++i)
    {
        auto const p = inv_view_proj * uniform(rng, tg::aabb3::minus_one_to_one);

        CHECK(contains(frustum, p, 0.01f));
    }
}
