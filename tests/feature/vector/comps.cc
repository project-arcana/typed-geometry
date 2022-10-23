#include <nexus/test.hh>

#include <typed-geometry/feature/vector.hh>

#include <array>
#include <vector>

static_assert(tg::is_comp_convertible<tg::vec3, int>, "");
static_assert(tg::is_comp_convertible<tg::vec3, float>, "");
static_assert(tg::is_comp_convertible<tg::vec3, double>, "");

TEST("comp construction")
{
    {
        auto ref_comp = tg::comp3(1, 2, 3);
        int ref_array[] = {1, 2, 3};
        static_assert(tg::detail::comp_size<decltype(ref_array)>::value == 3, "");

        CHECK(ref_comp == tg::comp3(tg::vec3(1, 2, 3)));
        CHECK(ref_comp == tg::comp3(tg::size3(1, 2, 3)));
        CHECK(ref_comp == tg::comp3(tg::pos3(1, 2, 3)));
        CHECK(ref_comp == tg::comp3(tg::comp3(1, 2, 3)));
        CHECK(ref_comp == tg::comp3(std::array<int, 3>{{1, 2, 3}}));
        CHECK(ref_comp == tg::comp3(std::vector<int>{1, 2, 3}));
        CHECK(ref_comp == tg::comp3(ref_array));
    }
    {
        auto ref_comp = tg::comp4(1, 2, 3, 0);
        int ref_array[] = {1, 2, 3};
        static_assert(tg::detail::comp_size<decltype(ref_array)>::value == 3, "");

        CHECK(ref_comp == tg::comp4(tg::vec3(1, 2, 3)));
        CHECK(ref_comp == tg::comp4(tg::size3(1, 2, 3)));
        CHECK(ref_comp == tg::comp4(tg::pos3(1, 2, 3)));
        CHECK(ref_comp == tg::comp4(tg::ipos3(1, 2, 3)));
        CHECK(ref_comp == tg::comp4(tg::comp3(1, 2, 3)));
        CHECK(ref_comp == tg::comp4(std::array<int, 3>{{1, 2, 3}}));
        CHECK(ref_comp == tg::comp4(std::vector<int>{1, 2, 3}));
        CHECK(ref_comp == tg::comp4(ref_array));
    }
}
