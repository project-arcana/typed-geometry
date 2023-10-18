#include <nexus/ext/tg-approx.hh>
#include <nexus/fuzz_test.hh>

#include <typed-geometry/tg-std.hh>

FUZZ_TEST("IntersectionTriangle3Triangle3")(tg::rng& rng)
{
    // a) Triangles in xz-plane and not intersecting
    auto bb_l_planar = tg::aabb3({-10, 0, -10}, {-0.1f, 0, 10});
    auto bb_r_planar = tg::aabb3({0.1f, 0, -10}, {10, 0, 10});

    auto ta = tg::triangle3(tg::uniform(rng, bb_l_planar), tg::uniform(rng, bb_l_planar), tg::uniform(rng, bb_l_planar));
    auto tb = tg::triangle3(tg::uniform(rng, bb_r_planar), tg::uniform(rng, bb_r_planar), tg::uniform(rng, bb_r_planar));

    CHECK(!intersects(ta, tb));

    // b) Tris in x-plane and intersecting
    auto centroid_ta = tg::centroid_of(ta);
    auto tc = tg::triangle3(tg::uniform(rng, bb_r_planar), tg::uniform(rng, bb_r_planar), centroid_ta);
    auto insec = tg::intersects(ta, tc);

    CHECK(intersects(ta, tc));

    // c) non-coplanar and not intersecting
    auto bb_l = tg::aabb3({-10, -10, -10}, {-0.1f, 10, 10});
    auto bb_r = tg::aabb3({0.1f, -10, -10}, {10, 10, 10});

    auto td = tg::triangle3(tg::uniform(rng, bb_l), tg::uniform(rng, bb_l), tg::uniform(rng, bb_l));
    auto te = tg::triangle3(tg::uniform(rng, bb_r), tg::uniform(rng, bb_r), tg::uniform(rng, bb_r));

    CHECK(!intersects(td, te));

    // d) non-coplanar and intersecting
    auto bb = tg::aabb3(-10, 10);

    auto tf = tg::triangle3(tg::uniform(rng, bb), tg::uniform(rng, bb), tg::uniform(rng, bb));
    auto tg_pos0 = tg::uniform(rng, bb);
    auto centroid_tf = tg::centroid_of(tf);
    auto p_elong_centroid = centroid_tf + (centroid_tf - tg_pos0);
    auto tg = tg::triangle3(tg_pos0, p_elong_centroid, tg::uniform(rng, bb));

    CHECK(intersects(tf, tg));
}

FUZZ_TEST("IntersectsSphere2in3Triangle3")(tg::rng& rng)
{
    auto circ = tg::sphere2in3({0, 0, 0}, 1.f, {0.f, 1.f, 0.f});
    auto above_circ = tg::aabb3({-0.5f, 0.1f, -0.5f}, {0.5f, 2.f, 0.5f});
    auto below_circ = tg::aabb3({-0.5f, -2.f, -0.5f}, {0.5f, -0.1f, -0.5f});
    auto env = tg::aabb3({-10.f, -10.f, -10.f}, {10.f, 10.f, 10.f});

    // a) triangle area intersects with circle
    auto pos0 = tg::pos3(-2.f, -0.5f, 0.f);
    auto pos1 = tg::pos3(2.f, -0.5f, 0.f);
    auto pos2 = tg::pos3(0.f, 1.f, 0.f);

    auto t1 = tg::triangle3(pos0, pos1, pos2);

    CHECK(tg::intersects(t1, circ));

    // b) at least one triangle edge intersects with circle
    pos0 = tg::uniform(rng, above_circ);
    pos1 = tg::uniform(rng, below_circ);
    pos2 = tg::uniform(rng, env);

    auto t2 = tg::triangle3(pos0, pos1, pos2);

    CHECK(tg::intersects(t2, circ));

    // c) circ in plane of triangle and overlapping
    pos0 = tg::uniform(rng, env);
    pos1 = tg::uniform(rng, env);
    pos2 = tg::uniform(rng, env);

    auto t3 = tg::triangle3(pos0, pos1, pos2);

    auto circ2 = tg::sphere2in3(tg::centroid_of(t3), tg::distance(pos0, pos1), tg::normalize(tg::cross(pos1 - pos0, pos2 - pos0)));

    CHECK(tg::intersects(t3, circ2));

    // d) no intersection (all triangle points above/below circ-plane)
    pos0 = tg::uniform(rng, above_circ);
    pos1 = tg::uniform(rng, above_circ);
    pos2 = tg::uniform(rng, above_circ);

    auto t4 = tg::triangle(pos0, pos1, pos2);

    CHECK(!tg::intersects(t4, circ));
}

