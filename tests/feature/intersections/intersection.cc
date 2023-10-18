#include <nexus/ext/tg-approx.hh>
#include <nexus/fuzz_test.hh>

#include <rich-log/log.hh>

#include <typed-geometry/tg-std.hh>

FUZZ_TEST("IntersectionSegment3AABB3")(tg::rng& rng)
{
    auto bb = tg::aabb3(-10.f, 10.f);
    auto bb_up = tg::aabb3({-10.f, 10.5f, -10.f}, {10.f, 20.f, 10.f});
    auto bb_low = tg::aabb3({-10.f, -20.f, -10.f}, {10.f, -10.5f, 10.f});

    { // a) segment inside the bounding box
        auto seg = tg::segment3(tg::uniform(rng, bb), tg::uniform(rng, bb));

        auto insec = tg::intersection(seg, bb);

        CHECK(insec.has_value());
        CHECK(tg::distance_sqr(insec.value().pos0, seg.pos0) == nx::approx(0.f));
        CHECK(tg::distance_sqr(insec.value().pos1, seg.pos1) == nx::approx(0.f));
    }

    { // b) one segment point inside the bounding box
        auto seg = tg::segment3(tg::uniform(rng, bb), tg::pos3(15.f, 15.f, 15.f));

        auto insec = tg::intersection(seg, bb);

        CHECK(insec.has_value());
    }

    { // c) both segment points outside, but intersecting the bounding box
        auto seg = tg::segment3(tg::uniform(rng, bb_up), tg::uniform(rng, bb_low));

        auto insec = tg::intersection(seg, bb);

        CHECK(insec.has_value());
    }

    { // d) both segment points outside and not intersecting the bounding box
        auto seg = tg::segment3(tg::uniform(rng, bb_up), tg::uniform(rng, bb_up));

        auto insec = tg::intersection(seg, bb);

        CHECK(!insec.has_value());
    }
}

FUZZ_TEST("IntersectionDisk3Plane3")(tg::rng& rng)
{
    auto plane_xz = tg::plane3({0.f, 1.f, 0.f}, tg::pos3::zero);

    { // a) xz-plane and orthogonal disk
        auto disk = tg::disk3(tg::pos3::zero, 1.f, {1.f, 0.f, 0.f});

        auto insec = tg::intersection(disk, plane_xz);

        CHECK(insec.has_value());
        CHECK(distance(insec.value().pos0, tg::pos3(0.f, 0.f, 1.f)) == nx::approx(0.f));
        CHECK(distance(insec.value().pos1, tg::pos3(0.f, 0.f, -1.f)) == nx::approx(0.f));
    }

    { // b) xz-plane and disk with random orientation - intersection through origin
        auto disk = tg::disk3(tg::pos3::zero, 1.f, tg::uniform<tg::dir3>(rng));

        bool insec_exists = !(disk.normal == plane_xz.normal);
        auto insec = tg::intersection(disk, plane_xz);

        CHECK(insec.has_value() == insec_exists);
        CHECK(tg::contains(insec.value(), tg::pos3::zero));
    }

    { // c) disk in plane - no intersection
        auto rng_dir = tg::uniform<tg::dir3>(rng);
        auto disk = tg::disk3(tg::pos3::zero, 1.f, rng_dir);
        auto plane = tg::plane3(rng_dir, tg::pos3::zero);

        auto insec = tg::intersection(disk, plane);

        CHECK(!insec.has_value());
    }
}


FUZZ_TEST("IntersectionPlane3Tube3")(tg::rng& rng)
{
    auto inf_cyl = tg::inf_cylinder3(tg::line3({0, 0, 0}, {0, 1.f, 0}), 1.f);
    auto cylinder_env = tg::aabb3({0.f, -5.f, 0.f}, {0.f, 5.f, 0.f});

    { // a) plane with random orientation through tube (intersection might not exist if exceeding tube on one side)
        auto p_pos = tg::uniform(rng, cylinder_env);
        auto normal = tg::uniform<tg::dir3>(rng);

        auto plane = tg::plane3(normal, p_pos);

        bool insec_exists = dot(normal, inf_cyl.axis.dir) == 0 ? false : true;

        auto insec = tg::intersection(plane, inf_cyl);

        if (insec.has_value())
            CHECK(distance(insec.value().center, p_pos) == nx::approx(0.f));
    }

    { // b) plane normal parallel to tub axis -> results in a circle/disk
        auto p_pos = tg::uniform(rng, cylinder_env);
        auto normal = inf_cyl.axis.dir;

        auto plane = tg::plane3(normal, p_pos);

        auto insec = tg::intersection(plane, inf_cyl);

        CHECK(insec.has_value());
        CHECK(tg::length_sqr(insec.value().semi_axes[0]) == tg::length_sqr(insec.value().semi_axes[1]));
    }
}

