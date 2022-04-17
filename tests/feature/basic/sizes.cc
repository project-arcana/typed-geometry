#include <nexus/test.hh>

#include <typed-geometry/tg-lean.hh>

TEST("Sizes")
{
    CHECK(sizeof(tg::vec3) == 3 * sizeof(float));
    CHECK(sizeof(tg::ivec4) == 4 * sizeof(int));
    CHECK(sizeof(tg::dpos3) == 3 * sizeof(double));
    CHECK(sizeof(tg::mat3) == 3 * 3 * sizeof(float));
    CHECK(sizeof(tg::umat3x4) == 3 * 4 * sizeof(unsigned));
    CHECK(sizeof(tg::dmat4x3) == 4 * 3 * sizeof(double));
    CHECK(sizeof(tg::mat2x3) == 2 * 3 * sizeof(float));
    CHECK(sizeof(tg::mat2x4) == 2 * 4 * sizeof(float));
    CHECK(sizeof(tg::quat) == 4 * sizeof(float));

    static_assert(sizeof(tg::mat2x4) == 2 * 4 * sizeof(float), "");
}