// FIXME: flaky right now
FUZZ_TEST("IntersectsCone3Triangle3", disabled)(tg::rng& rng)
{
    auto circ = tg::sphere2in3({0, 0, 0}, 1.f, {0.f, 1.f, 0.f});
    auto cone = tg::cone3(circ, 2.f);

    tg::aabb3 cone_box = tg::aabb3({-0.5f, 0.1f, -0.5f}, {0.5f, 2.f, 0.5f});
    tg::aabb3 cone_box_low = tg::aabb3(cone_box.min - tg::vec3{0.f, 2.1f, 0.f}, cone_box.max - tg::vec3{0.f, 2.1f, 0.f});
    tg::aabb3 cone_box_ext = tg::aabb3(cone_box_low.min, cone_box.max);
    tg::aabb3 cone_box_left = tg::aabb3(cone_box.min - tg::vec3{1.1f, 0.f, 0.f}, cone_box.max - tg::vec3{1.1f, 0.f, 0.f});

    // a) triangle intersects with cone base
    auto pos0 = tg::uniform(rng, cone_box);
    auto pos1 = tg::uniform(rng, cone_box_low);
    auto pos2 = tg::uniform(rng, cone_box_ext);

    auto t1 = tg::triangle(pos0, pos1, pos2);

    CHECK(tg::intersects(t1, cone));

    // b) area of triangle intersects with cone, but not the base
    auto pos0_1 = tg::pos3{0.f, 1.f, -1.f};
    auto pos1_1 = tg::pos3{0.f, 1.f, 1.f};
    auto pos2_1 = tg::uniform(rng, cone_box_left);

    auto t2 = tg::triangle(pos0_1, pos1_1, pos2_1);

    CHECK(tg::intersects(t2, cone));

    // c) no intersection (all triangle points below cone-base)
    pos0 = tg::uniform(rng, cone_box_low);
    pos1 = tg::uniform(rng, cone_box_low);
    pos2 = tg::uniform(rng, cone_box_low);

    auto t3 = tg::triangle(pos0, pos1, pos2);

    CHECK(!intersects(t3, cone));
}

FUZZ_TEST("IntersectsTriangle3Halfspace3")(tg::rng& rng)
{
    // normal points away from the halfspace
    tg::halfspace3 hs = tg::halfspace3({0.f, 1.f, 0.f}, tg::pos3::zero);
    auto inside_hs = tg::aabb3({-10.f, -10.f, -10.f}, {10.f, 0.f, 10.f});
    auto outside_hs = tg::aabb3({-10.f, 0.1f, -10.f}, {10.f, 10.f, 10.f});

    // a)
    auto pos0 = tg::uniform(rng, inside_hs);
    auto pos1 = tg::uniform(rng, inside_hs);
    auto pos2 = tg::uniform(rng, inside_hs);

    auto t1 = tg::triangle(pos0, pos1, pos2);

    CHECK(intersects(t1, hs));

    // b)
    auto pos0_2 = tg::uniform(rng, inside_hs);
    auto pos1_2 = tg::uniform(rng, outside_hs);
    auto pos2_2 = tg::uniform(rng, outside_hs);

    auto t2 = tg::triangle3(pos0_2, pos1_2, pos2_2);

    CHECK(intersects(t2, hs));

    // c)
    auto pos0_3 = tg::uniform(rng, outside_hs);
    auto pos1_3 = tg::uniform(rng, outside_hs);
    auto pos2_3 = tg::uniform(rng, outside_hs);

    auto t3 = tg::triangle3(pos0_3, pos1_3, pos2_3);

    CHECK(!intersects(t3, hs));
}

FUZZ_TEST("IntersectsPlane3Cone3")(tg::rng& rng)
{
    // xz-plane
    auto plane = tg::plane3(tg::dir3(0.f, 1.f, 0.f), tg::pos3(0.f, 0.f, 0.f));
    auto on_plane = tg::aabb3({-10.f, .0f, -10.f}, {10.f, .0f, 10.f});
    auto below_plane = tg::aabb3({-10.f, -10.f, -10.f}, {10.f, -0.1f, 10.f});
    auto above_plane = tg::aabb3({-10.f, 0.1f, 10.f}, {10.f, 10.f, 10.f});
    auto scalar_range = tg::aabb1{0.5, 5.f};

    // a) cone base intersecting with plane
    tg::pos3 circ_pos = tg::uniform(rng, on_plane);
    auto rad = tg::uniform(rng, scalar_range).x;
    auto shift_range = tg::aabb1(.0f, rad - 0.1f);
    circ_pos += tg::uniform<tg::dir3>(rng) * tg::uniform(rng, shift_range).x;

    auto circ1 = tg::sphere2in3(circ_pos, rad, {1.f, 0.f, 0.f});
    auto cone1 = tg::cone3(circ1, 3.f);

    CHECK(intersects(plane, circ1));
    CHECK(intersects(plane, cone1));

    // b) cone base and apex on different sides of the plane
    tg::pos3 circ_pos2 = tg::uniform(rng, on_plane);
    auto rad2 = tg::uniform(rng, scalar_range).x;
    auto shift = tg::uniform(rng, scalar_range).x;
    circ_pos2 += plane.normal * shift;

    auto shift_range2 = tg::aabb1(shift + 0.1f, 2 * shift);
    auto circ2 = tg::sphere2in3(circ_pos2, rad2, tg::dir3(.0f, 1.f, .0f));
    auto cone2 = tg::cone3(circ2, -tg::uniform(rng, shift_range2).x);

    CHECK(!intersects(plane, circ2));
    CHECK(intersects(plane, cone2));
}