// FIXME: is flaky right now
FUZZ_TEST("IntersectionSegment3BoundaryObject", disabled)(tg::rng& rng)
{
    { // a) segment3 - cylinder_boundary3
        auto cylinder_b = tg::cylinder_boundary<3, float>({0.f, -2.f, 0.f}, {0.f, 2.f, 0.f}, 1.f);

        auto rng_pos = tg::uniform(rng, cylinder_b);
        auto rng_dir = tg::uniform<tg::dir3>(rng);

        auto pos0 = rng_pos + 4.1f * rng_dir;
        auto pos1 = rng_pos - 4.1f * rng_dir;
        auto seg = tg::segment3(pos0, pos1);

        auto insec = tg::intersection(seg, cylinder_b);

        CHECK(insec.any());
    }

    { // b) segment3 - box_boundary3
        // boundary of unit cube
        auto box_b = tg::box_boundary<3, float>(tg::pos3::zero, tg::mat3::identity);

        auto rng_pos = tg::uniform(rng, box_b);
        auto rng_dir = tg::uniform<tg::dir3>(rng);

        auto pos0 = rng_pos + 4.1f * rng_dir;
        auto pos1 = rng_pos - 4.1f * rng_dir;
        auto seg = tg::segment3(pos0, pos1);

        auto insec = tg::intersection(seg, box_b);

        CHECK(insec.any());
    }
}

FUZZ_TEST("IntersectionSegment3Tube3")(tg::rng& rng)
{
    auto tube = tg::tube3({{0.f, -1.f, 0.f}, {0.f, 1.f, 0.f}}, 1.f);
    auto scalar_range = tg::aabb1(1.1f, 3.f);

    { // a) intersection through origin
        auto orth_vec = tg::vec3(1.f, 0.f, 0.f);
        auto pos1 = tg::pos3::zero + tg::uniform(rng, scalar_range).x * orth_vec;
        auto pos2 = tg::pos3::zero - tg::uniform(rng, scalar_range).x * orth_vec;

        tg::segment3 seg = {pos1, pos2};

        auto insec = tg::intersection(seg, tube);

        CHECK(insec.any());
        CHECK(insec.size() == 2);
        CHECK(tg::distance_sqr(tg::pos3::zero, insec.first()) > 0.f);
        CHECK(tg::distance_sqr(tg::pos3::zero, insec.last()) > 0.f);
    }

    { // b) segment inside the tube
        auto cylinder = tg::cylinder3({{0.f, -1.f, 0.f}, {0.f, 1.f, 0.f}}, 1.f);
        auto pos1 = tg::uniform(rng, cylinder);
        auto pos2 = tg::uniform(rng, cylinder);

        tg::segment3 seg = {pos1, pos2};

        auto insec = tg::intersection(seg, tube);

        CHECK(!insec.any());
    }
}

FUZZ_TEST("IntersectionSphere3Plane3")(tg::rng& rng)
{
    auto sphere = tg::sphere3::unit;
    auto scalar_range = tg::aabb1(1.1f, 5.f);

    { // a) intersection (through center of sphere)
        tg::dir3 normal = tg::uniform<tg::dir3>(rng);
        auto plane = tg::plane3(normal, {0.f, 0.f, 0.f});

        auto insec = tg::intersection(sphere, plane);

        CHECK(insec.has_value());
        CHECK(tg::distance_sqr(insec.value().center, tg::pos3{0.f, 0.f, 0.f}) == nx::approx(0.f));
        CHECK(insec.value().radius == nx::approx(1.f));
    }

    { // b) no intersection
        auto normal = tg::uniform<tg::dir3>(rng);
        auto pos = tg::pos3::zero + tg::uniform(rng, scalar_range).x * normal;

        auto plane = tg::plane3(normal, pos);

        auto insec = tg::intersection(sphere, plane);

        CHECK(!insec.has_value());
    }
}

