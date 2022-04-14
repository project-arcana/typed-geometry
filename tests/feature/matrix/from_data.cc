#include <nexus/test.hh>

#include <typed-geometry/feature/vector.hh>
#include <typed-geometry/tg-lean.hh>

TEST("mat::from_data")
{
    float vals[] = {
        1, 2, 3, //
        0, 1, 1,
    };
    auto M = tg::mat3x2::from_data_rowwise(vals);
    CHECK(M[2][0] == 3);
    CHECK(M * tg::vec3(2, 3, 4) == tg::vec2(1 * 2 + 2 * 3 + 3 * 4, 3 + 4));

    auto M2 = tg::mat3x2::from_data_colwise(vals);
    CHECK(M2[2][0] == 1);
    CHECK(M2 * tg::vec3(2, 3, 4) == tg::vec2(2 + 3 * 3 + 4, 2 * 2 + 4));

    M = tg::mat3x2::from_data_rowwise({1, 2, 3, 0, 1, 1});
    CHECK(M[2][0] == 3);
    CHECK(M * tg::vec3(2, 3, 4) == tg::vec2(1 * 2 + 2 * 3 + 3 * 4, 3 + 4));
}