FUZZ_TEST("IntersectsSphere2in3Plane3")(tg::rng& rng)
{
    // xz-plane
    auto plane = tg::plane3(tg::dir3(0.f, 1.f, 0.f), tg::pos3(0.f, 0.f, 0.f));
    auto scalar_range = tg::aabb1{0.5, 5.f};
    auto env_range = tg::aabb3{tg::pos3(-10.f, -10.f, -10.f), tg::pos3(10.f, 10.f, 10.f)};
    auto rotation_range = tg::aabb1(0.f, 90.f);

    // a) circle parallel to plane -> no intersection
    auto n = tg::uniform<tg::dir3>(rng);
    auto plane_pos = tg::uniform(rng, env_range);
    auto plane1 = tg::plane3(n, plane_pos);
    auto circ1 = tg::sphere2in3(plane_pos + n * tg::uniform(rng, scalar_range).x, tg::uniform(rng, scalar_range).x, n);

    CHECK(!intersects(circ1, plane1));

    // b) sphere center on plane -> intersection exists
    auto plane_pos2 = tg::uniform(rng, env_range);
    auto plane2 = tg::plane3(n, plane_pos2);

    auto rot_mat = tg::rotation_mat3_of(tg::rotation_around(tg::degree(tg::uniform(rng, rotation_range).x), tg::uniform<tg::dir3>(rng)));
    tg::vec3 non_parallel_n = rot_mat * n;
    tg::dir3 n_orth = tg::normalize(tg::cross(n, non_parallel_n));

    auto radius2 = tg::uniform(rng, scalar_range).x;
    auto shift_range = tg::aabb1(0.1f, radius2);
    auto circ2 = tg::sphere2in3(plane_pos2 + tg::uniform<tg::dir3>(rng) * tg::uniform(rng, shift_range).x, radius2, n_orth);

    CHECK(intersects(circ2, plane2));
}

FUZZ_TEST("IntersectsTriangle3Sphere3")(tg::rng& rng)
{
    // unit sphere
    tg::sphere3 s = tg::sphere3::unit;
    tg::aabb1 scale_range = tg::aabb1{2.f * s.radius, 5.f};

    // a) triangle inside sphere
    auto pos0 = tg::uniform(rng, s);
    auto pos1 = tg::uniform(rng, s);
    auto pos2 = tg::uniform(rng, s);

    auto t1 = tg::triangle3(pos0, pos1, pos2);

    CHECK(tg::intersects(t1, s));

    // b) no point inside the sphere but intersection exists
    // shift point outside the sphere
    tg::vec3 shift = tg::uniform<tg::dir3>(rng) * tg::uniform(rng, scale_range).x;
    tg::vec3 shift_orthogonal = {shift.x, -shift.y, shift.z};

    pos0 += shift;
    pos1 -= shift;
    pos2 += shift_orthogonal;

    auto t2 = tg::triangle3(pos0, pos1, pos2);

    CHECK(tg::intersects(t2, s));

    // c) no point inside the sphere and no intersection exists
    // shift t2 in normal direction outside the sphere
    auto n_t3 = tg::normalize(tg::cross(t2.pos1 - t2.pos0, t2.pos2 - t2.pos0));

    pos0 += n_t3 * 2 * s.radius;
    pos1 += n_t3 * 2 * s.radius;
    pos2 += n_t3 * 2 * s.radius;

    auto t3 = tg::triangle3(pos0, pos1, pos2);

    CHECK(!tg::intersects(t3, s));
}