FUZZ_TEST("IntersectionSegment3Hemisphere3")(tg::rng& rng)
{
    // upper part of the unit sphere
    auto hemis = tg::hemisphere3({0.f, 0.f, 0.f}, 1.f, {0.f, 1.f, 0.f});
    auto below_hemis = tg::aabb3({-0.5f, -5.f, -0.5f}, {0.5f, -0.1f, 0.5f});
    auto above_hemis_base = tg::aabb3({-0.5f, 0.1f, -0.5f}, {0.5f, 5.f, 0.5f});
    auto scalar_range = tg::aabb1(1.f, 5.f);
    auto short_scalar_range = tg::aabb1(0.1f, 0.99f);
    auto sphere_hemis = tg::sphere3(hemis.center, hemis.radius);

    { // a) intersection with the circle base
        auto pos0 = tg::uniform(rng, below_hemis);
        auto pos1 = tg::uniform(rng, above_hemis_base);

        auto seg = tg::segment3{pos0, pos1};
        bool insec_exists = tg::length(pos1 - pos0) > 0.f;
        auto insec = tg::intersection(seg, hemis);

        CHECK(insec.has_value() == insec_exists);
    }

    { // b) intersection with sphere extension (and with hemisphere part)
        auto rng_dir = tg::uniform<tg::dir3>(rng);
        auto pos0 = hemis.center + tg::uniform(rng, scalar_range).x * rng_dir;
        auto pos1 = hemis.center - tg::uniform(rng, scalar_range).x * rng_dir;

        auto seg = tg::segment3{pos0, pos1};

        auto insec = tg::intersection(seg, hemis);

        CHECK(insec.has_value());
        CHECK(distance(insec.value(), tg::pos3::zero) == nx::approx(0.f));
    }

    { // c) intersection with sphere extension (but not with the hemisphere)
        auto rot_range = tg::aabb1(0.f, 89.f);
        auto rot_mat = tg::rotation_mat3_of(tg::rotation_z(tg::degree(tg::uniform(rng, rot_range).x)));

        tg::dir3 inv_normal_rot = tg::normalize(rot_mat * hemis.normal);

        auto pos0 = hemis.center - tg::uniform(rng, short_scalar_range).x * inv_normal_rot;
        auto pos1 = tg::uniform(rng, below_hemis);

        auto seg = tg::segment3{pos0, pos1};

        auto insec = tg::intersection(seg, hemis);

        CHECK(!insec.has_value());
    }

    { // d) both segment points inside the hemisphere
        auto pos0 = tg::uniform(rng, hemis);
        auto pos1 = tg::uniform(rng, hemis);

        auto seg = tg::segment3{pos0, pos1};

        auto insec = tg::intersection(seg, hemis);

        CHECK(insec.has_value());
        CHECK(distance(insec.value().pos0, pos0) == nx::approx(0.f));
        CHECK(distance(insec.value().pos1, pos1) == nx::approx(0.f));
    }
}

FUZZ_TEST("IntersectionSegment3Disk3")(tg::rng& rng)
{
    auto disk = tg::disk3({0.f, 0.f, 0.f}, 1.f, {0.f, 1.f, 0.f});
    auto below_circle = tg::aabb3({-5.f, -5.f, -5.f}, {5.f, -0.1f, 5.f});
    auto scalar_range = tg::aabb1(1.f, 5.f);

    { // a) both seg points below the circle -> no intersection
        tg::pos3 pos0 = tg::uniform(rng, below_circle);
        tg::pos3 pos1 = tg::uniform(rng, below_circle);

        auto seg = tg::segment3{pos0, pos1};

        auto insec = tg::intersection(seg, disk);

        CHECK(!insec.has_value());
    }

    { // b) one seg point below the circle and one above with intersection

        auto rng_dir = tg::uniform<tg::dir3>(rng);

        tg::pos3 pos0 = tg::pos3::zero + tg::uniform(rng, scalar_range).x * rng_dir;
        tg::pos3 pos1 = tg::pos3::zero - tg::uniform(rng, scalar_range).x * rng_dir;

        auto seg = tg::segment3{pos0, pos1};

        auto insec = tg::intersection(seg, disk);

        CHECK(insec.has_value());
        CHECK(distance(insec.value(), tg::pos3::zero) == nx::approx(0.f));
    }

    { // c) segment in plane of circle (degenerated)
        tg::pos3 pos0 = tg::pos3::zero + tg::uniform(rng, scalar_range).x * tg::vec3{1.f, 0, 1.f};
        tg::pos3 pos1 = tg::pos3::zero + tg::uniform(rng, scalar_range).x * tg::vec3{-1.f, 0, -1.f};

        auto seg = tg::segment3{pos0, pos1};

        auto insec = tg::intersection(seg, disk);

        CHECK(!insec.has_value());
    }
}

