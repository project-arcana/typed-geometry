#include <nexus/test.hh>

#include <typed-geometry/feature/vector.hh>
#include <typed-geometry/feature/random.hh>

TEST("OperatorsVec")
{
    // float
    {
        // vec @ vec
        CHECK(tg::vec2(1, 2) + tg::vec2(2, 3) == tg::vec2(3, 5));
        CHECK(tg::vec4(1, 2, 3, 4) - tg::vec4(0.5f) == tg::vec4(0.5f, 1.5f, 2.5f, 3.5f));

        // vec @ size
        CHECK(tg::vec2(0.5f, 3) * tg::size2(4, 0.5f) == tg::vec2(2, 1.5f));
        CHECK(tg::vec2(0.5f, 3) / tg::size2(0.5f, 6) == tg::vec2(1, 0.5f));

        // @ vec
        CHECK(+tg::vec1(0.5f) == tg::vec1(0.5f));
        CHECK(-tg::vec1(1.5f) == tg::vec1(-1.5f));

        // vec @ scalar
        CHECK(tg::vec3(3, 6, 9) / 3.0f == tg::vec3(1, 2, 3));
        CHECK(tg::vec3(1, 2, 3) + 1.0f == tg::vec3(2, 3, 4));
        CHECK(tg::vec3(0.5f, 2.5f, 3.5f) - 2.0f == tg::vec3(-1.5f, 0.5f, 1.5f));
        CHECK(tg::vec3(0.5f, 2.5f, 3.5f) * 2.0f == tg::vec3(1, 5, 7));
    }

    // int
    {
        // vec @ vec
        CHECK(tg::ivec2(1, 2) + tg::ivec2(2, 3) == tg::ivec2(3, 5));
        CHECK(tg::ivec4(1, 2, 3, 4) - tg::ivec4(3) == tg::ivec4(-2, -1, 0, 1));

        // vec @ size
        CHECK(tg::ivec2(2, 3) * tg::isize2(4, 2) == tg::ivec2(8, 6));
        CHECK(tg::ivec2(2, 3) / tg::isize2(3, 3) == tg::ivec2(0, 1));

        // @ vec
        CHECK(+tg::ivec4(2) == tg::ivec4(2));
        CHECK(-tg::ivec4(3, 5, -6, 8) == tg::ivec4(-3, -5, 6, -8));

        // vec @ scalar
        CHECK(tg::ivec3(3, 6, 9) / 3 == tg::ivec3(1, 2, 3));
        CHECK(tg::ivec3(1, 2, 3) + 1 == tg::ivec3(2, 3, 4));
        CHECK(tg::ivec3(1, 2, 3) - 2 == tg::ivec3(-1, 0, 1));
        CHECK(tg::ivec3(3, 4, 5) * 2 == tg::ivec3(6, 8, 10));
    }

    // mixed
    {
        // vec @ scalar
        CHECK(tg::vec3(3, 6, 9) / 3 == tg::vec3(1, 2, 3));
        CHECK(tg::vec3(1, 2, 3) + 1 == tg::vec3(2, 3, 4));
        CHECK(tg::vec3(0.5f, 2.5f, 3.5f) - 2 == tg::vec3(-1.5f, 0.5f, 1.5f));
        CHECK(tg::vec3(0.5f, 2.5f, 3.5f) * 2 == tg::vec3(1, 5, 7));

        // NOTE: these are NOT supported anymore!
        // CHECK(tg::ivec3(2, 4, 7) / 4.0f == tg::vec3(0.5f, 1, 1.75f));
        // CHECK(tg::ivec3(1, 2, 3) + 1.5f == tg::vec3(2.5f, 3.5f, 4.5f));
        // CHECK(tg::ivec3(1, 2, 3) - 2.5f == tg::vec3(-1.5f, -0.5f, 0.5f));
        // CHECK(tg::ivec3(2, 3, 4) * 2.5f == tg::vec3(5, 7.5f, 10));
    }

    // assignment
    {
        tg::vec3 v;
        CHECK(v == tg::vec3(0, 0, 0));

        v += 1;
        CHECK(v == tg::vec3(1));

        v += 1.5f;
        CHECK(v == tg::vec3(2.5f));

        v *= 2;
        CHECK(v == tg::vec3(5));

        v /= 2;
        CHECK(v == tg::vec3(2.5f));

        v -= 0.5f;
        CHECK(v == tg::vec3(2));

        v /= 0.5f;
        CHECK(v == tg::vec3(4));

        v += tg::vec3(1, 2, 3);
        CHECK(v == tg::vec3(5, 6, 7));

        v /= tg::size3(2, 3, 2);
        CHECK(v == tg::vec3(2.5f, 2, 3.5f));

        v *= tg::size3(0.5f);
        CHECK(v == tg::vec3(1.25f, 1, 1.75f));

        v -= tg::vec3(1.25f, 1, 0.75f);
        CHECK(v == tg::vec3(0, 0, 1));
    }
}

