#include <nexus/fuzz_test.hh>
#include <nexus/ext/tg-approx.hh>

#include <typed-geometry/tg-std.hh>

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
    auto p2 = t0[{a,b,c}];
    auto rdir2 = normalize(p2 - origin);
    auto ray2 = tg::ray(origin, rdir2);
    auto nray2 = tg::ray(origin, -rdir2);

    CHECK(!tg::intersects(nray2, t0));
    if (a < 0 || b < 0 || c < 0)
        CHECK(!tg::intersects(ray2, t0));
}
