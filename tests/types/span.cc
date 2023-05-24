#include <nexus/test.hh>

#include <typed-geometry/feature/basic.hh>
#include <typed-geometry/feature/vector.hh>

#include <vector>

TEST("span")
{
    std::vector<int> v = {1, 2, 3};
    static_assert(tg::is_container<decltype(v), int>);

    tg::span<int> s = v;

    CHECK(s.size() == 3);

    CHECK(s[0] == 1);
    CHECK(s[1] == 2);
    CHECK(s[2] == 3);

    s[1] = 7;

    CHECK(v[1] == 7);

    int a[4] = {4, 3, 2, 1};
    s = a;

    CHECK(s[0] == 4);
    CHECK(s.size() == 4);

    CHECK(tg::min_element(s) == 1);
    CHECK(tg::max_element(s) == 4);

    s = s.subspan(1, 2);
    CHECK(s[0] == 3);
    CHECK(s[1] == 2);

    CHECK(tg::min_element(s) == 2);
    CHECK(tg::max_element(s) == 3);
}