FUZZ_TEST("IntersectsSegment3Sphere3")(tg::rng& rng)
{
    // unit sphere
    tg::sphere3 s = tg::sphere3::unit;

    { // a) segment points inside sphere
        auto p0_s1 = tg::uniform(rng, s);
        auto p1_s1 = tg::uniform(rng, s);
        tg::segment3 seg1 = {p0_s1, p1_s1};

        CHECK(tg::intersects(seg1, s));
    }

    { // b) one segment point inside the sphere and one outside
        auto p0_s2 = tg::uniform(rng, s);
        // point outside of the sphere
        auto p1_s2 = tg::uniform(rng, s) + tg::uniform<tg::dir3>(rng) * s.radius * 2;
        tg::segment3 seg2 = {p0_s2, p1_s2};

        CHECK(tg::intersects(seg2, s));
    }

    { // c) both segment points outside of the sphere
        auto p0_s3 = tg::uniform(rng, s) + tg::uniform<tg::dir3>(rng) * s.radius * 2;
        auto p1_s3 = tg::uniform(rng, s) + tg::uniform<tg::dir3>(rng) * s.radius * 2;
        tg::segment3 seg3 = {p0_s3, p1_s3};

        // line as extension of the segment
        auto line_seg3 = tg::line3(p0_s3, tg::normalize(p1_s3 - p0_s3));

        // check if closest intersection point of segment line is a point between p0_s3 & p1_s3
        // if this is the case and the point is inside the sphere -> intersection exists
        auto param = tg::closest_intersection_parameter(line_seg3, s);

        bool insec = false;
        if (param.has_value())
        {
            if (param.value() <= tg::length(seg3) && param.value() >= 0)
                insec = true;
        }

        CHECK(tg::intersects(seg3, s) == insec);
    }
}

FUZZ_TEST("IntersectsBox3Halfspace3")(tg::rng& rng)
{
    tg::aabb1 scalar_range = tg::aabb1(1.0f, 5.0f);

    // halfspace with random orientation through origin
    tg::halfspace3 hs = tg::halfspace3(tg::uniform<tg::dir3>(rng), tg::pos3::zero);

    // box with random halfextents and center at origin
    auto b = tg::box3();
    b.center = tg::pos3::zero;
    b.half_extents[0] = tg::uniform(rng, scalar_range).x * tg::uniform<tg::dir3>(rng);
    b.half_extents[1].x = tg::uniform(rng, scalar_range).x;
    b.half_extents[1].y = tg::uniform(rng, scalar_range).x;
    b.half_extents[1].z = (-b.half_extents[0].x * b.half_extents[1].x - b.half_extents[0].y * b.half_extents[1].y) / b.half_extents[0].z;
    b.half_extents[2] = tg::cross(b.half_extents[0], b.half_extents[1]);

    // rad of circumcircle
    float rad_circum = tg::distance(b.center + b.half_extents[0] + b.half_extents[1] + b.half_extents[2], b.center);
    float min_dist = tg::min(tg::length(b.half_extents[0]), tg::length(b.half_extents[1]), tg::length(b.half_extents[2]));

    { // a) box entirely inside the halfspace
        auto b1 = tg::box3();
        b1.half_extents = b.half_extents;
        b1.center = b.center - hs.normal * tg::uniform(rng, {rad_circum + 0.1f, rad_circum * 2});

        CHECK(tg::intersects(b1, hs));
    }

    { // b) box only partially inside the halfspace
        auto b2 = tg::box3();
        b2.half_extents = b.half_extents;
        b2.center = b.center + hs.normal * tg::uniform(rng, {0.0f, min_dist - 0.1f});

        CHECK(tg::intersects(b2, hs));
    }

    { // c) box not intersecting the halfspace
        auto b3 = tg::box3();
        b3.half_extents = b.half_extents;
        b3.center = b.center + hs.normal * tg::uniform(rng, {rad_circum + 0.1f, rad_circum * 2});

        CHECK(!tg::intersects(b3, hs));
    }
}

FUZZ_TEST("IntersectsSphere3Halfspace3")(tg::rng& rng)
{
    tg::aabb1 range_radius = tg::aabb1{0.5f, 5.0f};
    tg::halfspace3 hs = tg::halfspace3(tg::uniform<tg::dir3>(rng), tg::pos3::zero);

    { // a) sphere entirely in halfspace
        tg::sphere3 s1 = tg::sphere3(tg::pos3::zero, tg::uniform(rng, range_radius).x);

        // shift sphere inside the halfspace
        s1.center += -hs.normal * (s1.radius + 10.0f * tg::epsilon<float>);

        CHECK(tg::intersects(s1, hs));
    }

    { // b) sphere only partially inside of the halfspace
        tg::sphere3 s2 = tg::sphere3(tg::pos3::zero, tg::uniform(rng, range_radius).x);
        s2.center += -hs.normal * tg::uniform(rng, {0.0f, s2.radius});

        CHECK(tg::intersects(s2, hs));
    }

    { // c) sphere not intersecting the halfspace
        tg::sphere3 s3 = tg::sphere3(tg::pos3::zero, tg::uniform(rng, range_radius).x);
        // shift sphere in normal direction of halfspace
        s3.center += hs.normal * tg::uniform(rng, {s3.radius + 0.1f, 2 * s3.radius});

        CHECK(!tg::intersects(s3, hs));
    }
}

