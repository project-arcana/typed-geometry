#include <nexus/test.hh>

#include <vector>

TEST("pcg")
{
    tg::pcg rng;
    auto n0 = rng();
    auto n1 = rng();
    auto n2 = rng();

    CHECK(n0 != n1);
    CHECK(n1 != n2);
}

TEST("pcg constexpr")
{
    auto constexpr s = tg::pcg()() % 10;
    int arr[1 + s];
    arr[0] = 1;
    CHECK(arr[0] == 1);
}

TEST("uniform")
{
    tg::rng rng;

    std::vector<float> v;
    for (auto i = 0; i < 10000; ++i)
        v.push_back(uniform(rng, 0.0f, 1.0f));

    CHECK(tg::average(v) == nx::approx(0.5f).epsilon(0.01f));
    CHECK(tg::variance(v) == nx::approx(1 / 12.f).epsilon(0.01f));
    CHECK(tg::min_element(v) >= 0);
    CHECK(tg::max_element(v) <= 1);
}

TEST("gaussian")
{
    tg::rng rng;

    {
        std::vector<float> v;
        for (auto i = 0; i < 10000; ++i)
            v.push_back(tg::normal<float>(rng));

        CHECK(tg::average(v) == nx::approx(0.0f).epsilon(0.05f));
        CHECK(tg::variance(v) == nx::approx(1.0f).epsilon(0.05f));
        CHECK(tg::standard_deviation(v) == nx::approx(1.0f).epsilon(0.05f));
    }
    {
        std::vector<float> v;
        for (auto i = 0; i < 10000; ++i)
            v.push_back(tg::gaussian(rng, -17.5f, 6.f));

        CHECK(tg::average(v) == nx::approx(-17.5f).epsilon(0.05f));
        CHECK(tg::standard_deviation(v) == nx::approx(6.0f).epsilon(0.05f));
    }
}
