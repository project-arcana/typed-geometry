#include <nexus/ext/tg-approx.hh>
#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/matrix.hh>
#include <typed-geometry/feature/quat.hh>

#include <rich-log/log.hh>

FUZZ_TEST("Quaternion - Basics")(tg::rng& rng)
{
    auto const bb = tg::aabb3(-2, 2);

    auto const axis = tg::uniform<tg::dir3>(rng);
    auto const angle = tg::uniform<tg::angle>(rng);

    auto const mat = tg::rotation_around(axis, angle);
    auto const q = tg::quat::from_axis_angle(axis, angle);

    CHECK(length(q) == nx::approx(1.f));
    if (angle > 1_deg && angle < 359_deg) // axis not defined otherwise
        CHECK(axis_of(q) == nx::approx(axis));
    CHECK(angle_of(q) == nx::approx(angle).abs(0.001_radf));

    auto const v = uniform_vec(rng, bb);
    auto const vm = mat * v;
    auto const vq = q * v;

    CHECK(vm == nx::approx(vq));
    CHECK(mat == nx::approx(to_mat4(q)).abs(0.001f));

    auto const axis2 = tg::uniform<tg::dir3>(rng);
    auto const angle2 = tg::uniform<tg::angle>(rng);
    auto const q2 = tg::quat::from_axis_angle(axis2, angle2);
    auto const mat2 = tg::rotation_around(axis2, angle2);
    CHECK(mat2 == nx::approx(to_mat4(q2)));
    CHECK(length(q2) == nx::approx(1.f));
    CHECK((q * q2) * v == nx::approx(q * (q2 * v)).abs(0.0001f));
    CHECK(length(q * q2) == nx::approx(1.f));

    auto const m4q = tg::mat4(q);
    auto const m4q2 = tg::mat4(q2);
    CHECK(m4q * m4q2 == nx::approx(mat * mat2).abs(0.0001f));

    auto const mq = to_mat3(q);
    auto const mq2 = to_mat3(q2);
    CHECK(mq * mq2 == nx::approx(to_mat3(q * q2)));
    CHECK(det(mq) == nx::approx(1.f));

    // nlerp and slerp should always interpolate the shorter way
    auto const t = uniform(rng, 0.0f, 1.0f);
    auto const nq = nlerp(q, q2, t);
    auto const sq = nlerp(q, q2, t);
    CHECK(length(nq) == nx::approx(1.f));
    CHECK(length(sq) == nx::approx(1.f));
    CHECK(angle_between(q, nq) <= angle_between(q, q2));
    CHECK(angle_between(q2, nq) <= angle_between(q, q2));
    CHECK(angle_between(q, sq) <= angle_between(q, q2));
    CHECK(angle_between(q2, sq) <= angle_between(q, q2));
    CHECK(angle_between(q, q2) <= 180_deg);

    auto qr = tg::quat::from_rotation_matrix(mq);
    if (dot(qr, q) < 0)
        qr = -qr; // proper orientation

    CHECK(qr == nx::approx(q));
}