TEST("IntersectsSegment2Sphere2")
{
    tg::sphere2 s = tg::sphere2({0.0f, 0.0f}, 1.0f);

    { // a) both seg points inside the sphere
        tg::pos2 p1_s1 = {0.2f, -0.5f};
        tg::pos2 p2_s1 = {0.4f, 0.4f};
        tg::segment2 seg1 = {p1_s1, p2_s1};

        CHECK(tg::intersects(seg1, s));
    }

    { // b) one seg point inside the sphere and one outside
        tg::pos2 p1_s2 = {0.2f, -0.5f};
        tg::pos2 p2_s2 = {1.0f, 1.0f};
        tg::segment2 seg2 = {p1_s2, p2_s2};

        CHECK(tg::intersects(seg2, s));
    }

    { // c) both seg points outside of the sphere but intersection exists
        tg::pos2 p1_s3 = {1.0f, 1.0f};
        tg::pos2 p2_s3 = {-1.0f, -1.0f};
        tg::segment2 seg3 = {p1_s3, p2_s3};

        CHECK(tg::intersects(seg3, s));
    }
}

FUZZ_TEST("IntersectsBox2Sphere2")(tg::rng& rng)
{
    // 2D unit sphere
    tg::sphere2 s = tg::sphere2({0, 0}, 1.0f);
    tg::aabb2 inside_sphere = tg::aabb2({-0.5f, -0.5f}, {0.5f, 0.5f});

    { // a) box center inside the sphere
        auto c_b1 = tg::uniform(rng, inside_sphere);
        auto b1 = tg::box2::unit_centered;
        b1.center = c_b1;

        CHECK(tg::intersects(b1, s));
    }

    { // b) box vertex inside the sphere
        auto b2 = tg::box2::unit_centered;
        auto c_b2 = tg::uniform(rng, inside_sphere);
        c_b2 += 0.5f * tg::normalize(c_b2 - tg::pos2::zero);
        b2.center = c_b2;

        CHECK(tg::intersects(b2, s));
    }

    { // c) circle inside box, but box center not inside the sphere
        auto b3 = tg::box2();
        b3.half_extents[0] = {2.0f, 0.0f};
        b3.half_extents[1] = {0.0f, 2.0f};
        b3.center = s.center;

        CHECK(tg::intersects(b3, s));
    }

    { // d) no box vertice inside the sphere but intersec of one box edge with sphere
        tg::aabb1 shift_range = tg::aabb1{1.01f, tg::sqrt(2.0f) - tg::epsilon<float>};

        auto dir_1 = tg::uniform<bool>(rng) == false ? -1 : 1;
        auto shift_x = dir_1 * tg::vec2(tg::uniform(rng, shift_range).x, 0.0f);
        auto dir_2 = tg::uniform<bool>(rng) == false ? -1 : 1;
        auto shift_y = dir_2 * tg::vec2(0.0f, tg::sqrt(2.0f) - shift_x.x);

        auto b4 = tg::box2();
        b4.center = s.center + shift_x + shift_y;
        b4.half_extents[0] = {2.0f, 0.0f};
        b4.half_extents[1] = {0.0f, 2.0f};

        CHECK(tg::intersects(b4, s));
    }
}


FUZZ_TEST("IntersectsBox3Triangle3")(tg::rng& rng)
{
    tg::aabb3 env = {{-5.0f, -5.0f, -5.0f}, {5.0f, 5.0f, 5.0f}};
    tg::aabb1 scalar_range = tg::aabb1(-5.0f, 5.0f);
    tg::aabb1 pos_scalar_range = tg::aabb1(0.0f, 5.0f);

    tg::box3 b = tg::box3::unit_centered;

    { // a) triangle inside of the box
        auto pos1_t1 = tg::uniform(rng, b);
        auto pos2_t1 = tg::uniform(rng, b);
        auto pos3_t1 = tg::uniform(rng, b);
        auto t1 = tg::triangle3(pos1_t1, pos2_t1, pos3_t1);

        CHECK(tg::intersects(b, t1));
    }

    { // b) at least one point inside the box
        auto pos0_t2 = tg::uniform(rng, b);
        auto pos1_t2 = tg::uniform(rng, env);
        auto pos2_t2 = tg::uniform(rng, env);
        auto t2 = tg::triangle3(pos0_t2, pos1_t2, pos2_t2);

        CHECK(tg::intersects(b, t2));
    }

    { // c) box entirely above plane spanned by t1
        auto t2 = tg::triangle3(tg::uniform(rng, b), tg::uniform(rng, b), tg::uniform(rng, b));
        tg::plane3 p = tg::plane_of(t2);
        auto b2 = tg::box3::unit_centered;

        if (p.dis >= 0)
            b2.center = b2.center + (p.dis + tg::uniform(rng, pos_scalar_range).x + 1.0f + 10.0f * tg::epsilon<float>)*p.normal;

        else
            b2.center += (p.dis - tg::uniform(rng, pos_scalar_range).x - 1.0f - 10.0f * tg::epsilon<float>)*p.normal;

        CHECK(!tg::intersects(b2, t2));
    }
}