FUZZ_TEST("IntersectionSegment3Halfspace3")(tg::rng& rng)
{
    auto env = tg::aabb3({-10.f, -10.f, -10.f}, {10.f, 10.f, 10.f});
    auto scalar_range = tg::aabb1(1.f, 5.f);

    auto hs_pos = tg::uniform(rng, env);
    auto hs = tg::halfspace3(tg::uniform<tg::dir3>(rng), hs_pos);

    { // a) one point inside and one point outside of the halfspace
        tg::pos3 pos1 = hs_pos + (hs.normal * tg::uniform(rng, scalar_range).x);
        tg::pos3 pos2 = hs_pos - (hs.normal * tg::uniform(rng, scalar_range).x);

        auto seg = tg::segment3{pos1, pos2};

        auto insec = tg::intersection(seg, hs);

        CHECK(insec.has_value());
        CHECK(distance(insec.value().pos0, hs_pos) == nx::approx(0.f));
        CHECK(distance(insec.value().pos1, pos2) == nx::approx(0.f));
    }

    { // b) both points inside of the halfspace
        auto rng_dir = tg::uniform<tg::dir3>(rng);
        auto in_plane = tg::cross(rng_dir, hs.normal);

        tg::pos3 pos1 = hs_pos + tg::uniform(rng, scalar_range).x * in_plane - tg::uniform(rng, scalar_range).x * hs.normal;
        tg::pos3 pos2 = hs_pos - tg::uniform(rng, scalar_range).x * in_plane - tg::uniform(rng, scalar_range).x * hs.normal;

        auto seg = tg::segment3{pos1, pos2};

        auto insec = tg::intersection(seg, hs);

        CHECK(insec.has_value());
        CHECK(distance(insec.value().pos0, pos1) == nx::approx(0.f));
        CHECK(distance(insec.value().pos1, pos2) == nx::approx(0.f));
    }

    { // c) both points outside of the halfspace -> no intersection
        auto rng_dir = tg::uniform<tg::dir3>(rng);
        auto in_plane = tg::cross(rng_dir, hs.normal);

        tg::pos3 pos1 = hs_pos + tg::uniform(rng, scalar_range).x * in_plane + tg::uniform(rng, scalar_range).x * hs.normal;
        tg::pos3 pos2 = hs_pos - tg::uniform(rng, scalar_range).x * in_plane + tg::uniform(rng, scalar_range).x * hs.normal;

        auto seg = tg::segment3{pos1, pos2};

        auto insec = tg::intersection(seg, hs);

        CHECK(!insec.has_value());
    }
}

FUZZ_TEST("IntersectionSegment3Box3")(tg::rng& rng)
{
    // enlarged unit box
    tg::aabb3 bb = {{-2.0f, -2.0f, -2.0f}, {2.0f, 2.0f, 2.0}};
    tg::box3 box = tg::box3(bb);

    tg::aabb3 above_box = {{-2.0f, 2.5f, -2.0f}, {2.0f, 5.0f, 2.0f}};
    tg::aabb3 below_box = {{-2.0f, -5.0f, -2.0f}, {2.0f, -2.5f, 2.0f}};

    { // a) intersection exists, segment vertices outside of box
        auto p1 = tg::uniform(rng, above_box);
        auto p2 = tg::uniform(rng, below_box);
        tg::segment3 s1 = {p1, p2};

        CHECK(tg::intersection(s1, box).has_value());
    }

    { // b) One point of segment inside the box and one point outside of the box
        auto p3 = tg::uniform(rng, bb);
        auto p4 = tg::uniform(rng, below_box);
        tg::segment3 s2 = {p3, p4};

        auto insec2 = tg::intersection(s2, box);

        CHECK(insec2.has_value());
        CHECK(insec2.value().pos0 == s2.pos0);
    }

    { // c) both points inside of box
        auto p5 = tg::uniform(rng, bb);
        auto p6 = tg::uniform(rng, bb);
        tg::segment3 s3 = {p5, p6};

        auto insec3 = tg::intersection(s3, box);

        CHECK(insec3.has_value());
        CHECK(insec3.value().pos0 == s3.pos0);
        CHECK(insec3.value().pos1 == s3.pos1);
    }
}

FUZZ_TEST("IntersectionSegment3Triangle3")(tg::rng& rng)
{
    tg::aabb3 tbb = {{-10.0f, -10.0f, -10.0f}, {10.0f, 10.0f, 10.0f}};
    tg::triangle3 t1 = tg::triangle3(tg::uniform(rng, tbb), tg::uniform(rng, tbb), tg::uniform(rng, tbb));

    tg::vec3 normal_t1 = tg::cross(t1.pos1 - t1.pos0, t1.pos2 - t1.pos0);
    tg::pos3 center_t1 = (1.0f / 3.0f) * (t1.pos0 + t1.pos1 + t1.pos2);

    auto p1 = center_t1 + normal_t1;
    auto p2 = center_t1 - normal_t1;
    tg::segment s1 = {p1, p2};

    CHECK(tg::intersection(s1, t1).has_value());
}

