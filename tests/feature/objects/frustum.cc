#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/basic.hh>
#include <typed-geometry/feature/intersections.hh>
#include <typed-geometry/feature/matrix.hh>
#include <typed-geometry/feature/objects.hh>

FUZZ_TEST("Frustum - BasicFuzzer")(tg::rng& rng)
{
    auto const bb = tg::aabb3(-10, 10);

    auto const eye = uniform(rng, bb);
    auto const target = uniform(rng, bb);

    auto check_frustum = [&rng](auto frustum, tg::mat4 const& inv_view_proj, tg::aabb3 ndc)
    {
        if constexpr (std::is_same_v<decltype(frustum), tg::frustum3>)
            for (auto p : frustum.vertices)
                CHECK(contains(frustum, p, 0.01f));

        //
        // contains center
        //
        auto center = inv_view_proj * ndc[tg::comp3(0.5f)];
        CHECK(contains(frustum, center, 0.01f));

        //
        // contains point
        //
        for (auto i = 0; i < 10; ++i)
        {
            auto const p = inv_view_proj * uniform(rng, ndc);

            CHECK(contains(frustum, p, 0.01f));
        }

        //
        // not contains point
        //
        for (auto i = 0; i < 10; ++i)
        {
            auto const c = uniform(rng, tg::aabb3(-2, 2));
            if (tg::max_element(tg::abs(c)) < 1.2f)
                continue;
            if (ndc.min.z == 0 && c.z <= 0)
                continue; // not suitable for reverse_z

            auto const p = inv_view_proj * c;

            CHECK(!contains(frustum, p));
        }

        //
        // intersects sphere
        //
        for (auto i = 0; i < 10; ++i)
        {
            auto const r = uniform(rng, 0.1f, 1.f);
            auto const c = inv_view_proj * uniform(rng, tg::aabb3(-2, 2));
            auto const sphere = tg::sphere3(c, r);
            auto const p = uniform(rng, sphere);

            auto const intersects = tg::intersects_conservative(frustum, sphere);
            auto const contains_p = contains(frustum, p);

            if (contains_p)
                CHECK(intersects);

            if (!intersects)
                CHECK(!contains_p);
        }

        //
        // intersects aabb
        //
        // FIXME: slightly floating-point flaky
        // for (auto i = 0; i < 10; ++i)
        // {
        //     auto const bb = aabb_of(inv_view_proj * uniform(rng, tg::aabb3(-2, 2)), inv_view_proj * uniform(rng, tg::aabb3(-2, 2)));
        //     auto const p = uniform(rng, bb);

        //     auto const intersects = tg::intersects_conservative(frustum, bb);
        //     auto const contains_p = contains(frustum, p);

        //     if (contains_p)
        //         CHECK(intersects);

        //     if (!intersects)
        //         CHECK(!contains_p);
        // }

        //
        // intersects box
        //
        for (auto i = 0; i < 10; ++i)
        {
            auto const c = inv_view_proj * uniform(rng, tg::aabb3(-2, 2));
            auto const d0 = tg::uniform<tg::dir3>(rng) * uniform(rng, 0.1f, 1.f);
            auto d1 = tg::uniform<tg::dir3>(rng) * uniform(rng, 0.1f, 1.f);
            auto d2 = normalize(cross(d1, d0));
            d1 = normalize(cross(d1, d2)) * uniform(rng, {-1.f, 1.f});
            auto const box = tg::box3(c, tg::mat3::from_cols(d0, d1, d2));
            auto const p = uniform(rng, box);

            auto const intersects = tg::intersects_conservative(frustum, box);
            auto const contains_p = contains(frustum, p);

            if (contains_p)
                CHECK(intersects);

            if (!intersects)
                CHECK(!contains_p);
        }
    };

    //
    // normal frustum
    //
    {
        auto const view = tg::look_at_opengl(eye, target, tg::dir3::pos_y);
        auto const proj = tg::perspective_opengl(tg::horizontal_fov(uniform(rng, 30_deg, 80_deg)), uniform(rng, 0.5f, 2.0f), uniform(rng, 0.1f, 1.f), uniform(rng, 100.f, 500.f));

        auto const view_proj = proj * view;
        auto const inv_view_proj = inverse(view_proj);

        auto const frustum = tg::frustum3::from_view_proj(view_proj);

        check_frustum(frustum, inv_view_proj, tg::aabb3::minus_one_to_one);
    }

    //
    // reverse_z frustum
    //
    {
        auto const view = tg::look_at_opengl(eye, target, tg::dir3::pos_y);
        auto const proj = tg::perspective_reverse_z_opengl(tg::horizontal_fov(uniform(rng, 30_deg, 80_deg)), uniform(rng, 0.5f, 2.0f), uniform(rng, 0.1f, 1.f));

        auto const view_proj = proj * view;
        auto const inv_view_proj = inverse(view_proj);

        auto frustum = tg::inf_frustum3::from_view_proj_reverse_z(view_proj);

        check_frustum(frustum, inv_view_proj, tg::aabb3({-1, -1, 0}, {1, 1, 1}));
    }
}