FUZZ_TEST("IntersectsBox3Plane3")(tg::rng& rng)
{
    tg::aabb1 scalar_range = tg::aabb1{-5.0f, 5.0f};
    tg::aabb1 short_scalar_range = tg::aabb1{-2.0f, 2.0f};
    tg::aabb1 rot = tg::aabb1{-180.0f, 180.0f};
    tg::aabb3 bb = {{-10.0f, -10.0f, -10.0f}, {10.0f, 10.0f, 10.0f}};
    tg::plane3 p = tg::plane3(tg::dir3(0, 1, 0), tg::pos3(0, 0, 0));

    tg::aabb3 above_plane = {{-10.0f, p.dis + 10 * tg::epsilon<float>, -10.0f}, {10.0f, 10.0f + p.dis, 10.0f}};
    tg::aabb3 below_plane = {{-10.0f, p.dis - 10.0f, -10.0f}, {10.0f, p.dis - 10 * tg::epsilon<float>, 10.0f}};
    tg::aabb3 around_plane = {{-10.0f, -2.0f + 10.0f * tg::epsilon<float>, -10.0f}, {10.0f, 2.0f - 10.0f * tg::epsilon<float>, 10.0f}};

    { // a) box entirely above plane -> no intersection
        auto center_b1 = tg::uniform(rng, above_plane);
        tg::mat3 box1_he;

        // random box orientation
        box1_he[0] = tg::uniform(rng, scalar_range).x * tg::uniform<tg::dir3>(rng);
        box1_he[1].x = tg::uniform(rng, scalar_range).x;
        box1_he[1].y = tg::uniform(rng, scalar_range).x;
        box1_he[1].z = (-box1_he[0].x * box1_he[1].x - box1_he[0].y * box1_he[1].y) / box1_he[0].z;
        box1_he[2] = tg::cross(box1_he[0], box1_he[1]);
        // auto max_extent = tg::max(tg::length(box1_he[0]), tg::length(box1_he[1]), tg::length(box1_he[2]));
        auto extent_th = tg::length(box1_he[0]) + tg::length(box1_he[1]) + tg::length(box1_he[2]);

        // ensure that box is above of the plane:
        tg::box3 b1 = tg::box3(center_b1 + (extent_th + 10 * tg::epsilon<float>)*p.normal, box1_he);

        CHECK(!tg::intersects(b1, p));
    }

    { // b) intersection exists
        auto center_b2 = tg::uniform(rng, around_plane);
        tg::mat3 b2_he;

        // random box orientation
        b2_he[0] = (2.0f + 10.0f * tg::epsilon<float>)*tg::uniform<tg::dir3>(rng);
        b2_he[1].x = 2.0f;
        b2_he[1].y = tg::uniform(rng, short_scalar_range).x;
        b2_he[1].z = (-b2_he[0].x * b2_he[1].x - b2_he[0].y * b2_he[1].y) / b2_he[0].z;
        b2_he[2] = tg::cross(b2_he[0], b2_he[1]);

        // length of each extent is at least 2
        CHECK(tg::length(b2_he[0]) >= 2.0f);
        CHECK(tg::length(b2_he[1]) >= 2.0f);
        CHECK(tg::length(b2_he[2]) >= 2.0f);

        tg::box3 b2 = tg::box3(center_b2, b2_he);

        // b2 and plane should intersect
        CHECK(tg::intersects(b2, p));
    }
}

FUZZ_TEST("IntersectsSphere3Plane3")(tg::rng& rng)
{
    // xz-plane
    tg::plane3 p = tg::plane3(tg::dir3(0.0f, 1.0f, 0.0f), {0.0f, 0.0f, 0.0f});
    tg::aabb3 above_plane = {{-15.0f, 0.1f, -15.0f}, {15.0f, 0.1f, 15.0f}};
    tg::aabb1 range_radius = tg::aabb1{0.1f, 10.0f};

    { // a) rng sphere with center above the plane
        auto sphere_center = tg::uniform(rng, above_plane);
        auto radius = tg::uniform(rng, range_radius).x;
        tg::sphere3 s1 = tg::sphere3(sphere_center, radius);

        bool insec = sphere_center.y <= radius ? true : false;
        CHECK(tg::intersects(p, s1) == insec);
    }

    { // b) rng sphere and tangential plane
        auto sphere_center2 = tg::uniform(rng, above_plane);
        tg::sphere3 s2 = tg::sphere3(sphere_center2, tg::abs(sphere_center2.y));

        CHECK(tg::intersects(s2, p));
    }
}

