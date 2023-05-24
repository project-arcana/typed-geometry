#pragma once

#include <typed-geometry/tg.hh>

namespace helper
{
inline tg::mat3 rndMat3(tg::rng& rng)
{
    auto b = 5.0f;
    auto dBox = tg::aabb3(-tg::pos3(b), tg::pos3(b));

    auto mat = tg::mat3();
    for (auto c = 0; c < 3; c++)
        mat[c] = tg::vec3(uniform(rng, dBox));

    return mat;
}

inline tg::mat2 rndMat2(tg::rng& rng)
{
    auto b = 5.0f;
    auto dBox = tg::aabb2(-tg::pos2(b), tg::pos2(b));

    auto mat = tg::mat2();
    for (auto c = 0; c < 2; c++)
        mat[c] = tg::vec2(uniform(rng, dBox));

    return mat;
}

inline tg::mat3 rndInvertibleMat3(tg::rng& rng)
{
    // build matrix from random transformations (invertible)
    auto rd1 = tg::aabb1(-1.0f, 1.0f);
    auto a = uniform(rng, rd1).x;
    auto b = uniform(rng, rd1).x;
    auto c = uniform(rng, rd1).x;

    auto mat = tg::scaling(tg::size2(b, c));
    mat = tg::rotation_around(tg::pos2(a, c), tg::degree(b * 360.0f)) * mat;
    mat = tg::translation(tg::vec2(a, b) * 10.0f) * mat;

    return mat;
}

inline tg::mat4 rndInvertibleMat4(tg::rng& rng)
{
    // build matrix from random transformations (invertible)
    auto rd1 = tg::aabb1(-1.0f, 1.0f);
    auto a = uniform(rng, rd1).x;
    auto b = uniform(rng, rd1).x;
    auto c = uniform(rng, rd1).x;

    auto mat = tg::scaling(tg::size3(a, b, c));
    mat = tg::rotation_around(normalize(tg::vec3(a, b, c)), tg::degree(b * 360.0f)) * mat;
    mat = tg::translation(tg::vec3(a, b, c) * 10.0f) * mat;

    return mat;
}

inline tg::mat2 rndInvertibleMat2(tg::rng& rng)
{
    auto rd1 = tg::aabb1(-1.0f, 1.0f);
    auto a = uniform(rng, rd1).x;
    auto b = uniform(rng, rd1).x;

    auto mat = tg::scaling(tg::size1(b));
    mat = tg::translation(tg::vec1(a) * 10.0f) * mat;

    return mat;
}
} // namespace helper