FUZZ_TEST("IntersectionTriangle3Triangle3")(tg::rng& rng)
{
    tg::triangle3 t1 = {tg::pos3(-1.0f, 0.0f, 2.0f), tg::pos3(2.0f, 0.0f, -1.0f), tg::pos3(-1.0f, 0.0f, -1.0f)};

    { // a) intersection with only one segment of each triangle
        tg::triangle3 t2 = {tg::pos3(0.0f, -1.0f, 0.0f), tg::pos3(0.0f, 1.0f, 0.0f), tg::pos3(-5.0f, -1.0f, 0.0f)};

        auto insec = tg::intersection(t1, t2);

        CHECK(insec.has_value()); // How to further check if the segment is correct?
    }

    { // b) no intersection
        tg::aabb3 above_plane = tg::aabb3({-15.0f, 0.5f, -15.0f}, {15.0f, 15.0f, 15.0f});

        auto pos1 = tg::uniform(rng, above_plane);
        auto pos2 = tg::uniform(rng, above_plane);
        auto pos3 = tg::uniform(rng, above_plane);
        tg::triangle3 t3 = {pos1, pos2, pos3};

        CHECK(!tg::intersection(t1, t3).has_value());
    }

    { // c) intersection in one point
        tg::aabb3 tbb = {{-10.0f, 0.0f, -10.0f}, {10.0f, 0.0f, 10.0f}};
        // t4 in xz-plane
        tg::triangle3 t4 = {tg::uniform(rng, tbb), tg::uniform(rng, tbb), tg::uniform(rng, tbb)};
        auto n_t4 = tg::normalize(tg::cross(t4.pos1 - t4.pos0, t4.pos2 - t4.pos0));

        auto pos4 = t4.pos0 + tg::vec3(0, 2.0f, 0);
        auto pos5 = t4.pos0 - tg::vec3(0, 2.0f, 0);
        auto pos6 = t4.pos0 + tg::cross(t4.pos1 - t4.pos0, n_t4);

        tg::triangle3 t5 = tg::triangle(pos4, pos5, pos6);

        CHECK(tg::intersection(t4, t5).has_value());
    }
}

FUZZ_TEST("IntersectionSegment3ConvexShapes3")(tg::rng& rng)
{
    // segment3 - capsule3

    tg::pos3 origin = tg::pos3(0.0f);
    auto capsule = tg::capsule3({0, -1, 0}, {0, 1, 0}, 1.0);

    { // a) seg: both points outside, but intersection exists
        // intersection exists if segment passes (for ex.) through origin

        auto dir = tg::uniform<tg::dir3>(rng);
        // extent by factor -1 in other direction to get pos1 of the segment that passes through the origin with a random orientation
        auto pos0 = origin + tg::vec3(5 * dir);
        auto pos1 = origin - tg::vec3(5 * dir);
        tg::segment3 rng_segment = {pos0, pos1};

        CHECK(tg::intersection(rng_segment, capsule).has_value());
    }

    { // b) seg: both points outside, but no intersection exists
        tg::aabb1 midaxis_range = tg::aabb1(-0.9f, 0.9f);
        auto pos_midaxis = tg::pos3(0.0f, tg::uniform(rng, midaxis_range).x, 0.0f);
        auto rng_normal = tg::uniform<tg::dir2>(rng);
        tg::dir3 rng_normal3 = tg::dir3(rng_normal.x, 0.0f, rng_normal.y);

        auto pos_surface = pos_midaxis + 1.1f * rng_normal3;
        tg::dir3 tangent_vector = tg::dir3(-rng_normal.y, 0.0f, rng_normal.x);

        tg::aabb1 segment_range = tg::aabb1(0.0f, 10.0f);

        // points outside of capsule
        auto pos0 = pos_surface + tg::uniform(rng, segment_range).x * tangent_vector;
        auto pos1 = pos_surface - tg::uniform(rng, segment_range).x * tangent_vector;

        // segment parallel to the tangent at a surface point -> no intersection
        tg::segment3 rng_segment = {pos0, pos1};

        CHECK(!tg::intersection(rng_segment, capsule).has_value());
    }

    { // c) seg: both points inside
        auto pos0_2 = tg::uniform(rng, capsule);
        auto pos1_2 = tg::uniform(rng, capsule);
        tg::segment3 rng_segment_inside = {pos0_2, pos1_2};

        CHECK(tg::intersection(rng_segment_inside, capsule).has_value());
    }

    { // d) seg: one point inside, one point outside
        auto pos0 = tg::uniform(rng, capsule);

        // point outside of the capsule
        auto pos1 = tg::uniform(rng, capsule) + 2.0 * tg::uniform<tg::dir3>(rng);
        tg::segment3 rng_segment = {pos0, pos1};

        CHECK(tg::intersection(rng_segment, capsule).has_value());
    }
}

