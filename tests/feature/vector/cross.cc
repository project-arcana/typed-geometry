#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/matrix.hh>
#include <typed-geometry/feature/objects.hh>
#include <typed-geometry/feature/vector.hh>

FUZZ_TEST("Cross")(tg::rng& rng)
{
    auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));
    auto range2 = tg::aabb2(tg::pos2(-10), tg::pos2(10));
    {
        // 2d
        auto a = tg::vec2(uniform(rng, range2));
        auto b = tg::vec2(uniform(rng, range2));

        auto area = length(a) * length(b - project(b, a));

        auto c = cross(a, b);

        // TODO abs(.. rounds to integer?
        CHECK(tg::length(tg::vec1(c)) == nx::approx(area));
    }

    {
        // 3d
        // unit vectors, compute third unit vector after rotation by cross product
        auto unitX = tg::dir3::pos_x;
        auto unitY = tg::dir3::pos_y;

        // random rotation
        auto alpha = tg::degree(uniform(rng, 0.0f, 360.0f));
        auto undoAlpha = tg::degree(360.0f) - alpha;
        auto rotation = tg::rotation_x(alpha) * tg::rotation_y(alpha) * tg::rotation_z(alpha);

        // TODO use inverse once implemented for mat4
        auto undoRotation = tg::rotation_z(undoAlpha) * tg::rotation_y(undoAlpha) * tg::rotation_x(undoAlpha);

        // TODO simpler way to apply rotation to vector?
        unitX = normalize(tg::vec3(rotation * tg::vec4(unitX.x, unitX.y, unitX.z, 0.0f)));
        unitY = normalize(tg::vec3(rotation * tg::vec4(unitY.x, unitY.y, unitY.z, 0.0f)));

        auto unitZ_v = cross(unitX, unitY);

        CHECK(dot(unitZ_v, unitX) == nx::approx(0.f).abs(0.001f));
        CHECK(dot(unitZ_v, unitY) == nx::approx(0.f).abs(0.001f));

        auto unitZ = normalize(tg::vec3(undoRotation * tg::vec4(unitZ_v, 0.0f)));

        CHECK(unitZ.z == 1.f);
        CHECK(unitZ.x == nx::approx(0.f).abs(0.001f));
        CHECK(unitZ.y == nx::approx(0.f).abs(0.001f));
        CHECK(is_normalized(unitZ_v, 0.00001f));


        // random vectors
        auto vecX = tg::vec3(uniform(rng, range3));
        auto vecY = tg::vec3(uniform(rng, range3));

        auto B = length(vecY);
        auto H = length(vecX - project(vecX, vecY));
        auto areaOfParallelogram = B * H;

        auto vecZ = cross(vecX, vecY);

        // magnitude of new vector should be equal to area of parallelogram of first two vectors
        CHECK(length(vecZ) == nx::approx(areaOfParallelogram));

        CHECK(dot(vecZ, vecX) == nx::approx(0.f).abs(1e-3));
        CHECK(dot(vecZ, vecY) == nx::approx(0.f).abs(1e-3));
    }
}
