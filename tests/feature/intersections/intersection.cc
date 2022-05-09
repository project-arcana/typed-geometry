#include <nexus/ext/tg-approx.hh>
#include <nexus/fuzz_test.hh>

#include <typed-geometry/tg-std.hh>

// TEMP
#include <iostream>

TEST("IntersectsSegment2Sphere2")
{
    tg::sphere2 s = tg::sphere2({0.0f, 0.0f}, 1.0f);

    // 1st case: both seg points inside the sphere
    tg::pos2 p1_s1 = {0.2f, -0.5f};
    tg::pos2 p2_s1 = {0.4f, 0.4f};
    tg::segment2 seg1 = {p1_s1, p2_s1};

    CHECK(tg::intersects(seg1, s));

    // 2nd case: one seg point inside the sphere and one outside
    tg::pos2 p1_s2 = p1_s1;
    tg::pos2 p2_s2 = {1.0f, 1.0f};
    tg::segment2 seg2 = {p1_s2, p2_s2};

    CHECK(tg::intersects(seg2, s));

    // 3rd case: both seg points outside of the sphere but intersection exists
    tg::pos2 p1_s3 = p2_s2;
    tg::pos2 p2_s3 = {-1.0f, -1.0f};
    tg::segment2 seg3 = {p1_s3, p2_s3};

    CHECK(tg::intersects(seg3, s));
}

FUZZ_TEST("IntersectsBox2Sphere2")(tg::rng& rng)
{
    // unit sphere in two dimensions
    tg::sphere2 s = tg::sphere2({0, 0}, 1.0f);
    tg::aabb2 inside_sphere = tg::aabb2({-0.5f, -0.5f}, {0.5f, 0.5f});

    // 1st case: box center inside the sphere
    auto c_b1 = tg::uniform(rng, inside_sphere);
    auto b1 = tg::box2::unit_centered;
    b1.center = c_b1;

    std::cout << b1.center << std::endl;
    std::cout << b1.half_extents[0] << std::endl;
    std::cout << b1.half_extents[1] << std::endl;

    CHECK(tg::intersects(b1, s));

    //// 2nd case: box: vertice inside the sphere
    // auto v = tg::uniform<tg::dir2>(rng);
    // auto b2 = tg::box2::unit_centered;
    // auto c_b2 = tg::uniform(rng, inside_sphere);
    // c_b2 += tg::sqrt(0.5f) * tg::normalize(c_b2 - tg::pos2::zero);
    // b2.center = c_b2;

    // CHECK(tg::intersects(b2, s));
}


