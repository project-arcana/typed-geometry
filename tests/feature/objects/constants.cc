#include <nexus/test.hh>

TEST("object constants")
{
    CHECK(volume_of(tg::aabb3::unit_centered) == 1.0f);
    CHECK(volume_of(tg::aabb3::unit_from_zero) == 1.0f);
    CHECK(volume_of(tg::aabb3::minus_one_to_one) == 8.0f);

    CHECK(centroid_of(tg::aabb3::unit_centered) == tg::pos3::zero);
    CHECK(centroid_of(tg::aabb3::unit_from_zero) == tg::pos3(0.5f));
    CHECK(centroid_of(tg::aabb3::minus_one_to_one) == tg::pos3::zero);

    CHECK(area_of(tg::sphere3::unit) == approx(12.5663706));
    CHECK(volume_of(tg::sphere3::unit) == approx(4.1887902f));

    CHECK(centroid_of(tg::sphere3::unit) == tg::pos3::zero);
    CHECK(centroid_of(tg::sphere3::unit) == tg::pos3::zero);
}
