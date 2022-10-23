#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/vector.hh>
#include <typed-geometry/feature/objects.hh>

TEST("Tangent")
{
    tg::triangle3 t_pos = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}};
    tg::triangle2 t_uv = {{0, 0}, {1, 0}, {0, 1}};

    auto t = tangent(t_pos, t_uv);

    CHECK(t == tg::vec3{1, 0, 0});

    auto n = normal_of(t_pos);

    auto tbn = tbn_matrix(t_pos, t_uv);

    CHECK(tbn[0] == t);
    CHECK(tbn[1] == cross(normal_of(t_pos), t));
    CHECK(tbn[2] == n);

    CHECK(tbn[1] == tg::vec3{0, 1, 0});

    auto t2d = project(t, tg::plane(n, 0.0f));

    CHECK(t2d.z == 0.0f);
}


FUZZ_TEST("Tangent")(tg::rng& rng)
{
    auto range2 = tg::aabb2(tg::pos2(-10), tg::pos2(10));
    auto range3 = tg::aabb3(tg::pos3(-10), tg::pos3(10));

    auto pos1 = uniform(rng, range3);
    auto pos2 = uniform(rng, range3);
    auto pos3 = uniform(rng, range3);

    auto uv1 = uniform(rng, range2);
    auto uv2 = uniform(rng, range2);

    auto constexpr min_dist = 3.0f;

    // find three positions with pairwise minimum distances
    while (distance(pos1, pos2) < min_dist)
        pos2 = uniform(rng, range3);
    while (distance(pos1, pos3) < min_dist || distance(pos2, pos3) < min_dist)
        pos3 = uniform(rng, range3);

    // define triangle positions
    auto tri_pos = tg::triangle3(pos1, pos2, pos3);

    auto tri_normal = normal_of(tri_pos);

    auto p21 = pos2 - pos1;
    auto p31 = pos3 - pos1;
    auto curAngle = tg::atan2(dot(cross(p21, p31), tri_normal), dot(p21, p31));

    // TODO: 2d rotation in tg
    auto ca = cos(curAngle);
    auto sa = sin(curAngle);
    auto r = tg::mat2::from_cols(tg::vec2{ca, sa}, tg::vec2{-sa, ca});

    // find two uv coordinates with pairwise minimum distance
    while (distance(uv1, uv2) < min_dist)
        uv2 = uniform(rng, range2);

    auto length_ratio = length(uv2 - uv1) / length(p21);

    // make uv frame orthogonal
    auto uv3 = uv1 + length_ratio * length(p31) * (r * normalize(uv2 - uv1));

    // define triangle uv coords
    auto tri_uv = tg::triangle2(uv1, uv2, uv3);

    // triangle tangent
    auto tbn = tbn_matrix(tri_pos, tri_uv);

    // tangent, bitangent and normal
    auto t = tbn[0];
    auto b = tbn[1];
    auto n = tbn[2];

    // In this scenario, uv coords are chosen such that uv frame is orthogonal
    // and thus tangent frame should be
    CHECK(tg::abs(dot(t, b)) == nx::approx(0.0f).abs(0.001f));

    // for the given triangle positions, all tbn vectors should be unit
    CHECK(length(t) == nx::approx(1.0f));
    CHECK(length(b) == nx::approx(1.0f));
    CHECK(length(n) == nx::approx(1.0f));

    // random point in triangle
    auto p = uniform(rng, tri_pos);
    auto coords = coordinates(tri_pos, p);

    auto coords_sum = coords[0] + coords[1] + coords[2];
    CHECK(coords_sum == nx::approx(1.0f));

    auto uv = interpolate(tri_uv, coords[0], coords[1], coords[2]);

    auto dis1 = (tg::uniform<bool>(rng) ? -1.0f : 1.0f) * uniform(rng, 0.5f, 1.0f);
    auto dis2 = (tg::uniform<bool>(rng) ? -1.0f : 1.0f) * uniform(rng, 0.5f, 1.0f);

    // move along the tangent direction
    auto p_t = p + dis1 * t;
    auto coords_t = coordinates(tri_pos, p_t);
    auto uv_t = interpolate(tri_uv, coords_t[0], coords_t[1], coords_t[2]);

    // move along the bitangent direction
    auto p_b = p + dis2 * b;
    auto coords_b = coordinates(tri_pos, p_b);
    auto uv_b = interpolate(tri_uv, coords_b[0], coords_b[1], coords_b[2]);

    // moving along tangent should not cange the v coordinate
    CHECK(uv.y == nx::approx(uv_t.y));

    // moving along bitangent should not change the u coodinate
    CHECK(uv.x == nx::approx(uv_b.x));

    // Check normal
    auto displace = uniform(rng, -1.0f, 1.0f);
    auto p_n = p + displace * n;
    CHECK(distance(p, p_n) == nx::approx(tg::abs(displace)));
}
