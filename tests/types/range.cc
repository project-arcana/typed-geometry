#include <nexus/test.hh>

#include <initializer_list>
#include <iostream>

#include <typed-geometry/tg-std.hh>

template <int D>
static void check_range_equal(tg::range<D> r, std::initializer_list<typename tg::range<D>::comp_t> vals)
{
    size_t cnt = 0;
    for (auto c : r)
    {
        CHECK(c == vals.begin()[cnt]);

        CHECK(cnt < vals.size());
        ++cnt;
    }
    CHECK(cnt == vals.size());
}

TEST("range basics")
{
    check_range_equal(tg::range(0), {});
    check_range_equal(tg::range(1), {0});
    check_range_equal(tg::range(3), {0, 1, 2});
    check_range_equal(tg::range(-5), {});
    check_range_equal(tg::range(2, 5), {2, 3, 4});
    check_range_equal(tg::range(-10, -5), {-10, -9, -8, -7, -6});

    check_range_equal(tg::range2(2), {{0, 0}, {1, 0}, {0, 1}, {1, 1}});
    check_range_equal(tg::range2(tg::icomp2{2, 3}), {{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2}, {1, 2}});

    auto s = 0;
    for (auto i : tg::range(10))
        s += i;
    CHECK(s == 9 * 10 / 2);

    s = 0;
    for (auto [x, y] : tg::range2(10))
    {
        s += x;
        s += y;
    }
    CHECK(s == 9 * 10 * 10);
}