FUZZ_TEST("IntersectionPlane3Triangle3")(tg::rng& rng)
{
    // plane is the xy-plane
    tg::plane3 plane = tg::plane3(tg::dir3(0, 1, 0), tg::pos3(0, 0, 0));

    // random triangle with intersection
    tg::aabb3 above_plane = tg::aabb3({-15.0f, 0.0f, -15.0f}, {15.0f, 15.0f, 15.0f});
    tg::aabb3 below_plane = tg::aabb3({-15.0f, -15.0f, -15.0f}, {15.0f, 0.0f, 15.0f});

    { // a) one point above plane and two points below the plane
        auto pos0 = tg::uniform(rng, above_plane);
        auto pos1 = tg::uniform(rng, below_plane);
        auto pos2 = tg::uniform(rng, below_plane);

        tg::triangle3 triangle_1 = tg::triangle3(pos0, pos1, pos2);

        CHECK(tg::intersection(plane, triangle_1).has_value());
    }

    { // b) all points below plane
        auto pos0 = tg::uniform(rng, below_plane);
        auto pos1 = tg::uniform(rng, below_plane);
        auto pos2 = tg::uniform(rng, below_plane);

        tg::triangle3 triangle_2 = tg::triangle(pos0, pos1, pos2);

        CHECK(!tg::intersection(plane, triangle_2).has_value());
    }

    { // c) all points above plane
        auto pos0 = tg::uniform(rng, above_plane);
        auto pos1 = tg::uniform(rng, above_plane);
        auto pos2 = tg::uniform(rng, above_plane);

        tg::triangle3 triangle_3 = tg::triangle3(pos0, pos1, pos2);

        CHECK(!tg::intersection(plane, triangle_3).has_value());
    }
}

FUZZ_TEST("IntersectionRay3Sphere3")(tg::rng& rng)
{
    auto box1 = tg::aabb1(tg::pos1(-1.0f), tg::pos1(1.0f));
    auto box3 = tg::aabb3(tg::pos3(-1.0f), tg::pos3(1.0f));
    // random sphere
    auto s = tg::sphere_boundary<3, float>(uniform(rng, box3) * 10.0f, uniform(rng, 0.1f, 1.0f));

    {
        // ray from sphere origin to random direction
        auto randomDir3 = normalize(tg::vec3(uniform(rng, box3)));
        auto r = tg::ray3(s.center, randomDir3);

        auto result = tg::intersection(r, s);
        CHECK(result.size() == 1);
        CHECK(distance(result[0], s.center) == nx::approx(s.radius));

        // inside (or on surface of) sphere moved along its direction
        auto m = uniform(rng, box1).x * s.radius;
        auto rv = tg::vec3::zero;
        while (length(rv) == 0)
            rv = tg::vec3(uniform(rng, box3));
        r.dir = normalize(rv);
        r.origin += m * r.dir;

        result = tg::intersection(r, s);
        CHECK(result.size() == 1);
        CHECK(distance(result[0], r.origin) == nx::approx(s.radius - m));

        // close to surface and perpendicular shots
        auto eps = tg::f32(1e-2);
        auto n = r.dir;
        auto t = any_normal(n);
        auto rnd = tg::abs(uniform(rng, box1).x); // positive

        // moved slightly below surface
        {
            r.origin = s.center + s.radius * (1.0f - eps) * n;

            result = tg::intersection(r, s);
            CHECK(result.size() == 1);

            // shoot from outside
            r.origin += t * s.radius * (1.0f + eps + rnd);
            r.dir = -t;

            result = tg::intersection(r, s);
            CHECK(result.size() == 2);
        }

        // moved slightly above surface
        {
            r.dir = n;
            r.origin = s.center + s.radius * (1.0f + eps) * n;

            result = tg::intersection(r, s);
            CHECK(result.size() == 0);

            // shoot from outside
            r.origin += t * s.radius * rnd * 10.0f;
            r.dir = -t;

            result = tg::intersection(r, s);
            CHECK(result.size() == 0);
        }

        // ray outside sphere
        {
            r.origin = s.center + s.radius * (1.0f + eps + rnd) * randomDir3;
            // shooting at center (expecting two intersections)
            r.dir = normalize(s.center - r.origin);

            result = tg::intersection(r, s);
            CHECK(result.size() == 2);

            // shooting away from center (expecting no intersection)
            r.dir = -r.dir;

            result = tg::intersection(r, s);
            CHECK(result.size() == 0);
        }
    }
}

