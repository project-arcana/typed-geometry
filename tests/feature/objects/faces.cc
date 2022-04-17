#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>

FUZZ_TEST("FacesPreserveArea")(tg::rng& rng)
{
    const auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));
    const auto pos30 = uniform(rng, range3);
    const auto pos31 = uniform(rng, range3);

    const auto d30 = tg::uniform<tg::dir3>(rng);
    const auto d31 = any_normal(d30);
    const auto d32 = normalize(cross(d30, d31));
    auto m3 = tg::mat3();
    m3[0] = d30 * uniform(rng, 1.0f, 3.0f);
    m3[1] = d31 * uniform(rng, 1.0f, 3.0f);
    m3[2] = d32 * uniform(rng, 1.0f, 3.0f);

    auto const test_face_areas = [](const auto& o)
    {
        const auto areaObj = area_of(o);
        auto areaSum = 0.f;
        for (const auto& face : faces_of(o))
            areaSum += area_of(face);
        CHECK(areaSum == nx::approx(areaObj));
    };

    test_face_areas(aabb_of(pos30, pos31));
    test_face_areas(tg::box3(pos30, m3));
}

FUZZ_TEST("FacesOfPyramids")(tg::rng& rng)
{
    const auto h = uniform(rng, 0.0f, 10.0f);

    const auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));
    const auto pos30 = uniform(rng, range3);
    const auto pos31 = uniform(rng, range3);
    const auto pos32 = uniform(rng, range3);

    const auto d30 = tg::uniform<tg::dir3>(rng);
    const auto d31 = any_normal(d30);
    auto m23 = tg::mat2x3();
    m23[0] = d30 * uniform(rng, 1.0f, 3.0f);
    m23[1] = d31 * uniform(rng, 1.0f, 3.0f);

    const auto pyTri = tg::pyramid<tg::triangle3>(tg::triangle3(pos30, pos31, pos32), h);
    const auto facesPyTri = faces_of(pyTri);
    static_assert(std::is_same_v<decltype(facesPyTri.base), tg::triangle3>);
    static_assert(facesPyTri.mantle.size() == 3);

    const auto pyBox = tg::pyramid<tg::box2in3>(tg::box2in3(pos30, m23), h);
    const auto facesPyBox = faces_of(pyBox);
    static_assert(std::is_same_v<decltype(facesPyBox.base), tg::box2in3>);
    static_assert(facesPyBox.mantle.size() == 4);

    /* This structured binding should also work, but there are apparently problems with clang
    const auto pyBox = tg::pyramid<tg::box2in3>(tg::box2in3(pos30, m23), h);
    auto [baseFace, triangleFaces] = faces_of(pyBox);
    static_assert(std::is_same_v<decltype(baseFace), tg::box2in3>);
    CHECK(triangleFaces.size() == 4);
    */

    CHECK(true);
}
