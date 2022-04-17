#include <nexus/fuzz_test.hh>

#include <typed-geometry/feature/vector.hh>
#include <typed-geometry/feature/matrix.hh>

/// confirms that dot product of vec is positive definite
template <int D = 3, class ScalarT = tg::f32>
static void confPosDef(const tg::vec<D, ScalarT>& v)
{
    auto r = dot(v, v);
    CHECK(r >= 0.0f);
    if (length(v) == 0.0f)
        CHECK(r == nx::approx(0.0f).abs(0.001f));
    else
        CHECK(r != 0.0f);
}

/// confirms that dot product of dir is positive definite
template <int D = 3, class ScalarT = tg::f32>
static void confPosDef(const tg::dir<D, ScalarT>& v)
{
    auto r = dot(v, v);
    // directions have length 1
    CHECK(r > 0.0f);
}

/// returns random pos of dimension D with values between -1 and 1
template <int D, class ScalarT = tg::f32>
static tg::pos<D, ScalarT> randomPos(tg::rng& rng)
{
    const auto aabb = tg::aabb<D, ScalarT>(tg::pos<D, ScalarT>(-1.0f), tg::pos<D, ScalarT>(1.0f));
    return uniform(rng, aabb);
}
/// wraps pos in vec
template <int D, class ScalarT = tg::f32>
static tg::vec<D, ScalarT> randomVec(tg::rng& rng)
{
    return tg::vec<D, ScalarT>(randomPos<D, ScalarT>(rng));
}

/// wraps vec in dir
template <int D, class ScalarT = tg::f32>
static tg::dir<D, ScalarT> randomDir(tg::rng& rng)
{
    return normalize(randomVec<D, ScalarT>(rng));
}

