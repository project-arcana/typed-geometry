#include <nexus/test.hh>

#include <typed-geometry/tg-lean.hh>

#include <type_traits>

TEST("Usage")
{
    // structured binding
    {
        auto p = tg::pos3{1, 2, 3};

        auto [x, y, z] = p;

        CHECK(x == p.x);
        CHECK(y == p.y);
        CHECK(z == p.z);
    }
    {
        auto p = tg::vec4{1, 2, 3, 4};

        auto [x, y, z, w] = p;

        CHECK(x == p.x);
        CHECK(y == p.y);
        CHECK(z == p.z);
        CHECK(w == p.w);
    }
    {
        auto p = tg::dir3{0, 1, 0};

        auto [x, y, z] = p;

        CHECK(x == p.x);
        CHECK(y == p.y);
        CHECK(z == p.z);
    }
    {
        auto p = tg::isize2{1, 2};

        auto [w, h] = p;

        CHECK(w == p.width);
        CHECK(h == p.height);
    }
    {
        auto p = tg::comp2{1, 2};

        auto [c0, c1] = p;

        CHECK(c0 == p.comp0);
        CHECK(c1 == p.comp1);
    }
    {
        auto p = tg::color3{0.1f, 0.2f, 0.3f};

        auto [r, g, b] = p;

        CHECK(r == p.r);
        CHECK(g == p.g);
        CHECK(b == p.b);
    }
    {
        auto p = tg::color4{0.1f, 0.2f, 0.3f, 0.4f};

        auto [r, g, b, a] = p;

        CHECK(r == p.r);
        CHECK(g == p.g);
        CHECK(b == p.b);
        CHECK(a == p.a);
    }

    // deduction guides
    {
        auto v0 = tg::vec(1, 2, 3);
        auto v1 = tg::vec(1.0f, 2.0f, 3.0f, 4.5f);

        CHECK(v0 == tg::ivec3(1, 2, 3));
        CHECK(v1 == tg::vec4(1, 2, 3, 4.5f));

        static_assert(std::is_same<decltype(v0), tg::ivec3>::value);
        static_assert(std::is_same<decltype(v1), tg::vec4>::value);
    }
    {
        auto v0 = tg::pos(1, 2, 3);
        auto v1 = tg::pos(1.0f, 2.0f, 3.0f, 4.5f);

        CHECK(v0 == tg::ipos3(1, 2, 3));
        CHECK(v1 == tg::pos4(1, 2, 3, 4.5f));

        static_assert(std::is_same<decltype(v0), tg::ipos3>::value);
        static_assert(std::is_same<decltype(v1), tg::pos4>::value);
    }
    {
        auto v0 = tg::size(1, 2, 3);
        auto v1 = tg::size(1.0f, 2.0f, 3.0f, 4.5f);

        CHECK(v0 == tg::isize3(1, 2, 3));
        CHECK(v1 == tg::size4(1, 2, 3, 4.5f));

        static_assert(std::is_same<decltype(v0), tg::isize3>::value);
        static_assert(std::is_same<decltype(v1), tg::size4>::value);
    }

    // deduction guides (mixed)
    {
        auto v0 = tg::vec(1, 2.0f, 3);
        auto v1 = tg::pos(1.0f, 2);
        auto v2 = tg::size(2, 3.f, 4., 5);

        static_assert(std::is_same<decltype(v0), tg::vec3>::value);
        static_assert(std::is_same<decltype(v1), tg::pos2>::value);
        static_assert(std::is_same<decltype(v2), tg::dsize4>::value);

        CHECK(v0 == tg::vec3(1, 2, 3));
        CHECK(v1 == tg::pos2(1, 2));
        CHECK(v2 == tg::dsize4(2, 3, 4, 5));
    }

    // deduction guides (conversions)
    {
        auto v1 = tg::vec1(1);
        auto v2 = tg::ipos2(1, 2);
        auto v3 = tg::usize3(2, 3, 4);
        auto v4 = tg::dvec4(1, 2, 3, 4);

        auto w11 = tg::vec(v1);
        auto w12 = tg::pos(v1);
        auto w13 = tg::size(v1);

        auto w21 = tg::vec(v2);
        auto w22 = tg::pos(v2);
        auto w23 = tg::size(v2);

        auto w31 = tg::vec(v3);
        auto w32 = tg::pos(v3);
        auto w33 = tg::size(v3);

        auto w41 = tg::vec(v4);
        auto w42 = tg::pos(v4);
        auto w43 = tg::size(v4);

        static_assert(std::is_same<decltype(w11), tg::vec1>::value);
        static_assert(std::is_same<decltype(w12), tg::pos1>::value);
        static_assert(std::is_same<decltype(w13), tg::size1>::value);

        static_assert(std::is_same<decltype(w21), tg::ivec2>::value);
        static_assert(std::is_same<decltype(w22), tg::ipos2>::value);
        static_assert(std::is_same<decltype(w23), tg::isize2>::value);

        static_assert(std::is_same<decltype(w31), tg::uvec3>::value);
        static_assert(std::is_same<decltype(w32), tg::upos3>::value);
        static_assert(std::is_same<decltype(w33), tg::usize3>::value);

        static_assert(std::is_same<decltype(w41), tg::dvec4>::value);
        static_assert(std::is_same<decltype(w42), tg::dpos4>::value);
        static_assert(std::is_same<decltype(w43), tg::dsize4>::value);

        CHECK(v1 == tg::vec(w11));
        CHECK(v1 == tg::vec(w12));
        CHECK(v1 == tg::vec(w13));

        CHECK(v2 == tg::pos(w21));
        CHECK(v2 == tg::pos(w22));
        CHECK(v2 == tg::pos(w23));

        CHECK(v3 == tg::size(w31));
        CHECK(v3 == tg::size(w32));
        CHECK(v3 == tg::size(w33));

        CHECK(v4 == tg::vec(w41));
        CHECK(v4 == tg::vec(w42));
        CHECK(v4 == tg::vec(w43));
    }
}
