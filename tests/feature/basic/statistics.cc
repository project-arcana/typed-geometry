#include <nexus/test.hh>

#include <typed-geometry/feature/basic.hh>

#include <array>
#include <list>
#include <map>
#include <vector>

TEST("statistics basics")
{
    auto const range_test = [](auto&& r) {
        CHECK(tg::sum(r) == 10);
        CHECK(tg::sum<float>(r) == 10.f);
        CHECK(tg::sum(r, [](auto i) { return i * 2 - 1; }) == 16);

        CHECK(tg::mean(r) == 2);
        CHECK(tg::average(r) == 2);
        CHECK(tg::average<float>(r) == 2.5f);

        CHECK(tg::product(r) == 1 * 2 * 3 * 4);

        CHECK(tg::min_element(r) == 1);
        CHECK(tg::max_element(r) == 4);
    };

    auto r0 = std::vector<int>{1, 3, 4, 2};
    int r1[] = {2, 3, 1, 4};
    auto r2 = std::array<int, 4>{{2, 1, 3, 4}};
    auto r3 = std::list<int>{2, 3, 4, 1};
    auto r4 = tg::span(r0);

    range_test(r0);
    range_test(r1);
    range_test(r2);
    range_test(r3);
    range_test(r4);
}