FUZZ_TEST("IntersectsBox3Triangle3")(tg::rng& rng)
{
    tg::aabb3 env = {{-5.0f, -5.0f, -5.0f}, {5.0f, 5.0f, 5.0f}};
    tg::aabb1 scalar_range = tg::aabb1(-5.0f, 5.0f);
    tg::aabb1 pos_scalar_range = tg::aabb1(0.0f, 5.0f);

    // center = (0,0,0), half_extents = {(0.5,0,0),(0,0.5,0),(0,0,0.5)}
    tg::box3 b = tg::box3::unit_centered;

    // 1st case: triangle insie of the box
    auto pos1_t1 = tg::uniform(rng, b);
    auto pos2_t1 = tg::uniform(rng, b);
    auto pos3_t1 = tg::uniform(rng, b);
    auto t1 = tg::triangle3(pos1_t1, pos2_t1, pos3_t1);

    CHECK(tg::intersects(b, t1));

    // 2nd case: at least one point inside the box
    auto pos2_t2 = tg::uniform(rng, env);
    auto pos3_t2 = tg::uniform(rng, env);
    auto t2 = tg::triangle3(pos1_t1, pos2_t2, pos3_t2);

    CHECK(tg::intersects(b, t2));

    // 3rd case: box entirely above plane spanned by t1
    tg::plane3 p = tg::plane_of(t1);
    auto b2 = tg::box3::unit_centered;

    if (p.dis >= 0)
        b2.center = b2.center + (p.dis + tg::uniform(rng, pos_scalar_range).x + 1.0f + 10.0f * tg::epsilon<float>)*p.normal;

    else
        b2.center += (p.dis - tg::uniform(rng, pos_scalar_range).x - 1.0f - 10.0f * tg::epsilon<float>)*p.normal;

    CHECK(!tg::intersects(b2, t1));
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

    // 1st case: box entirely above plane
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

    // case 2: intersection exists
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

TEST("IntersectionSegment3Triangle3")
{
    // t in xy-plane
    tg::triangle3 t = {{-1.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, -1.0f}};

    tg::segment3 s = {{-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f}};

    CHECK(tg::intersection(s, t).has_value());
}

FUZZ_TEST("IntersectsSphere3Plane3")(tg::rng& rng)
{
    tg::plane3 p = tg::plane3(tg::dir3(0.0f, 1.0f, 0.0f), {0.0f, 0.0f, 0.0f});
    tg::aabb3 above_plane = {{-15.0f, 0.1f, -15.0f}, {15.0f, 0.1f, 15.0f}};
    tg::aabb1 range_radius = tg::aabb1{0.1f, 10.0f};

    auto sphere_center = tg::uniform(rng, above_plane);
    auto radius = tg::uniform(rng, range_radius).x;
    tg::sphere3 s = tg::sphere3(sphere_center, radius);

    bool insec = sphere_center.y <= radius ? true : false;
    CHECK(tg::intersects(p, s) == insec);

    // TODO: Handle one more case maybe
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

    //{ // TEMP
    //    auto v = glow::viewer::view();
    //    gv::view(b2, tg::color3::cyan);
    //    gv::view(s1, tg::color3::magenta);
    //}

    //{ // TEMP
    //    auto v = glow::viewer::view();
    //    gv::view(b3, tg::color3::cyan);
    //    gv::view(s1, tg::color3::magenta);
    //}
}

TEST("IntersectsSphere3Sphere3")
{
    tg::sphere3 s1 = tg::sphere3(tg::pos3(0, 0, 0), 5.0f);
    tg::sphere3 s2 = tg::sphere3(tg::pos3(6.0, 0, 0), 1.0f);

    // s1 and s2 should intersect (in exactly one point)
    CHECK(tg::intersects(s1, s2));

    tg::sphere3 s3 = tg::sphere3(tg::pos3(0, 0, 0), 2.0f);

    // s3 is inside of s1. Thus, s3 and s1 should intersect
    CHECK(tg::intersects(s1, s3));

    tg::sphere3 s4 = tg::sphere3(tg::pos3(6.0f + 10 * tg::epsilon<float>, 0, 0), 1.0f);

    // s1 and s4 should not intersect
    CHECK(!tg::intersects(s1, s4));
}

FUZZ_TEST("IntersectionSegment3Box3")(tg::rng& rng)
{
    // 1st case: intersection exists, segment vertices outside of box
    tg::aabb3 bb = {{-2.0f, -2.0f, -2.0f}, {2.0f, 2.0f, 2.0}}; // enlarged unit box
    tg::box3 box = tg::box3(bb);

    tg::aabb3 above_box = {{-2.0f, 2.5f, -2.0f}, {2.0f, 5.0f, 2.0f}};
    tg::aabb3 below_box = {{-2.0f, -5.0f, -2.0f}, {2.0f, -2.5f, 2.0f}};

    auto p1 = tg::uniform(rng, above_box);
    auto p2 = tg::uniform(rng, below_box);
    tg::segment3 s1 = {p1, p2};


    //{ // TEMP
    //    auto insec = tg::intersection(s1, box);

    //    auto v = glow::viewer::view();
    //    gv::view(box, tg::color3::cyan);
    //    gv::view(s1, tg::color3::magenta);
    //    gv::view(insec.value(), tg::color3::red);
    //}

    CHECK(tg::intersection(s1, box).has_value());

    // 2nd case: One point of segment inside the box and one point outside of the box
    auto p3 = tg::uniform(rng, bb);
    auto p4 = tg::uniform(rng, below_box);
    tg::segment3 s2 = {p3, p4};

    auto insec2 = tg::intersection(s2, box);

    //{ // TEMP
    //    auto v = glow::viewer::view();
    //    gv::view(box, tg::color3::cyan);
    //    gv::view(s2, tg::color3::magenta);
    //    // gv::view(insec2.value(), tg::color3::red);
    //}

    CHECK(insec2.has_value());
    CHECK(insec2.value().pos0 == s2.pos0);

    // 3rd case: Both points inside of box
    auto p5 = tg::uniform(rng, bb);
    tg::segment3 s3 = {p3, p5};

    auto insec3 = tg::intersection(s3, box);

    CHECK(insec3.has_value());
    CHECK(insec3.value().pos0 == s3.pos0);
    CHECK(insec3.value().pos1 == s3.pos1);
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

    // auto insec = tg::intersection(s1, t1);

    //{ // TEMP
    //    auto v = glow::viewer::view();
    //    gv::view(t1, tg::color3::cyan);
    //    gv::view(s1, tg::color3::magenta);
    //    gv::view(insec.value(), tg::color3::red);
    //}

    CHECK(tg::intersection(s1, t1).has_value());
}

FUZZ_TEST("IntersectionTriangle3Triangle3")(tg::rng& rng)
{
    // Triangle3 - Triangle3
    tg::triangle3 t1 = {tg::pos3(-1.0f, 0.0f, 2.0f), tg::pos3(2.0f, 0.0f, -1.0f), tg::pos3(-1.0f, 0.0f, -1.0f)};

    // 1st case: intersection with only one segment of each triangle
    tg::triangle3 t2 = {tg::pos3(0.0f, -1.0f, 0.0f), tg::pos3(0.0f, 1.0f, 0.0f), tg::pos3(-5.0f, -1.0f, 0.0f)};

    auto insec = tg::intersection(t1, t2);

    CHECK(insec.has_value()); // How to further check if the segment is correct?

    // 2nd Case: No intersection
    tg::aabb3 above_plane = tg::aabb3({-15.0f, 0.0f, -15.0f}, {15.0f, 15.0f, 15.0f}); // above plane spaned by t1
    // tg::aabb3 below_plane = tg::aabb3({-15.0f, -15.0f, -15.0f}, {15.0f, 0.0f, 15.0f}); // below plane spaned by t1

    auto pos1 = tg::uniform(rng, above_plane);
    auto pos2 = tg::uniform(rng, above_plane);
    auto pos3 = tg::uniform(rng, above_plane);
    tg::triangle3 t3 = {pos1, pos2, pos3};

    CHECK(!tg::intersection(t1, t3).has_value());

    // 3rd Case: Intersection in one point
    tg::aabb3 tbb = {{-10.0f, 0.0f, -10.0f}, {10.0f, 0.0f, 10.0f}};
    // t4 in xy-plane
    tg::triangle3 t4 = {tg::uniform(rng, tbb), tg::uniform(rng, tbb), tg::uniform(rng, tbb)};
    auto n_t4 = tg::normalize(tg::cross(t4.pos1 - t4.pos0, t4.pos2 - t4.pos0));

    auto pos4 = t4.pos0 + tg::vec3(0, 2.0f, 0);
    auto pos5 = t4.pos0 - tg::vec3(0, 2.0f, 0);
    auto pos6 = t4.pos0 + tg::cross(t4.pos1 - t4.pos0, n_t4);

    tg::triangle3 t5 = tg::triangle(pos4, pos5, pos6);

    CHECK(tg::intersection(t4, t5).has_value());
}

FUZZ_TEST("IntersectsBox3Box3")(tg::rng& rng)
{
    // Box3 - Box3

    // rng aabbs
    auto short_scalar_range = tg::aabb1(0.0f, 5.0f);
    auto scalar_range = tg::aabb1(-9.0f, 9.0f);
    auto range3D = tg::aabb3(tg::pos3(-15.0f), tg::pos3(15.0f));
    auto range_rotation = tg::aabb1(0.0f, 360.0f);

    // 1st test case: Not intersecting boxes should not intersect when same random translation is applied to both boxes

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

    //{ // TEMP
    //    auto v = glow::viewer::view();
    //    gv::view(box1, tg::color3::cyan);
    //    gv::view(box2, tg::color3::magenta);
    //}

    CHECK(!tg::intersects(box1, box2));


    // 2nd test case: invariant to rotation when circumspheres of boxes do not intersect

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

    //{ // TEMP
    //    auto v = glow::viewer::view();
    //    gv::view(box3, tg::color3::cyan);
    //    gv::view(box4, tg::color3::magenta);
    //}

    CHECK(!tg::intersects(box3, box4));


    // 3rd case: rotation + translation (rigid transformation)

    auto box5_he = tg::mat3();
    auto box6_he = tg::mat3();

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

    //{ // TEMP
    //    auto v = glow::viewer::view();
    //    gv::view(box5, tg::color3::cyan);
    //    gv::view(box6, tg::color3::magenta);
    //}

    bool insec = tg::intersects(box5, box6);

    tg::vec3 translation_vector = tg::vec3(tg::uniform(rng, range3D));
    // auto rotation_angle = tg::degree(tg::uniform(rng, range_rotation).x);
    auto rotation_axis = tg::uniform<tg::dir3>(rng);
    auto rotation = tg::rotation_around(rotation_axis, tg::degree(tg::uniform(rng, range_rotation).x));

    box5.center += translation_vector;
    box6.center += translation_vector;

    for (auto v = 0; v < 3; v++)
    {
        box5.half_extents[v] = tg::vec3(rotation * tg::vec4(box3.half_extents[v], 1.0f));
        box6.half_extents[v] = tg::vec3(rotation * tg::vec4(box4.half_extents[v], 1.0f));
    }

    //{ // TEMP
    //    auto v = glow::viewer::view();
    //    gv::view(box5, tg::color3::cyan);
    //    gv::view(box6, tg::color3::magenta);
    //    std::cout << insec << std::endl;
    //}

    CHECK(tg::intersects(box5, box6) == insec);
}

FUZZ_TEST("IntersectsBox2Box2")(tg::rng& rng)
{
    // Box2 - Box2
    auto short_scalar_range = tg::aabb1(0.0f, 5.0f);
    auto range2D = tg::aabb2(tg::pos2(-10.0f), tg::pos2(10.0f));
    auto range_rotation = tg::aabb1(90.0f, 360.0f);

    auto box1_half_extents = tg::mat2();
    auto box2_half_extents = tg::mat2();

    box1_half_extents[0] = {tg::uniform(rng, short_scalar_range).x, 0.0f};
    box1_half_extents[1] = {0.0f, tg::uniform(rng, short_scalar_range).x};
    auto rad_circumsphere1 = tg::max(tg::length(box1_half_extents[0]), tg::length(box1_half_extents[1]));

    box2_half_extents[0] = {tg::uniform(rng, short_scalar_range).x, 0.0f};
    box2_half_extents[1] = {0.0f, tg::uniform(rng, short_scalar_range).x};
    auto rad_circumsphere2 = tg::max(tg::length(box2_half_extents[0]), tg::length(box2_half_extents[1]));

    auto offset = (rad_circumsphere1 + rad_circumsphere2 + 0.001f) * tg::uniform<tg::dir2>(rng);

    auto box1 = tg::box2(tg::uniform(rng, range2D), box1_half_extents);
    auto box2 = tg::box2(box1.center + offset, box2_half_extents);

    auto insec = tg::intersects(box1, box2);

    //{ // TEMP
    //    tg::pos3 origin = {0, 0, 0};
    //    tg::quad3 a = {tg::pos3(box1.center - box1_half_extents[0] - box1_half_extents[1], 0.0f),
    //                   tg::pos3(box1.center - box1_half_extents[0] + box1_half_extents[1], 0.0f),
    //                   tg::pos3(box1.center + box1_half_extents[0] + box1_half_extents[1], 0.0f),
    //                   tg::pos3(box1.center + box1_half_extents[0] - box1_half_extents[1], 0.0f)};
    //    tg::quad3 b = {tg::pos3(box2.center - box2_half_extents[0] - box2_half_extents[1], 0.0f),
    //                   tg::pos3(box2.center - box2_half_extents[0] + box2_half_extents[1], 0.0f),
    //                   tg::pos3(box2.center + box2_half_extents[0] + box2_half_extents[1], 0.0f),
    //                   tg::pos3(box2.center + box2_half_extents[0] - box2_half_extents[1], 0.0f)};
    //    auto v = glow::viewer::view();
    //    gv::view(origin, tg::color3::red);
    //    gv::view(a, tg::color3::cyan);
    //    gv::view(b, tg::color3::magenta);
    //}

    // rigid transformation
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

    //{ // TEMP
    //    tg::pos3 origin = {0, 0, 0};
    //    tg::quad3 a = {tg::pos3(box1.center - box1.half_extents[0] - box1.half_extents[1], 0.0f),
    //                   tg::pos3(box1.center - box1.half_extents[0] + box1.half_extents[1], 0.0f),
    //                   tg::pos3(box1.center + box1.half_extents[0] + box1.half_extents[1], 0.0f),
    //                   tg::pos3(box1.center + box1.half_extents[0] - box1.half_extents[1], 0.0f)};
    //    tg::quad3 b = {tg::pos3(box2.center - box2.half_extents[0] - box2.half_extents[1], 0.0f),
    //                   tg::pos3(box2.center - box2.half_extents[0] + box2.half_extents[1], 0.0f),
    //                   tg::pos3(box2.center + box2.half_extents[0] + box2.half_extents[1], 0.0f),
    //                   tg::pos3(box2.center + box2.half_extents[0] - box2.half_extents[1], 0.0f)};
    //    auto v = glow::viewer::view();
    //    gv::view(origin, tg::color3::red);
    //    gv::view(a, tg::color3::cyan);
    //    gv::view(b, tg::color3::magenta);
    //}

    CHECK(intersects(box1, box2) == insec);
}

FUZZ_TEST("IntersectionSegment3ConvexShapes3")(tg::rng& rng)
{
    // segment3 - capsule3
    // -> handle the underlying cases:
    // a) seg: both points outside, but intersection exists
    // b) seg: both points outside, but no intersection exists
    // c) seg: both points inside
    // d) seg: one point inside, one point outside
    tg::pos3 origin = tg::pos3(0.0f);
    auto capsule = tg::capsule3({0, -1, 0}, {0, 1, 0}, 1.0);

    {
        // a) intersection exists if segment passes (for ex.) through origin.
        auto dir = tg::uniform<tg::dir3>(rng);
        // extent by factor -1 in other direction to get pos1 of the segment that passes through the origin with a random orientation
        auto pos0 = origin + tg::vec3(5 * dir);
        auto pos1 = origin - tg::vec3(5 * dir);
        tg::segment3 rng_segment = {pos0, pos1};

        CHECK(tg::intersection(rng_segment, capsule).has_value());
    }
    {
        // b)
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
    {
        // c)
        auto pos0_2 = tg::uniform(rng, capsule);
        auto pos1_2 = tg::uniform(rng, capsule);
        tg::segment3 rng_segment_inside = {pos0_2, pos1_2};

        CHECK(tg::intersection(rng_segment_inside, capsule).has_value());
    }
    {
        // d)
        auto pos0 = tg::uniform(rng, capsule);

        // point outside of the capsule
        auto pos1 = tg::uniform(rng, capsule) + 2.0 * tg::uniform<tg::dir3>(rng);
        tg::segment3 rng_segment = {pos0, pos1};

        CHECK(tg::intersection(rng_segment, capsule).has_value());
    }
}

FUZZ_TEST("IntersectionPlane3Triangle3")(tg::rng& rng)
{ // plane is the xy-plane
    tg::plane3 plane = tg::plane3(tg::dir3(0, 1, 0), tg::pos3(0, 0, 0));

    // random triangle with intersection
    tg::aabb3 above_plane = tg::aabb3({-15.0f, 0.0f, -15.0f}, {15.0f, 15.0f, 15.0f});
    tg::aabb3 below_plane = tg::aabb3({-15.0f, -15.0f, -15.0f}, {15.0f, 0.0f, 15.0f});

    // 1st test case: one point above plane and two points below the plane
    auto pos0 = tg::uniform(rng, above_plane);
    auto pos1 = tg::uniform(rng, below_plane);
    auto pos2 = tg::uniform(rng, below_plane);

    tg::triangle3 triangle_1 = tg::triangle3(pos0, pos1, pos2);

    CHECK(tg::intersection(plane, triangle_1).has_value());

    // 2nd test case: all points below plane
    pos0 = tg::uniform(rng, below_plane);
    pos1 = tg::uniform(rng, below_plane);
    pos2 = tg::uniform(rng, below_plane);

    tg::triangle3 triangle_2 = tg::triangle(pos0, pos1, pos2);

    CHECK(!tg::intersection(plane, triangle_2).has_value());

    // 3rd test case: all points above plane
    pos0 = tg::uniform(rng, above_plane);
    pos1 = tg::uniform(rng, above_plane);
    pos2 = tg::uniform(rng, above_plane);

    tg::triangle3 triangle_3 = tg::triangle3(pos0, pos1, pos2);

    CHECK(!tg::intersection(plane, triangle_3).has_value());
}

FUZZ_TEST("IntersectionRay3Sphere3")(tg::rng& rng)
{
    auto box1 = tg::aabb1(tg::pos1(-1.0f), tg::pos1(1.0f));
    auto box3 = tg::aabb3(tg::pos3(-1.0f), tg::pos3(1.0f));
    // random sphere
    auto s = tg::sphere_boundary<3, float>(uniform(rng, box3) * 10.0f, tg::abs(uniform(rng, box1).x));

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

    CHECK(ip0.first() == nx::approx(p).abs(0.1f));
    CHECK(ip1.first() == nx::approx(p).abs(0.1f));
    CHECK(ip2.first() == nx::approx(p).abs(0.1f));
    CHECK(ip3.first() == nx::approx(p).abs(0.1f));
    CHECK(ip4.first() == nx::approx(p).abs(0.1f));
    CHECK(ip5.first() == nx::approx(p).abs(0.1f));

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