TEST("IntersectionSphere3Sphere3")
{
    { // touching spheres 1 (side-by-side)
        auto ac = tg::pos3(0, 0, 0);
        auto ar = 1.0f;
        auto bc = tg::pos3(0, 0, 2);
        auto br = 1.0f;

        auto a = tg::sphere3(ac, ar);
        auto b = tg::sphere3(bc, br);

        auto res = intersection(boundary_of(a), boundary_of(b));

        // Yields an intersection (in one point)
        CHECK(res.has_value());

        CHECK(res.value().center == tg::pos3(0, 0, 1));
    }

    { // touching spheres 2 (one inside the other)
        auto ac = tg::pos3(0, 0, 0);
        auto ar = 2.0f;
        auto bc = tg::pos3(0, 0, 1);
        auto br = 1.0f;

        auto a = tg::sphere3(ac, ar);
        auto b = tg::sphere3(bc, br);

        auto res = intersection(boundary_of(a), boundary_of(b));

        // yields an intersection (in one point)
        CHECK(res.has_value());

        CHECK(res.value().center == tg::pos3(0, 0, 2));
    }

    { // non-touching spheres 1 (side-by-side)
        auto ac = tg::pos3(0, 0, 0);
        auto ar = 1.0f;
        auto bc = tg::pos3(0, 0, 3);
        auto br = 1.0f;

        auto a = tg::sphere3(ac, ar);
        auto b = tg::sphere3(bc, br);

        auto res = intersection(boundary_of(a), boundary_of(b));

        // yields no intersection
        CHECK(!res.has_value());
    }

    { // non-touching spheres 2 (one inside the other)
        auto ac = tg::pos3(0, 0, 0);
        auto ar = 2.0f;
        auto bc = tg::pos3(0, 0, 0);
        auto br = 1.0f;

        auto a = tg::sphere3(ac, ar);
        auto b = tg::sphere3(bc, br);

        auto res = intersection(boundary_of(a), boundary_of(b));

        // yields no intersection
        CHECK(!res.has_value());
    }

    { // intersecting spheres
        auto ac = tg::pos3(0, 0, 0);
        auto ar = 1.0f;
        auto bc = tg::pos3(0, 0, 1);
        auto br = 1.0f;

        auto a = tg::sphere3(ac, ar);
        auto b = tg::sphere3(bc, br);

        auto res = intersection(boundary_of(a), boundary_of(b));

        // yields an intersection circle
        CHECK(res.has_value());

        // radius is sqrt(1^2 - 0.5^2)
        CHECK(res.value().radius == nx::approx(tg::sqrt(0.75f)));

        // intersection circle center exactly between sphere centers
        CHECK(res.value().center == tg::pos3(0, 0, 0.5));

        // intersection normal points from sphere a to sphere b
        CHECK(res.value().normal == tg::vec3(0, 0, 1));
    }

    { // intersecting spheres (swapped spheres)
        auto ac = tg::pos3(0, 0, 0);
        auto ar = 1.0f;
        auto bc = tg::pos3(0, 0, 1);
        auto br = 1.0f;

        auto a = tg::sphere3(ac, ar);
        auto b = tg::sphere3(bc, br);

        auto res = intersection(boundary_of(b), boundary_of(a));

        // yields an intersection circle
        CHECK(res.has_value());

        // radius is sqrt(1^2 - 0.5^2)
        CHECK(res.value().radius == nx::approx(tg::sqrt(0.75f)));

        // intersection circle center exactly between sphere centers
        CHECK(res.value().center == tg::pos3(0, 0, 0.5));

        // intersection normal points from sphere a to sphere b (swapped here)
        CHECK(res.value().normal == tg::vec3(0, 0, -1));
    }
}

TEST("IntersectionSegment3Triangle3")
{
    // t in xz-plane
    tg::triangle3 t = {{-1.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, -1.0f}};

    // 1st case: intersection of segment through origin
    tg::segment3 s = {{-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f}};

    auto insec = tg::intersection(s, t);
    CHECK(insec.has_value());
}

TEST("IntersectionCircle2Circle2")
{
    { // touching circles 1 (side-by-side)
        auto ac = tg::pos2(0, 0);
        auto ar = 1.0f;
        auto bc = tg::pos2(0, 2);
        auto br = 1.0f;

        auto a = tg::sphere2(ac, ar);
        auto b = tg::sphere2(bc, br);

        auto res = intersection(boundary_of(a), boundary_of(b));

        // Yields an intersection (in one point)
        CHECK(res.has_value());

        CHECK(res.value().first == tg::pos2(0, 1));
        CHECK(res.value().first == res.value().second);
    }

    { // touching sphere2 2 (one inside the other)
        auto ac = tg::pos2(0, 0);
        auto ar = 2.0f;
        auto bc = tg::pos2(0, 1);
        auto br = 1.0f;

        auto a = tg::sphere2(ac, ar);
        auto b = tg::sphere2(bc, br);

        auto res = intersection(boundary_of(a), boundary_of(b));

        // yields an intersection (in one point)
        CHECK(res.has_value());

        CHECK(res.value().first == tg::pos2(0, 2));
        CHECK(res.value().first == res.value().second);
    }

    { // non-touching circles 1 (side-by-side)
        auto ac = tg::pos2(0, 0);
        auto ar = 1.0f;
        auto bc = tg::pos2(0, 3);
        auto br = 1.0f;

        auto a = tg::sphere2(ac, ar);
        auto b = tg::sphere2(bc, br);

        auto res = intersection(boundary_of(a), boundary_of(b));

        // yields no intersection
        CHECK(!res.has_value());
    }

    { // non-touching circles 2 (one inside the other)
        auto ac = tg::pos2(0, 0);
        auto ar = 2.0f;
        auto bc = tg::pos2(0, 0);
        auto br = 1.0f;

        auto a = tg::sphere2(ac, ar);
        auto b = tg::sphere2(bc, br);

        auto res = intersection(boundary_of(a), boundary_of(b));

        // yields no intersection
        CHECK(!res.has_value());
    }

    { // intersecting circles
        auto ac = tg::pos2(0, 0);
        auto ar = 1.0f;
        auto bc = tg::pos2(1, 0);
        auto br = 1.0f;

        auto a = tg::sphere2(ac, ar);
        auto b = tg::sphere2(bc, br);

        auto res = intersection(boundary_of(a), boundary_of(b));

        // yields an intersection circle
        CHECK(res.has_value());

        // intersection positions
        CHECK(res.value().first.x == nx::approx(0.5f));
        CHECK(res.value().first.y == nx::approx(tg::sqrt(0.75f)));
        CHECK(res.value().second.x == nx::approx(0.5f));
        CHECK(res.value().second.y == nx::approx(-tg::sqrt(0.75f)));
    }

    { // intersecting circles (swapped circles)
        auto ac = tg::pos2(0, 0);
        auto ar = 1.0f;
        auto bc = tg::pos2(1, 0);
        auto br = 1.0f;

        auto a = tg::sphere2(ac, ar);
        auto b = tg::sphere2(bc, br);

        auto res = intersection(boundary_of(b), boundary_of(a));

        // yields an intersection circle
        CHECK(res.has_value());

        // intersection positions
        CHECK(res.value().first.x == nx::approx(0.5f));
        CHECK(res.value().first.y == nx::approx(-tg::sqrt(0.75f)));
        CHECK(res.value().second.x == nx::approx(0.5f));
        CHECK(res.value().second.y == nx::approx(tg::sqrt(0.75f)));
    }
}