// FIXME: this test is flaky by design due to floating point issues, nx::max_error_rate might be a possible solution
FUZZ_TEST("IntersectsBox3Box3", disabled)(tg::rng& rng)
{
    // rng aabbs
    auto short_scalar_range = tg::aabb1(0.0f, 5.0f);
    auto scalar_range = tg::aabb1(-9.0f, 9.0f);
    auto range3D = tg::aabb3(tg::pos3(-15.0f), tg::pos3(15.0f));
    auto range_rotation = tg::aabb1(0.0f, 360.0f);

    { // a) non-intersecting boxes should not intersect when same translation is applied to both boxes
        auto mat1 = tg::mat3();
        mat1[0] = {tg::uniform(rng, short_scalar_range).x, 0, 0};
        mat1[1] = {0, tg::uniform(rng, short_scalar_range).x, 0};
        mat1[2] = {0, 0, tg::uniform(rng, short_scalar_range).x};

        // box1 and box2 should not overlap
        auto box1 = tg::box3(tg::pos3(0, 0, 0), mat1);
        auto box2 = tg::box3(tg::pos3(5, 5, 5), mat1);

        auto range1 = tg::aabb3(tg::pos3(-10.0), tg::pos3(10.0));

        // random translation
        auto trans = uniform(rng, range1);
        auto model_matrix = tg::translation(trans);

        box1.center = model_matrix * box1.center;
        box2.center = model_matrix * box2.center;

        CHECK(!tg::intersects(box1, box2));
    }

    { // b) invariant to rotation when circumspheres of boxes do not intersect

        // random box half extents
        auto m1 = tg::mat3();
        auto m2 = tg::mat3();

        m1[0] = tg::uniform(rng, scalar_range).x * tg::uniform<tg::dir3>(rng);
        m1[1].x = tg::uniform(rng, scalar_range).x;
        m1[1].y = tg::uniform(rng, scalar_range).x;
        m1[1].z = (-m1[0].x * m1[1].x - m1[0].y * m1[1].y) / m1[0].z;
        m1[2] = tg::cross(m1[0], m1[1]);
        float rad_circumsphere1 = tg::max(tg::length(m1[0]), tg::length(m1[1]), tg::length(m1[2])); // radius of circumsphere of box3

        m2[0] = tg::uniform(rng, scalar_range).x * tg::uniform<tg::dir3>(rng);
        m2[1].x = tg::uniform(rng, scalar_range).x;
        m2[1].y = tg::uniform(rng, scalar_range).x;
        m2[1].z = (-m2[0].x * m2[1].x - m2[0].y * m2[1].y) / m2[0].z;
        m2[2] = tg::cross(m2[0], m2[1]);
        float rad_circumsphere2 = tg::max(tg::length(m2[0]), tg::length(m2[1]), tg::length(m2[2])); // radius of circumsphere of box4

        auto offset = (rad_circumsphere1 + rad_circumsphere2 + 0.001f) * tg::uniform<tg::dir3>(rng);

        // box3 and box4 should not intersect no matter of any rotation, (similar) translation
        auto box3 = tg::box3(tg::pos3(0, 0, 0), m1);
        auto box4 = tg::box3(tg::pos3(0, 0, 0) + offset, m2);

        // random rotation per box
        auto rot1 = tg::rotation_around(tg::uniform<tg::dir3>(rng), tg::degree(tg::uniform(rng, range_rotation).x));
        auto rot2 = tg::rotation_around(tg::uniform<tg::dir3>(rng), tg::degree(tg::uniform(rng, range_rotation).x));

        for (auto u = 0; u < 3; u++)
        {
            box3.half_extents[u] = tg::vec3(rot1 * tg::vec4(box3.half_extents[u], 1.0f));
            box4.half_extents[u] = tg::vec3(rot2 * tg::vec4(box4.half_extents[u], 1.0f));
        }

        CHECK(!tg::intersects(box3, box4));
    }

    { // c) rigid transformation invariance (rotation + translation)
        auto box5_he = tg::mat3();
        auto box6_he = tg::mat3();

        // random box orientations
        box5_he[0] = tg::uniform(rng, scalar_range).x * tg::uniform<tg::dir3>(rng);
        box5_he[1].x = tg::uniform(rng, scalar_range).x;
        box5_he[1].y = tg::uniform(rng, scalar_range).x;
        box5_he[1].z = (-box5_he[0].x * box5_he[1].x - box5_he[0].y * box5_he[1].y) / box5_he[0].z;
        box5_he[2] = tg::cross(box5_he[0], box5_he[1]);

        box6_he[0] = tg::uniform(rng, scalar_range).x * tg::uniform<tg::dir3>(rng);
        box6_he[1].x = tg::uniform(rng, scalar_range).x;
        box6_he[1].y = tg::uniform(rng, scalar_range).x;
        box6_he[1].z = (-box6_he[0].x * box6_he[1].x - box6_he[0].y * box6_he[1].y) / box6_he[0].z;
        box6_he[2] = tg::cross(box6_he[0], box6_he[1]);

        auto box5 = tg::box3(tg::uniform(rng, range3D), box5_he);
        auto box6 = tg::box3(tg::uniform(rng, range3D), box6_he);

        // store if box5 and box6 intersect before applying translation and rotation
        bool insec = tg::intersects(box5, box6);

        // rng translation
        tg::vec3 translation_vector = tg::vec3(tg::uniform(rng, range3D));
        // rng rotation-axis
        auto rotation_axis = tg::uniform<tg::dir3>(rng);
        auto rotation = tg::rotation_around(rotation_axis, tg::degree(tg::uniform(rng, range_rotation).x));

        box5.center += translation_vector;
        box6.center += translation_vector;

        for (auto v = 0; v < 3; v++)
        {
            box5.half_extents[v] = tg::vec3(rotation * tg::vec4(box5.half_extents[v], 1.0f));
            box6.half_extents[v] = tg::vec3(rotation * tg::vec4(box6.half_extents[v], 1.0f));
        }

        CHECK(tg::intersects(box5, box6) == insec);
    }
}

