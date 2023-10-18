#include <nexus/ext/tg-approx.hh>
#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>

FUZZ_TEST("Triangle2 - BarycoordMatrix")(tg::rng& rng)
{
    auto const bb = tg::aabb2(-10, 10);

    auto const p0 = uniform(rng, bb);
    auto const p1 = uniform(rng, bb);
    auto const p2 = uniform(rng, bb);

    auto const t = tg::triangle(p0, p1, p2);

    if (area_of(t) < 0.1)
        return; // too unstable for now

    auto const M = tg::to_barycoord_matrix_of(t);

    CHECK(M * tg::vec3(p0, 1) == nx::approx(tg::vec3(1, 0, 0)).abs(0.01f));
    CHECK(M * tg::vec3(p1, 1) == nx::approx(tg::vec3(0, 1, 0)).abs(0.01f));
    CHECK(M * tg::vec3(p2, 1) == nx::approx(tg::vec3(0, 0, 1)).abs(0.01f));
    CHECK(M * tg::vec3(centroid_of(t), 1) == nx::approx(tg::vec3(1 / 3.f, 1 / 3.f, 1 / 3.f)).abs(0.01f));

    auto p = uniform(rng, t);
    auto b = M * tg::vec3(p, 1);

    CHECK(b.x >= -0.001f);
    CHECK(b.y >= -0.001f);
    CHECK(b.z >= -0.001f);
    CHECK(b.x <= 1.001f);
    CHECK(b.y <= 1.001f);
    CHECK(b.z <= 1.001f);
    CHECK(b.x + b.y + b.z == nx::approx(1.f).abs(0.01f));

    auto p_recon = tg::from_barycoord_matrix_of(t) * b;
    CHECK(tg::pos2(p_recon) == nx::approx(p).abs(0.01f));
    CHECK(p_recon.z == nx::approx(1.f).abs(0.01f));
}

FUZZ_TEST("Triangle3 - BarycoordMatrix")(tg::rng& rng)
{
    auto const bb = tg::aabb3(-10, 10);

    auto const p0 = uniform(rng, bb);
    auto const p1 = uniform(rng, bb);
    auto const p2 = uniform(rng, bb);

    auto const t = tg::triangle(p0, p1, p2);

    if (area_of(t) < 0.1)
        return; // too unstable for now

    auto const M = tg::to_barycoord_matrix_of(t);

    CHECK(M * tg::vec4(p0, 1) == nx::approx(tg::vec4(1, 0, 0, 0)).abs(0.001f));
    CHECK(M * tg::vec4(p1, 1) == nx::approx(tg::vec4(0, 1, 0, 0)).abs(0.001f));
    CHECK(M * tg::vec4(p2, 1) == nx::approx(tg::vec4(0, 0, 1, 0)).abs(0.001f));
    CHECK(M * tg::vec4(centroid_of(t), 1) == nx::approx(tg::vec4(1 / 3.f, 1 / 3.f, 1 / 3.f, 0)).abs(0.001f));

    auto p = uniform(rng, t);
    auto b = M * tg::vec4(p, 1);

    CHECK(b.x >= -0.001f);
    CHECK(b.y >= -0.001f);
    CHECK(b.z >= -0.001f);
    CHECK(b.x <= 1.001f);
    CHECK(b.y <= 1.001f);
    CHECK(b.z <= 1.001f);
    CHECK(b.w == nx::approx(0.f).abs(0.001f));
    CHECK(b.x + b.y + b.z == nx::approx(1.f).abs(0.001f));

    auto p_recon = tg::from_barycoord_matrix_of(t) * b;
    CHECK(tg::pos3(p_recon) == nx::approx(p).abs(0.1f));
    CHECK(p_recon.w == nx::approx(1.f));

    auto n = tg::normal_of(t);
    auto offset = uniform(rng, -10.f, 10.f);
    auto b2 = M * tg::vec4(p + n * offset, 1);

    CHECK(b2.x == nx::approx(b.x));
    CHECK(b2.y == nx::approx(b.y));
    CHECK(b2.z == nx::approx(b.z));
    CHECK(b2.w == nx::approx(offset));

    auto p_recon2 = tg::from_barycoord_matrix_of(t) * b2;
    CHECK(tg::pos3(p_recon2) == nx::approx(p + n * offset).abs(0.01f));
    CHECK(p_recon.w == nx::approx(1.f).abs(0.01f));
}

FUZZ_TEST("Triangle2 - TransformationFromTo")(tg::rng& rng)
{
    auto const bb = tg::aabb2(-10, 10);

    auto const p0 = uniform(rng, bb);
    auto const p1 = uniform(rng, bb);
    auto const p2 = uniform(rng, bb);

    auto const q0 = uniform(rng, bb);
    auto const q1 = uniform(rng, bb);
    auto const q2 = uniform(rng, bb);

    auto const t0 = tg::triangle(p0, p1, p2);
    auto const t1 = tg::triangle(q0, q1, q2);

    if (area_of(t0) < 0.01 || area_of(t1) < 0.01)
        return; // too small -> too unstable

    auto const M = tg::transformation_from_to(t0, t1);

    CHECK(M * p0 == nx::approx(q0).abs(0.1f).rel(0.01f));
    CHECK(M * p1 == nx::approx(q1).abs(0.1f).rel(0.01f));
    CHECK(M * p2 == nx::approx(q2).abs(0.1f).rel(0.01f));

    auto const b = tg::uniform_barycoords(rng);
    auto const p = t0[b];
    auto const q = t1[b];
    CHECK(M * p == nx::approx(q).abs(0.01f));
}

FUZZ_TEST("Triangle3 - TransformationFromTo")(tg::rng& rng)
{
    auto const bb = tg::aabb3(-10, 10);

    auto const p0 = uniform(rng, bb);
    auto const p1 = uniform(rng, bb);
    auto const p2 = uniform(rng, bb);

    auto const q0 = uniform(rng, bb);
    auto const q1 = uniform(rng, bb);
    auto const q2 = uniform(rng, bb);

    auto const t0 = tg::triangle(p0, p1, p2);
    auto const t1 = tg::triangle(q0, q1, q2);

    if (area_of(t0) < 0.01 || area_of(t1) < 0.01)
        return; // too small -> too unstable

    auto const M = tg::transformation_from_to(t0, t1);

    CHECK(M * p0 == nx::approx(q0).abs(0.1f).rel(0.01f));
    CHECK(M * p1 == nx::approx(q1).abs(0.1f).rel(0.01f));
    CHECK(M * p2 == nx::approx(q2).abs(0.1f).rel(0.01f));

    auto const b = tg::uniform_barycoords(rng);
    auto const p = t0[b];
    auto const q = t1[b];
    CHECK(M * p == nx::approx(q).abs(0.01f));
}
