#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>

FUZZ_TEST("Interpolate")(tg::rng& rng)
{
    auto box1 = tg::aabb1(-1.0f, 1.0f);
    auto box2 = tg::aabb2(tg::pos2(-1.0f), tg::pos2(1.0f));
    auto box3 = tg::aabb3(tg::pos3(-1.0f), tg::pos3(1.0f));

    {
        // 2D triangle
        tg::f32 x = tg::abs(uniform(rng, box1).x) * 9 + 1;
        tg::pos2 vertices[] = {tg::pos2(0, 0), tg::pos2(x, 0), tg::pos2(x, x)};

        // random 2d transformation
        auto alpha = tg::radians(tg::abs(uniform(rng, box1).x) * tg::pi_scalar<tg::f32>);
        auto rotation = tg::mat2::from_cols(tg::vec2(tg::cos(alpha), -sin(alpha)), tg::vec2(sin(alpha), cos(alpha)));
        tg::f32 randomScale = 0;
        auto max = 10.f;
        while (randomScale == 0)
            randomScale = max * uniform(rng, box1).x;
        auto scale = tg::mat2::from_cols(tg::vec2(randomScale, 0), tg::vec2(0, randomScale));
        auto translation = tg::translation(max * uniform(rng, box2) - tg::pos2::zero);

        // apply transformations on vertices
        for (tg::pos2& p : vertices)
        {
            p = translation * tg::pos2(rotation * scale * tg::vec2(p));
        }
        auto triangle = tg::triangle2(vertices[0], vertices[1], vertices[2]);

        // testing interpolation, lazy and must be exactly the same as passing all three scalars
        auto p0Lazy = tg::interpolate(triangle, 1.0f, 0.0f);
        auto p0 = tg::interpolate(triangle, 1.0f, 0.0f, 0.0f);
        CHECK(distance(p0, vertices[0]) == nx::approx(0.f).abs(0.001f));
        CHECK(distance(p0Lazy, p0) == nx::approx(0.f).abs(0.001f));

        auto p1Lazy = tg::interpolate(triangle, 0.0f, 1.0f);
        auto p1 = tg::interpolate(triangle, 0.0f, 1.0f, 0.0f);
        CHECK(distance(p1, vertices[1]) == nx::approx(0.f).abs(0.001f));
        CHECK(distance(p1Lazy, p1) == nx::approx(0.f).abs(0.001f));

        auto p2Lazy = tg::interpolate(triangle, 0.0f, 0.0f);
        auto p2 = tg::interpolate(triangle, 0.0f, 0.0f, 1.0f);
        CHECK(distance(p2, vertices[2]) == nx::approx(0.f).abs(0.001f));
        CHECK(distance(p2Lazy, p2) == nx::approx(0.f).abs(0.001f));

        auto third = 1.0f / 3.0f;
        auto result = tg::interpolate(triangle, third, third);
        auto center = tg::pos2::zero;
        for (auto v : vertices)
            center += tg::vec2(v) * third;
        CHECK(distance(center, result) == nx::approx(0.f).abs(0.001f));
    }


    {
        // 3D
        tg::f32 x = tg::abs(uniform(rng, box1)).x * 9 + 1;
        tg::pos3 vertices[] = {tg::pos3(0, 0, 0), tg::pos3(x, 0, 0), tg::pos3(x, x, 0)};

        auto alpha = tg::radians(tg::abs(uniform(rng, box1).x) * tg::pi_scalar<tg::f32>);
        auto rotation = rotation_x(alpha) * rotation_y(alpha) * rotation_z(alpha);

        tg::f32 randomScale = 0;
        auto max = 10.f;
        while (randomScale == 0)
            randomScale = max * uniform(rng, box1).x;

        auto rand_size = tg::size3(randomScale, randomScale, randomScale);
        auto scale = scaling(rand_size);

        auto translation = tg::translation(max * uniform(rng, box3) - tg::pos3::zero);

        for (auto& p : vertices)
        {
            tg::vec4 v = {p.x, p.y, p.z, 1};
            v = translation * rotation * scale * v;
            p = tg::pos3(v.x, v.y, v.z);
        }

        auto triangle = tg::triangle3(vertices[0], vertices[1], vertices[2]);

        // testing interpolation, lazy and must be exactly the same as passing all three scalars
        auto p0Lazy = tg::interpolate(triangle, 1.0f, 0.0f);
        auto p0 = tg::interpolate(triangle, 1.0f, 0.0f, 0.0f);
        CHECK(distance(p0, vertices[0]) == nx::approx(0.f).abs(0.001f));
        CHECK(distance(p0Lazy, p0) == nx::approx(0.f).abs(0.001f));

        auto p1Lazy = tg::interpolate(triangle, 0.0f, 1.0f);
        auto p1 = tg::interpolate(triangle, 0.0f, 1.0f, 0.0f);
        CHECK(distance(p1, vertices[1]) == nx::approx(0.f).abs(0.001f));
        CHECK(distance(p1Lazy, p1) == nx::approx(0.f).abs(0.001f));

        auto p2Lazy = tg::interpolate(triangle, 0.0f, 0.0f);
        auto p2 = tg::interpolate(triangle, 0.0f, 0.0f, 1.0f);
        CHECK(distance(p2, vertices[2]) == nx::approx(0.f).abs(0.001f));
        CHECK(distance(p2Lazy, p2) == nx::approx(0.f).abs(0.001f));

        auto third = 1.0f / 3.0f;
        auto result = tg::interpolate(triangle, third, third);
        auto center = tg::pos3::zero;
        for (auto v : vertices)
            center += tg::vec3(v) * third;
        CHECK(distance(center, result) == nx::approx(0.f).abs(0.001f));
    }

    auto t = uniform(rng, box1).x;
    {
        // 1D random
        {
            // pos
            auto p0 = uniform(rng, box1);
            auto p1 = uniform(rng, box1);

            auto result = tg::interpolate(p0, p1, t);
            CHECK(distance(result, p0 + t * (p1 - p0)) == nx::approx(0.f).abs(0.001f));
        }
        {
            // vec
            auto p0 = uniform(rng, box1);
            auto p1 = uniform(rng, box1);

            auto result = tg::pos1(tg::interpolate(tg::vec1(p0), tg::vec1(p1), t));

            auto wa = 1.0f - t;
            auto wb = t;
            auto wResult = tg::pos1(tg::interpolate(tg::vec1(p0), tg::vec1(p1), wa, wb));

            CHECK(distance(result, p0 + t * (p1 - p0)) == nx::approx(0.f).abs(0.001f));
            CHECK(distance(result, wResult) == nx::approx(0.f).abs(0.001f));
        }
        {
            // scalar
            auto p0 = uniform(rng, box1).x;
            auto p1 = uniform(rng, box1).x;

            auto wa = 1.0f - t;
            auto wb = t;
            auto wResult = tg::interpolate(p0, p1, wa, wb);

            auto result = tg::interpolate(p0, p1, t);
            CHECK(tg::abs(result - (p0 + t * (p1 - p0))) == nx::approx(0.f).abs(0.001f));
            CHECK(tg::abs(result - wResult) == nx::approx(0.f).abs(0.001f));
        }

        // 2D random
        {
            // pos
            auto p0 = uniform(rng, box2);
            auto p1 = uniform(rng, box2);

            auto wa = 1.0f - t;
            auto wb = t;
            auto wResult = tg::pos2(tg::interpolate(p0, p1, wa, wb));

            auto result = tg::interpolate(p0, p1, t);
            CHECK(distance(result, p0 + t * (p1 - p0)) == nx::approx(0.f).abs(0.001f));
            CHECK(distance(result, wResult) == nx::approx(0.f).abs(0.001f));
        }
        {
            // vec
            auto p0 = uniform(rng, box2);
            auto p1 = uniform(rng, box2);

            auto wa = 1.0f - t;
            auto wb = t;
            auto wResult = tg::pos2(tg::interpolate(tg::vec2(p0), tg::vec2(p1), wa, wb));

            auto result = tg::pos2(tg::interpolate(tg::vec2(p0), tg::vec2(p1), t));
            CHECK(distance(result, p0 + t * (p1 - p0)) == nx::approx(0.f).abs(0.001f));
            CHECK(distance(result, wResult) == nx::approx(0.f).abs(0.001f));
        }

        // 3D random
        {
            // pos
            auto p0 = uniform(rng, box3);
            auto p1 = uniform(rng, box3);

            auto wa = 1.0f - t;
            auto wb = t;
            auto wResult = tg::pos3(tg::interpolate(p0, p1, wa, wb));

            auto result = tg::interpolate(p0, p1, t);
            CHECK(distance(result, p0 + t * (p1 - p0)) == nx::approx(0.f).abs(0.001f));
            CHECK(distance(result, wResult) == nx::approx(0.f).abs(0.001f));
        }
        {
            // vec
            auto p0 = uniform(rng, box3);
            auto p1 = uniform(rng, box3);

            auto wa = 1.0f - t;
            auto wb = t;
            auto wResult = tg::pos3(tg::interpolate(tg::vec3(p0), tg::vec3(p1), wa, wb));

            auto result = tg::pos3(tg::interpolate(tg::vec3(p0), tg::vec3(p1), t));
            CHECK(distance(result, p0 + t * (p1 - p0)) == nx::approx(0.f).abs(0.001f));
            CHECK(distance(result, wResult) == nx::approx(0.f).abs(0.001f));
        }
    }
}