// FIXME: this test is flaky by design due to floating point issues, nx::max_error_rate might be a possible solution
FUZZ_TEST("IntersectsBox2Box2", disabled)(tg::rng& rng)
{
    auto short_scalar_range = tg::aabb1(0.0f, 5.0f);
    auto range2D = tg::aabb2(tg::pos2(-10.0f), tg::pos2(10.0f));
    auto range_rotation = tg::aabb1(90.0f, 360.0f);

    auto box1_half_extents = tg::mat2();
    auto box2_half_extents = tg::mat2();

    // rng box orientation
    box1_half_extents[0] = {tg::uniform(rng, short_scalar_range).x, 0.0f};
    box1_half_extents[1] = {0.0f, tg::uniform(rng, short_scalar_range).x};
    auto rad_circumsphere1 = tg::max(tg::length(box1_half_extents[0]), tg::length(box1_half_extents[1]));

    box2_half_extents[0] = {tg::uniform(rng, short_scalar_range).x, 0.0f};
    box2_half_extents[1] = {0.0f, tg::uniform(rng, short_scalar_range).x};
    auto rad_circumsphere2 = tg::max(tg::length(box2_half_extents[0]), tg::length(box2_half_extents[1]));

    auto offset = (rad_circumsphere1 + rad_circumsphere2 + 0.001f) * tg::uniform<tg::dir2>(rng);

    auto box1 = tg::box2(tg::uniform(rng, range2D), box1_half_extents);
    auto box2 = tg::box2(box1.center + offset, box2_half_extents);

    // store if box1 and box2 intersect before applying translation and rotation
    auto insec = tg::intersects(box1, box2);

    // applying rigid transformation (rotation + translation)
    auto translation = tg::vec2(tg::uniform(rng, range2D));
    auto rotation_angle = tg::degree(tg::uniform(rng, range_rotation).x);

    box1.center += translation;
    box2.center += translation;

    for (auto i = 0; i < 2; i++)
    {
        box1.half_extents[i] = tg::rotate(box1_half_extents[i], rotation_angle);
        box2.half_extents[i] = tg::rotate(box2_half_extents[i], rotation_angle);
    }

    // rotate center
    box1.center = tg::rotate(box1.center, rotation_angle);
    box2.center = tg::rotate(box2.center, rotation_angle);

    CHECK(intersects(box1, box2) == insec);
}

TEST("IntersectsBox3Sphere3")
{
    tg::sphere3 s1 = tg::sphere3(tg::pos3(0.0f, 0.0f, 0.0f), 5.0f);
    tg::box3 b1 = tg::box3(tg::aabb3({-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}));

    // b1 is located entirely inside s1. Thus, they should intersect.
    CHECK(tg::intersects(s1, b1));

    tg::box3 b2 = tg::box3(tg::aabb3({5.0f, 0, 0}, {8.0f, 8.0f, 8.0f}));

    // b2 intersects s1 in one point
    CHECK(tg::intersects(s1, b2));

    tg::box3 b3 = tg::box3(tg::aabb3({5.0f + 10 * tg::epsilon<float>, 0, 0}, {8.0f, 8.0f, 8.0f}));

    // b3 and s1 do not intersect
    CHECK(!tg::intersects(s1, b3));
}

TEST("IntersectsSphere3Sphere3")
{
    tg::sphere3 s1 = tg::sphere3(tg::pos3(0, 0, 0), 5.0f);
    tg::sphere3 s2 = tg::sphere3(tg::pos3(6.0, 0, 0), 1.0f);

    // s1 and s2 should intersect (in exactly one point)
    CHECK(tg::intersects(s1, s2));

    tg::sphere3 s3 = tg::sphere3(tg::pos3(0, 0, 0), 2.0f);

    // s3 is inside of s1 -> s3 and s1 should intersect
    CHECK(tg::intersects(s1, s3));

    tg::sphere3 s4 = tg::sphere3(tg::pos3(6.0f + 10 * tg::epsilon<float>, 0, 0), 1.0f);

    // s1 and s4 should not intersect
    CHECK(!tg::intersects(s1, s4));
}