TEST("OperatorsPos")
{
    // float
    {
        // pos @ pos
        CHECK(tg::pos2(1, 2) + tg::vec2(2, 3) == tg::pos2(3, 5));
        CHECK(tg::pos4(1, 2, 3, 4) - tg::pos4(0.5f) == tg::vec4(0.5f, 1.5f, 2.5f, 3.5f));

        // pos @ size
        CHECK(tg::pos2(0.5f, 3) * tg::size2(4, 0.5f) == tg::pos2(2, 1.5f));
        CHECK(tg::pos2(0.5f, 3) / tg::size2(0.5f, 6) == tg::pos2(1, 0.5f));

        // @ pos
        CHECK(+tg::pos1(0.5f) == tg::pos1(0.5f));
        CHECK(-tg::pos1(1.5f) == tg::pos1(-1.5f));

        // pos @ scalar
        CHECK(tg::pos3(3, 6, 9) / 3.0f == tg::pos3(1, 2, 3));
        CHECK(tg::pos3(1, 2, 3) + 1.0f == tg::pos3(2, 3, 4));
        CHECK(tg::pos3(0.5f, 2.5f, 3.5f) - 2.0f == tg::pos3(-1.5f, 0.5f, 1.5f));
        CHECK(tg::pos3(0.5f, 2.5f, 3.5f) * 2.0f == tg::pos3(1, 5, 7));
    }

    // int
    {
        // pos @ pos
        CHECK(tg::ipos2(1, 2) + tg::ivec2(2, 3) == tg::ipos2(3, 5));
        CHECK(tg::ipos4(1, 2, 3, 4) - tg::ipos4(3) == tg::ivec4(-2, -1, 0, 1));

        // pos @ size
        CHECK(tg::ipos2(2, 3) * tg::isize2(4, 2) == tg::ipos2(8, 6));
        CHECK(tg::ipos2(2, 3) / tg::isize2(3, 3) == tg::ipos2(0, 1));

        // @ pos
        CHECK(+tg::ipos4(2) == tg::ipos4(2));
        CHECK(-tg::ipos4(3, 5, -6, 8) == tg::ipos4(-3, -5, 6, -8));

        // pos @ scalar
        CHECK(tg::ipos3(3, 6, 9) / 3 == tg::ipos3(1, 2, 3));
        CHECK(tg::ipos3(1, 2, 3) + 1 == tg::ipos3(2, 3, 4));
        CHECK(tg::ipos3(1, 2, 3) - 2 == tg::ipos3(-1, 0, 1));
        CHECK(tg::ipos3(3, 4, 5) * 2 == tg::ipos3(6, 8, 10));
    }

    // mixed
    {
        // pos @ scalar
        CHECK(tg::pos3(3, 6, 9) / 3 == tg::pos3(1, 2, 3));
        CHECK(tg::pos3(1, 2, 3) + 1 == tg::pos3(2, 3, 4));
        CHECK(tg::pos3(0.5f, 2.5f, 3.5f) - 2 == tg::pos3(-1.5f, 0.5f, 1.5f));
        CHECK(tg::pos3(0.5f, 2.5f, 3.5f) * 2 == tg::pos3(1, 5, 7));

        // NOTE: these are NOT supported anymore!
        // CHECK(tg::ipos3(2, 4, 7) / 4.0f == tg::pos3(0.5f, 1, 1.75f));
        // CHECK(tg::ipos3(1, 2, 3) + 1.5f == tg::pos3(2.5f, 3.5f, 4.5f));
        // CHECK(tg::ipos3(1, 2, 3) - 2.5f == tg::pos3(-1.5f, -0.5f, 0.5f));
        // CHECK(tg::ipos3(2, 3, 4) * 2.5f == tg::pos3(5, 7.5f, 10));
    }

    // assignment
    {
        tg::pos3 v;
        CHECK(v == tg::pos3(0, 0, 0));

        v += 1;
        CHECK(v == tg::pos3(1));

        v += 1.5f;
        CHECK(v == tg::pos3(2.5f));

        v *= 2;
        CHECK(v == tg::pos3(5));

        v /= 2;
        CHECK(v == tg::pos3(2.5f));

        v -= 0.5f;
        CHECK(v == tg::pos3(2));

        v /= 0.5f;
        CHECK(v == tg::pos3(4));

        v += tg::vec3(1, 2, 3);
        CHECK(v == tg::pos3(5, 6, 7));

        v /= tg::size3(2, 3, 2);
        CHECK(v == tg::pos3(2.5f, 2, 3.5f));

        v *= tg::size3(0.5f);
        CHECK(v == tg::pos3(1.25f, 1, 1.75f));

        v -= tg::vec3(1.25f, 1, 0.75f);
        CHECK(v == tg::pos3(0, 0, 1));
    }
}

