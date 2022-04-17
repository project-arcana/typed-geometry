#include <nexus/test.hh>

#include <vector>

#include <typed-geometry/feature/random.hh>

TEST("tg::shuffle")
{
    std::vector<int> v;
    std::vector<int> cnts;
    std::vector<bool> has;

    auto total = 25 * 1000;
    cnts.resize(5 * 5, 0);

    tg::rng rng;
    for (auto i = 0; i < total; ++i)
    {
        v = {1, 2, 3, 4, 5};
        has = {false, false, false, false, false};
        tg::shuffle(rng, v);
        for (size_t j = 0; j < v.size(); ++j)
        {
            auto val = size_t(v[j]);
            CHECK(1 <= val);
            CHECK(val <= 5);
            val--;
            CHECK(!has[val]); // duplicated value
            has[val] = true;
            cnts[j * 5 + val]++;
        }
    }

    for (auto c : cnts)
    {
        CHECK(c >= total / 5 * 0.9f);
        CHECK(c <= total / 5 * 1.1f);
    }
}