FUZZ_TEST("DotVec")(tg::rng& rng)
{
    { // 3D
        // random angle
        auto theta = tg::degree(tg::abs(randomPos<1>(rng).x) * 360.0f);
        { // dot product of normalized vector with itself = 1
            auto n = normalize(randomVec<3>(rng));
            CHECK(dot(n, n) == nx::approx(1.0f));

            // dot product of random vector with itself is length^2
            auto r = randomVec<3>(rng);
            auto l = length(r);
            auto result = l * l;
            auto calc = dot(r, r);
            CHECK(calc == nx::approx(result));
            // equivalent to..
            CHECK(tg::sqrt(calc) == nx::approx(l));
        }
        { // orthogonality
            // random rotation..
            auto rotMat = rotation_around(normalize(randomVec<3>(rng)), theta);
            // ..of two perpendicular vectors
            auto bx = tg::vec3(1, 0, 0);
            auto by = tg::vec3(0, 1, 0);
            auto v0 = tg::vec3(rotMat * tg::vec4(bx, 0));
            auto v1 = tg::vec3(rotMat * tg::vec4(by, 0));
            // dot product is 0
            auto calc = dot(v0, v1);
            CHECK(calc == nx::approx(0.0f).abs(0.001f));
        }

        { // cosine
            // random normalized vector (|v| > 0)
            auto v0 = tg::vec3::zero;
            while (length(v0) == 0.0f)
                v0 = tg::vec3(normalize(randomVec<3>(rng)));

            // random rotation around perpendicular vector
            auto t = tg::any_normal(v0);
            auto rotMat = rotation_around(t, theta);
            // results in second (normalized) vector
            auto v1 = tg::vec3(rotMat * tg::vec4(v0, 0));
            auto c = cos(theta);
            // dot is cosine(theta)!
            CHECK(dot(v0, v1) == nx::approx(c));
            // symmetry
            CHECK(dot(v1, v0) == nx::approx(c));
        }
        { // 3 random vectors

            auto a = randomVec<3>(rng);
            auto b = randomVec<3>(rng);
            auto c = randomVec<3>(rng);

            // distributive law
            CHECK(dot(a, b + c) == nx::approx(dot(a, b) + dot(a, c)).abs(0.001f));

            // dot product is positive definite
            confPosDef(a);
            confPosDef(b);
            confPosDef(c);

            // symmetry
            CHECK(dot(a, b) == dot(b, a));
            CHECK(dot(a, c) == dot(c, a));
            CHECK(dot(c, b) == dot(b, c));
        }
    }
    { // 2D
        // random angle
        auto theta = tg::degree(tg::abs(randomPos<1>(rng).x) * 360.0f);
        { // dot product of normalized vector with itself = 1
            auto n = normalize(randomVec<2>(rng));
            CHECK(dot(n, n) == nx::approx(1.0f));

            // dot product of random vector with itself is length^2
            auto r = randomVec<2>(rng);
            auto l = length(r);
            auto result = l * l;
            auto calc = dot(r, r);
            CHECK(calc == nx::approx(result));
            // equivalent to..
            CHECK(tg::sqrt(calc) == nx::approx(l));
        }
        {
            { //  two perpendicular vectors
                auto v0 = randomVec<2>(rng);
                auto v1 = perpendicular(v0);
                // dot product is 0
                auto calc = dot(v0, v1);
                CHECK(calc == nx::approx(0.0f).abs(0.001f));
            }
            { // cosine
              // random normalized vector (|v| > 0)
                auto v0 = tg::vec2::zero;
                while (length(v0) == 0.0f)
                    v0 = tg::vec2(normalize(randomVec<2>(rng)));
                // random rotation around origin
                auto rotMat = rotation_around(tg::pos2::zero, theta);
                // results in second (normalized) vector
                auto v1 = tg::vec2(rotMat * tg::vec3(v0, 0));
                auto c = cos(theta);
                // dot is cosine(theta)!
                CHECK(dot(v0, v1) == nx::approx(c));
                // symmetry
                CHECK(dot(v1, v0) == nx::approx(c));
            }
            { // 3 random vectors

                auto a = randomVec<2>(rng);
                auto b = randomVec<2>(rng);
                auto c = randomVec<2>(rng);

                // distributive law
                CHECK(dot(a, b + c) == nx::approx(dot(a, b) + dot(a, c)).abs(0.0001f));

                // dot product is positive definite
                confPosDef<2>(a);
                confPosDef<2>(b);
                confPosDef<2>(c);

                // symmetry
                CHECK(dot(a, b) == dot(b, a));
                CHECK(dot(a, c) == dot(c, a));
                CHECK(dot(c, b) == dot(b, c));
            }
        }
    }
    {     // 1D
        { // dot product of normalized vector with itself = 1
            auto n = normalize(randomVec<1>(rng));
            CHECK(dot(n, n) == nx::approx(1.0f));

            // dot product of random vector with itself is length^2
            auto r = randomVec<1>(rng);
            auto l = length(r);
            auto result = l * l;
            auto calc = dot(r, r);
            CHECK(calc == nx::approx(result));
            // equivalent to..
            CHECK(tg::sqrt(calc) == nx::approx(l));
        }
        {
            { // 3 random vectors

                auto a = randomVec<2>(rng);
                auto b = randomVec<2>(rng);
                auto c = randomVec<2>(rng);

                // distributive law
                CHECK(dot(a, b + c) == nx::approx(dot(a, b) + dot(a, c)).abs(0.0001f));

                // dot product is positive definite
                confPosDef<2>(a);
                confPosDef<2>(b);
                confPosDef<2>(c);

                // symmetry
                CHECK(dot(a, b) == dot(b, a));
                CHECK(dot(a, c) == dot(c, a));
                CHECK(dot(c, b) == dot(b, c));
            }
        }
    }
}

