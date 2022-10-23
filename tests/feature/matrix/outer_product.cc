#include <nexus/ext/tg-approx.hh>
#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/matrix.hh>
#include <typed-geometry/feature/vector.hh>

FUZZ_TEST("OuterProduct")(tg::rng& rng)
{
    auto min = -10.0f;
    auto max = 10.0f;

    auto box1 = tg::aabb1(tg::pos1(min), tg::pos1(max));
    auto box2 = tg::aabb2(tg::pos2(min), tg::pos2(max));
    auto box3 = tg::aabb3(tg::pos3(min), tg::pos3(max));
    auto box4 = tg::aabb4(tg::pos4(min), tg::pos4(max));

    // 4D
    {
        // test case
        {
            auto x = tg::vec4(1, 3, 2, 4);
            auto y = tg::vec4(2, 1, 0, 3);

            auto m = outer_product(x, y);

            auto res = tg::mat4();
            res[0] = tg::vec4(2, 6, 4, 8);
            res[1] = tg::vec4(1, 3, 2, 4);
            res[2] = tg::vec4::zero;
            res[3] = tg::vec4(3, 9, 6, 12);

            CHECK(m == nx::approx(res));
        }

        auto x = tg::vec4(uniform(rng, box4));
        auto y = tg::vec4(uniform(rng, box4));
        auto z = uniform(rng, box1).x * x; // is x scaled

        auto zero = tg::vec4::zero;

        // commutativity
        CHECK(transpose(outer_product(y, x)) == nx::approx(outer_product(x, y)));

        // as u is x multiplied by random
        CHECK(outer_product(z, x) == nx::approx(outer_product(x, z)));

        // distributivity
        CHECK(outer_product(x, y + z) == nx::approx(outer_product(x, y) + outer_product(x, z)));

        // scalar multiplication
        auto lambda = uniform(rng, box1).x;
        CHECK(outer_product(x, y) * lambda == nx::approx(outer_product(lambda * x, y)));

        // zero
        CHECK(outer_product(x, zero) == nx::approx(tg::mat4::zero));
    }

    // 3D
    {
        // test case
        {
            auto x = tg::vec3(1, 3, 2);
            auto y = tg::vec3(2, 1, 0);

            auto m = outer_product(x, y);

            auto res = tg::mat3();
            res[0] = tg::vec3(2, 6, 4);
            res[1] = tg::vec3(1, 3, 2);
            res[2] = tg::vec3::zero;

            CHECK(m == nx::approx(res));
        }

        auto x = tg::vec3(uniform(rng, box3));
        auto y = tg::vec3(uniform(rng, box3));
        auto z = uniform(rng, box1).x * x; // is x scaled

        auto zero = tg::vec3::zero;

        // commutativity
        CHECK(transpose(outer_product(y, x)) == nx::approx(outer_product(x, y)));

        // as u is x multiplied by random
        CHECK(outer_product(z, x) == nx::approx(outer_product(x, z)));

        // distributivity
        CHECK(outer_product(x, y + z) == nx::approx(outer_product(x, y) + outer_product(x, z)));

        // scalar multiplication
        auto lambda = uniform(rng, box1).x;
        CHECK(outer_product(x, y) * lambda == nx::approx(outer_product(lambda * x, y)));

        // zero
        CHECK(outer_product(x, zero) == nx::approx(tg::mat3::zero));
    }

    // 2D
    {
        // test case
        {
            auto x = tg::vec2(1, 3);
            auto y = tg::vec2(2, 1);

            auto m = outer_product(x, y);

            auto res = tg::mat2();
            res[0] = tg::vec2(2, 6);
            res[1] = tg::vec2(1, 3);

            CHECK(m == nx::approx(res));
        }

        auto x = tg::vec2(uniform(rng, box2));
        auto y = tg::vec2(uniform(rng, box2));
        auto z = uniform(rng, box1).x * x; // is x scaled

        auto zero = tg::vec2::zero;

        // commutativity
        CHECK(transpose(outer_product(y, x)) == nx::approx(outer_product(x, y)));

        // as u is x multiplied by random
        CHECK(outer_product(z, x) == nx::approx(outer_product(x, z)));

        // distributivity
        CHECK(outer_product(x, y + z) == nx::approx(outer_product(x, y) + outer_product(x, z)));

        // scalar multiplication
        auto lambda = uniform(rng, box1).x;
        CHECK(outer_product(x, y) * lambda == nx::approx(outer_product(lambda * x, y)));

        // zero
        CHECK(outer_product(x, zero) == nx::approx(tg::mat2::zero));
    }

    // 1D
    {
        // test case
        {
            auto x = tg::vec1(1);
            auto y = tg::vec1(2);

            auto m = outer_product(x, y);

            auto res = tg::mat1();
            res[0] = tg::vec1(2);

            CHECK(m == nx::approx(res));
        }

        auto x = tg::vec1(uniform(rng, box1));
        auto y = tg::vec1(uniform(rng, box1));
        auto z = uniform(rng, box1).x * x; // is x scaled

        auto zero = tg::vec1::zero;

        // commutativity
        CHECK(transpose(outer_product(y, x)) == nx::approx(outer_product(x, y)));

        // as u is x multiplied by random
        CHECK(outer_product(z, x) == nx::approx(outer_product(x, z)));

        // distributivity
        CHECK(outer_product(x, y + z) == nx::approx(outer_product(x, y) + outer_product(x, z)));

        // scalar multiplication
        auto lambda = uniform(rng, box1).x;
        CHECK(outer_product(x, y) * lambda == nx::approx(outer_product(lambda * x, y)));

        // zero
        CHECK(outer_product(x, zero) == nx::approx(tg::mat1::zero));
    }
}
