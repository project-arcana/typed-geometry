#include <nexus/test.hh>

#include <typed-geometry/detail/optional.hh>

TEST("tg::optional")
{
    tg::optional<int> i;
    CHECK(!i.has_value());
    CHECK(i != 7);
    CHECK(i != 0);

    i = 7;
    CHECK(i.has_value());
    CHECK(i.value() == 7);
    CHECK(i == 7);

    i = {};
    CHECK(!i.has_value());
    CHECK(i != 7);
    CHECK(i != 0);

    i = 9;
    CHECK(i.has_value());
    CHECK(i == 9);

    tg::optional<int> j = 11;
    CHECK(j.has_value());
    CHECK(j == 11);

    i = {};
    CHECK(!i.has_value());

    i = j;
    CHECK(i.has_value());
    CHECK(i == 11);

    tg::optional<int> o;
    CHECK(!o.has_value());

    i = o;
    CHECK(!i.has_value());
}