FUZZ_TEST("DotDir")(tg::rng& rng)
{
    {     // 3D
          // random angle
        { // dot product of dir with itself is 1
            auto n = randomDir<3>(rng);
            CHECK(dot(n, n) == nx::approx(1.0f));

            auto r = randomDir<3>(rng);
            auto calc = dot(r, r);
            CHECK(calc == nx::approx(1.0f));
        }
        auto theta = tg::degree(tg::abs(randomPos<1>(rng).x) * 360.0f);
        { // orthogonality
            // random rotation..
            auto rotMat = rotation_around(randomDir<3>(rng), theta);
            // ..of two perpendicular dirs
            auto bx = tg::vec3(1, 0, 0);
            auto by = tg::vec3(0, 1, 0);
            auto d0 = normalize(tg::vec3(rotMat * tg::vec4(bx, 0)));
            auto d1 = normalize(tg::vec3(rotMat * tg::vec4(by, 0)));
            // dot product is 0
            auto calc = dot(d0, d1);
            CHECK(calc == nx::approx(0.0f).abs(0.001f));
        }
    }
    { // cosine
        // random dir
        auto d0 = randomDir<3>(rng);

        // random rotation around perpendicular direction
        auto t = any_normal(d0);
        auto theta = tg::degree(tg::abs(randomPos<1>(rng).x) * 360.0f);
        auto rotMat = rotation_around(t, theta);
        // results in second direction
        auto d1 = normalize(tg::vec3(rotMat * tg::vec4(tg::vec3(d0), 0)));
        auto c = cos(theta);
        // dot is cosine(theta)!
        CHECK(dot(d0, d1) == nx::approx(c));
        // symmetry
        CHECK(dot(d1, d0) == nx::approx(c));
    }
    { // 3 random dirs
        auto a = randomDir<3>(rng);
        auto b = randomDir<3>(rng);
        auto c = randomDir<3>(rng);

        // dot product is positive definite
        confPosDef(a);
        confPosDef(b);
        confPosDef(c);

        // symmetry
        CHECK(dot(a, b) == dot(b, a));
        CHECK(dot(a, c) == dot(c, a));
        CHECK(dot(c, b) == dot(b, c));
    }
    { // 2D
        // random angle
        auto theta = tg::degree(tg::abs(randomPos<1>(rng).x) * 360.0f);
        { // dot product of dir with itself is 1
            auto n = randomDir<2>(rng);
            CHECK(dot(n, n) == nx::approx(1.0f));

            auto r = randomDir<2>(rng);
            auto calc = dot(r, r);
            CHECK(calc == nx::approx(1.0f));
        }
        { //  two perpendicular directions
            auto d0 = randomDir<2>(rng);
            auto d1 = perpendicular(d0);
            // dot product is 0
            auto calc = dot(d0, d1);
            CHECK(calc == nx::approx(0.0f).abs(0.001f));
        }
        { // cosine
          // random direction (|v| > 0)
            auto p = tg::pos2::zero;
            while (tg::abs(p.x) + tg::abs(p.y) == 0.0f) // skip origin (0,0)
                p = randomPos<2>(rng);
            // random rotation around origin
            auto rotMat = rotation_around(tg::pos2::zero, theta);
            // results in second (normalized) direction
            auto d0 = normalize(tg::vec2(p.x, p.y));
            auto d1 = normalize(tg::vec2(rotMat * tg::vec3(p.x, p.y, 0)));
            auto c = cos(theta);
            // dot is cosine(theta)!
            CHECK(dot(d0, d1) == nx::approx(c));
            // symmetry
            CHECK(dot(d1, d0) == nx::approx(c));
        }
        { // 3 random directions
            auto a = randomDir<2>(rng);
            auto b = randomDir<2>(rng);
            auto c = randomDir<2>(rng);

            // dot product is positive definite
            confPosDef<2>(a);
            confPosDef<2>(b);
            confPosDef<2>(c);

            // symmetry
            CHECK(dot(a, b) == dot(b, a));
            CHECK(dot(a, c) == dot(c, a));
            CHECK(dot(c, b) == dot(b, c));
        }
    }
    {     // 1D
        { // dot product of direction with itself is 1
            auto n = randomDir<1>(rng);
            CHECK(dot(n, n) == nx::approx(1.0f));

            auto r = randomDir<1>(rng);
            auto calc = dot(r, r);
            CHECK(calc == nx::approx(1.0f));
        }
        {
            { // 3 random directions
                auto a = randomDir<1>(rng);
                auto b = randomDir<1>(rng);
                auto c = randomVec<1>(rng);

                // distributive law
                CHECK(dot(a, b + c) == nx::approx(dot(a, b) + dot(a, c)).abs(0.0001f));

                // dot product is positive definite
                confPosDef<1>(a);
                confPosDef<1>(b);
                confPosDef<1>(c);

                // symmetry
                CHECK(dot(a, b) == dot(b, a));
                CHECK(dot(a, c) == dot(c, a));
                CHECK(dot(c, b) == dot(b, c));
            }
        }
    }
}