FUZZ_TEST("Triangle - Intersection")(tg::rng& rng)
{
    auto bb = tg::aabb3(-10, 10);

    auto v0 = uniform(rng, bb);
    auto v1 = uniform(rng, bb);
    auto v2 = uniform(rng, bb);
    auto origin = uniform(rng, bb);

    auto t0 = tg::triangle(v0, v1, v2);
    auto t1 = tg::triangle(v1, v2, v0);
    auto t2 = tg::triangle(v2, v0, v1);
    auto t3 = tg::triangle(v2, v1, v0);
    auto t4 = tg::triangle(v1, v0, v2);
    auto t5 = tg::triangle(v0, v2, v1);

    auto p = uniform(rng, t0);
    auto rdir = normalize(p - origin);
    auto ray = tg::ray(origin, rdir);
    auto nray = tg::ray(origin, -rdir);

    CHECK(tg::intersects(ray, t0));
    CHECK(tg::intersects(ray, t1));
    CHECK(tg::intersects(ray, t2));
    CHECK(tg::intersects(ray, t3));
    CHECK(tg::intersects(ray, t4));
    CHECK(tg::intersects(ray, t5));
    CHECK(tg::intersects(ray, tg::plane3(rdir, p)));
    CHECK(tg::intersects(ray, tg::plane3(-rdir, p)));

    CHECK(!tg::intersects(nray, t0));
    CHECK(!tg::intersects(nray, t1));
    CHECK(!tg::intersects(nray, t2));
    CHECK(!tg::intersects(nray, t3));
    CHECK(!tg::intersects(nray, t4));
    CHECK(!tg::intersects(nray, t5));
    CHECK(!tg::intersects(nray, tg::plane3(rdir, p)));
    CHECK(!tg::intersects(nray, tg::plane3(-rdir, p)));

    auto ip0 = tg::intersection(ray, t0);
    auto ip1 = tg::intersection(ray, t1);
    auto ip2 = tg::intersection(ray, t2);
    auto ip3 = tg::intersection(ray, t3);
    auto ip4 = tg::intersection(ray, t4);
    auto ip5 = tg::intersection(ray, t5);

    CHECK(ip0.any());
    CHECK(ip1.any());
    CHECK(ip2.any());
    CHECK(ip3.any());
    CHECK(ip4.any());
    CHECK(ip5.any());

    CHECK(ip0.first() == nx::approx(p).abs(0.3f));
    CHECK(ip1.first() == nx::approx(p).abs(0.3f));
    CHECK(ip2.first() == nx::approx(p).abs(0.3f));
    CHECK(ip3.first() == nx::approx(p).abs(0.3f));
    CHECK(ip4.first() == nx::approx(p).abs(0.3f));
    CHECK(ip5.first() == nx::approx(p).abs(0.3f));

    auto a = uniform(rng, -2.f, 2.f);
    auto b = uniform(rng, -2.f, 2.f);
    auto c = 1 - a - b;
    auto p2 = t0[{a, b, c}];
    auto rdir2 = normalize(p2 - origin);
    auto ray2 = tg::ray(origin, rdir2);
    auto nray2 = tg::ray(origin, -rdir2);

    CHECK(!tg::intersects(nray2, t0));
    if (a < 0 || b < 0 || c < 0)
        CHECK(!tg::intersects(ray2, t0));
}