TEST("OperatorsSize")
{
    // float
    {
        // size @ size
        CHECK(tg::size2(1, 2) + tg::size2(2, 3) == tg::size2(3, 5));
        CHECK(tg::size4(1, 2, 3, 4) - tg::size4(0.5f) == tg::size4(0.5f, 1.5f, 2.5f, 3.5f));

        // size @ size
        CHECK(tg::size2(0.5f, 3) * tg::size2(4, 0.5f) == tg::size2(2, 1.5f));
        CHECK(tg::size2(0.5f, 3) / tg::size2(0.5f, 6) == tg::size2(1, 0.5f));

        // @ size
        CHECK(+tg::size1(0.5f) == tg::size1(0.5f));
        CHECK(-tg::size1(1.5f) == tg::size1(-1.5f));

        // size @ scalar
        CHECK(tg::size3(3, 6, 9) / 3.0f == tg::size3(1, 2, 3));
        CHECK(tg::size3(1, 2, 3) + 1.0f == tg::size3(2, 3, 4));
        CHECK(tg::size3(0.5f, 2.5f, 3.5f) - 2.0f == tg::size3(-1.5f, 0.5f, 1.5f));
        CHECK(tg::size3(0.5f, 2.5f, 3.5f) * 2.0f == tg::size3(1, 5, 7));
    }

    // int
    {
        // size @ size
        CHECK(tg::isize2(1, 2) + tg::isize2(2, 3) == tg::isize2(3, 5));
        CHECK(tg::isize4(1, 2, 3, 4) - tg::isize4(3) == tg::isize4(-2, -1, 0, 1));

        // size @ size
        CHECK(tg::isize2(2, 3) * tg::isize2(4, 2) == tg::isize2(8, 6));
        CHECK(tg::isize2(2, 3) / tg::isize2(3, 3) == tg::isize2(0, 1));

        // @ size
        CHECK(+tg::isize4(2) == tg::isize4(2));
        CHECK(-tg::isize4(3, 5, -6, 8) == tg::isize4(-3, -5, 6, -8));

        // size @ scalar
        CHECK(tg::isize3(3, 6, 9) / 3 == tg::isize3(1, 2, 3));
        CHECK(tg::isize3(1, 2, 3) + 1 == tg::isize3(2, 3, 4));
        CHECK(tg::isize3(1, 2, 3) - 2 == tg::isize3(-1, 0, 1));
        CHECK(tg::isize3(3, 4, 5) * 2 == tg::isize3(6, 8, 10));
    }

    // mixed
    {
        // size @ scalar
        CHECK(tg::size3(3, 6, 9) / 3 == tg::size3(1, 2, 3));
        CHECK(tg::size3(1, 2, 3) + 1 == tg::size3(2, 3, 4));
        CHECK(tg::size3(0.5f, 2.5f, 3.5f) - 2 == tg::size3(-1.5f, 0.5f, 1.5f));
        CHECK(tg::size3(0.5f, 2.5f, 3.5f) * 2 == tg::size3(1, 5, 7));

        // NOTE: these are NOT supported anymore!
        // CHECK(tg::isize3(2, 4, 7) / 4.0f == tg::size3(0.5f, 1, 1.75f));
        // CHECK(tg::isize3(1, 2, 3) + 1.5f == tg::size3(2.5f, 3.5f, 4.5f));
        // CHECK(tg::isize3(1, 2, 3) - 2.5f == tg::size3(-1.5f, -0.5f, 0.5f));
        // CHECK(tg::isize3(2, 3, 4) * 2.5f == tg::size3(5, 7.5f, 10));
    }

    // assignment
    {
        tg::size3 v;
        CHECK(v == tg::size3(0, 0, 0));

        v += 1;
        CHECK(v == tg::size3(1));

        v += 1.5f;
        CHECK(v == tg::size3(2.5f));

        v *= 2;
        CHECK(v == tg::size3(5));

        v /= 2;
        CHECK(v == tg::size3(2.5f));

        v -= 0.5f;
        CHECK(v == tg::size3(2));

        v /= 0.5f;
        CHECK(v == tg::size3(4));

        v += tg::size3(1, 2, 3);
        CHECK(v == tg::size3(5, 6, 7));

        v /= tg::size3(2, 3, 2);
        CHECK(v == tg::size3(2.5f, 2, 3.5f));

        v *= tg::size3(0.5f);
        CHECK(v == tg::size3(1.25f, 1, 1.75f));

        v -= tg::size3(1.25f, 1, 0.75f);
        CHECK(v == tg::size3(0, 0, 1));
    }
}

TEST("OperatorsTriangle")
{
    {
        tg::rng rng;
        auto range2 = tg::aabb2(tg::pos2(-5), tg::pos2(5));

        auto t = tg::triangle2(uniform(rng, range2), uniform(rng, range2), uniform(rng, range2));
        t = t * tg::size2(2);

        CHECK(true);
    }
}
