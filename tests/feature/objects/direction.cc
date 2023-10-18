#include <nexus/ext/tg-approx.hh>
#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/objects.hh>
#include <typed-geometry/feature/vector.hh>

// FIXME: flaky right now
FUZZ_TEST("Direction", disabled)(tg::rng& rng)
{
    // TODO direction (i.e. closest_points) for more objects? currently just pos, line, segment, planes..
    auto rBox1 = tg::aabb1(tg::pos1(-1.0f), tg::pos1(1.0f));
    auto rBox2 = tg::aabb2(tg::pos2(-1.0f, -1.0f), tg::pos2(1.0f, 1.0f));
    auto rBox3 = tg::aabb3(tg::pos3(-1.0f, -1.0f, -1.0f), tg::pos3(1.0f, 1.0f, 1.0f));

    // pos3
    {
        // random point in space, move away in random direction
        auto dir = normalize(tg::vec3(uniform(rng, rBox3)));
        auto p = tg::pos3(uniform(rng, rBox3));
        auto np = p + dir;

        // compare calculated with given
        auto cDir = tg::dir3(direction(p, np));
        CHECK(cDir == nx::approx(dir));
    }
    // pos2
    {
        // random point in space, move away in random direction
        auto dir = normalize(tg::vec2(uniform(rng, rBox2)));
        auto p = tg::pos2(uniform(rng, rBox2));
        auto np = p + dir;

        // compare calculated with given
        auto cDir = tg::dir2(direction(p, np));
        CHECK(cDir == nx::approx(dir));
    }
    // pos1
    {
        // random point in space, move away in random direction
        auto dir = normalize(tg::vec1(uniform(rng, rBox1)));
        auto p = tg::pos1(uniform(rng, rBox2));
        auto np = p + dir;

        // compare calculated with given
        auto cDir = tg::dir1(direction(p, np));
        CHECK(cDir == nx::approx(dir));
    }

    // line3
    {
        // random line
        auto lpos = uniform(rng, rBox3);
        auto ldir = normalize(tg::vec3(uniform(rng, rBox3)));
        auto l = tg::line3(lpos, ldir);
        // random point on line
        auto p = l.pos + ldir * uniform(rng, rBox1).x;

        // new point, random direction from p
        auto scatter = normalize(tg::vec3(uniform(rng, rBox3)));
        auto np = p + scatter;
        // compare calculated with given
        auto cdir = tg::dir3(direction(p, np));
        CHECK(cdir == nx::approx(scatter));

        // find perpendicular direction
        auto pv = ldir;
        while (dot(pv, ldir) != 0.0f) // in case rv was parallel to ldir
        {
            auto rv = normalize(tg::vec3(uniform(rng, rBox3)));
            pv = normalize(cross(ldir, rv));
        }

        auto r = uniform(rng, rBox1).x * 10.0f; // might be negative
        auto push = pv * r;
        np = p + push; // move along perpendicular direction
        // compare calculated with given
        cdir = tg::dir3(direction(l, np));
        CHECK(cdir == nx::approx(normalize(push)).abs(0.1f));
    }
    // line2
    {
        // random line
        auto lpos = uniform(rng, rBox2);
        auto ldir = normalize(tg::vec2(uniform(rng, rBox2)));
        auto l = tg::line2(lpos, ldir);
        // random point on line
        auto p = l.pos + ldir * uniform(rng, rBox1).x;

        // new point, random direction from p
        auto scatter = normalize(tg::vec2(uniform(rng, rBox2)));
        auto np = p + scatter;
        // compare calculated with given
        auto cdir = tg::dir2(direction(p, np));
        CHECK(cdir == nx::approx(scatter));

        // perpendicular direction
        auto pv = tg::dir2(-ldir.y, ldir.x);
        CHECK(dot(pv, ldir) == nx::approx(0.0f).abs(0.001f)); // just to be sure

        auto r = uniform(rng, rBox1).x * 10.0f; // might be negative
        auto push = pv * r;
        np = p + push; // move along perpendicular direction
        // compare calculated with given
        cdir = tg::dir2(direction(l, np));
        CHECK(cdir == nx::approx(normalize(push)).abs(0.1f));
    }
    // segment3
    {
        // random segment
        auto length = tg::abs(uniform(rng, rBox1).x) * 10.0f;
        auto lpos = uniform(rng, rBox3);
        auto ldir = normalize(tg::vec3(uniform(rng, rBox3)));
        auto l = tg::segment3(lpos, lpos + ldir * length);
        // random point ON segment
        auto p = l.pos0 + ldir * tg::abs(uniform(rng, rBox1).x) * length;

        // new point, random direction from p
        auto scatter = normalize(tg::vec3(uniform(rng, rBox3)));
        auto np = p + scatter;
        // compare calculated with given
        auto cdir = tg::dir3(direction(p, np));
        CHECK(cdir == nx::approx(scatter));

        // find perpendicular direction
        auto pv = ldir;
        while (dot(pv, ldir) != 0.0f) // in case rv was parallel to ldir
        {
            auto rv = normalize(tg::vec3(uniform(rng, rBox3)));
            pv = normalize(cross(ldir, rv));
        }

        auto r = uniform(rng, rBox1).x; // might be negative
        auto push = pv * r;
        np = p + push; // move along perpendicular direction
        // compare calculated with given
        cdir = tg::dir3(direction(l, np));
        CHECK(cdir == nx::approx(normalize(push)).abs(0.1f));
    }
    // segment2
    {
        // random segment
        auto length = tg::abs(uniform(rng, rBox1).x);
        auto lpos = uniform(rng, rBox2);
        auto ldir = normalize(tg::vec2(uniform(rng, rBox2)));
        auto l = tg::segment2(lpos, lpos + ldir * length);
        // random point on segment
        auto p = l.pos0 + ldir * tg::abs(uniform(rng, rBox1).x) * length;

        // new point, random direction from p
        auto scatter = normalize(tg::vec2(uniform(rng, rBox2)));
        auto np = p + scatter;
        // compare calculated with given
        auto cdir = tg::dir2(direction(p, np));
        CHECK(cdir == nx::approx(scatter).abs(0.01f));

        // perpendicular direction
        auto pv = tg::dir2(-ldir.y, ldir.x);
        CHECK(dot(pv, ldir) == nx::approx(0.0f).abs(0.001f)); // just to be sure

        auto r = uniform(rng, rBox1).x * 10.0f; // might be negative
        auto push = pv * r;
        np = p + push; // move along perpendicular direction
        // compare calculated with given
        cdir = tg::dir2(direction(l, np));
        CHECK(cdir == nx::approx(normalize(push)).abs(0.01f));
    }
    // plane
    {
        // point on plane moved along plane's normal
        auto rdir = normalize(tg::vec3(uniform(rng, rBox3)));
        auto plane = tg::plane(rdir, 1.0f);
        auto planePos = tg::pos3(plane.dis * plane.normal);
        // compare calculated with given
        auto p = planePos + rdir;
        auto cdir = tg::dir3(direction(plane, p));
        CHECK(cdir == nx::approx(rdir).abs(0.01f));

        // find perpendicular direction
        auto pv = rdir;
        while (dot(pv, rdir) != 0.0f) // in case rv was parallel to ldir
        {
            auto rv = normalize(tg::vec3(uniform(rng, rBox3)));
            pv = normalize(cross(rdir, rv));
        }

        // move point orthogonally
        p += pv * uniform(rng, rBox1).x * 10.0f;
        // this should not change the calculated direction to plane
        cdir = tg::dir3(direction(plane, p));
        CHECK(cdir == nx::approx(rdir).abs(0.01f));
    }
}
