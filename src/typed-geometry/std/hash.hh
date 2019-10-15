#pragma once

#include <functional>

#include <typed-geometry/tg-lean.hh>

namespace tg
{
namespace detail
{
inline size_t hash_combine(size_t a, size_t b) { return a * 6364136223846793005ULL + b + 0xda3e39cb94b95bdbULL; }
inline size_t hash_combine(size_t a, size_t b, size_t c) { return hash_combine(hash_combine(a, b), c); }
inline size_t hash_combine(size_t a, size_t b, size_t c, size_t d) { return hash_combine(hash_combine(a, b), hash_combine(c, d)); }

template <class T>
size_t hash(T const& a)
{
    return std::hash<T>{}(a);
}
template <class T>
size_t hash(T const& a, T const& b)
{
    return hash_combine(std::hash<T>{}(a), std::hash<T>{}(b));
}
template <class T>
size_t hash(T const& a, T const& b, T const& c)
{
    return hash_combine(std::hash<T>{}(a), std::hash<T>{}(b), std::hash<T>{}(c));
}
template <class T>
size_t hash(T const& a, T const& b, T const& c, T const& d)
{
    return hash_combine(std::hash<T>{}(a), std::hash<T>{}(b), std::hash<T>{}(c), std::hash<T>{}(d));
}
} // namespace detail
} // namespace tg

namespace std
{
// -- scalars
template <class T>
struct hash<tg::angle_t<T>>
{
    std::size_t operator()(tg::angle_t<T> const& v) const noexcept { return tg::detail::hash(v.radians()); }
};

// -- comp
template <class ScalarT>
struct hash<tg::comp<1, ScalarT>>
{
    std::size_t operator()(tg::comp<1, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x); }
};
template <class ScalarT>
struct hash<tg::comp<2, ScalarT>>
{
    std::size_t operator()(tg::comp<2, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x, v.y); }
};
template <class ScalarT>
struct hash<tg::comp<3, ScalarT>>
{
    std::size_t operator()(tg::comp<3, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x, v.y, v.z); }
};
template <class ScalarT>
struct hash<tg::comp<4, ScalarT>>
{
    std::size_t operator()(tg::comp<4, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x, v.y, v.z, v.w); }
};

// -- vec
template <class ScalarT>
struct hash<tg::vec<1, ScalarT>>
{
    std::size_t operator()(tg::vec<1, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x); }
};
template <class ScalarT>
struct hash<tg::vec<2, ScalarT>>
{
    std::size_t operator()(tg::vec<2, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x, v.y); }
};
template <class ScalarT>
struct hash<tg::vec<3, ScalarT>>
{
    std::size_t operator()(tg::vec<3, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x, v.y, v.z); }
};
template <class ScalarT>
struct hash<tg::vec<4, ScalarT>>
{
    std::size_t operator()(tg::vec<4, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x, v.y, v.z, v.w); }
};

// -- dir
template <class ScalarT>
struct hash<tg::dir<1, ScalarT>>
{
    std::size_t operator()(tg::dir<1, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x); }
};
template <class ScalarT>
struct hash<tg::dir<2, ScalarT>>
{
    std::size_t operator()(tg::dir<2, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x, v.y); }
};
template <class ScalarT>
struct hash<tg::dir<3, ScalarT>>
{
    std::size_t operator()(tg::dir<3, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x, v.y, v.z); }
};
template <class ScalarT>
struct hash<tg::dir<4, ScalarT>>
{
    std::size_t operator()(tg::dir<4, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x, v.y, v.z, v.w); }
};

// -- pos
template <class ScalarT>
struct hash<tg::pos<1, ScalarT>>
{
    std::size_t operator()(tg::pos<1, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x); }
};
template <class ScalarT>
struct hash<tg::pos<2, ScalarT>>
{
    std::size_t operator()(tg::pos<2, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x, v.y); }
};
template <class ScalarT>
struct hash<tg::pos<3, ScalarT>>
{
    std::size_t operator()(tg::pos<3, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x, v.y, v.z); }
};
template <class ScalarT>
struct hash<tg::pos<4, ScalarT>>
{
    std::size_t operator()(tg::pos<4, ScalarT> const& v) const noexcept { return tg::detail::hash(v.x, v.y, v.z, v.w); }
};

// -- size
template <class ScalarT>
struct hash<tg::size<1, ScalarT>>
{
    std::size_t operator()(tg::size<1, ScalarT> const& v) const noexcept { return tg::detail::hash(v.width); }
};
template <class ScalarT>
struct hash<tg::size<2, ScalarT>>
{
    std::size_t operator()(tg::size<2, ScalarT> const& v) const noexcept { return tg::detail::hash(v.width, v.height); }
};
template <class ScalarT>
struct hash<tg::size<3, ScalarT>>
{
    std::size_t operator()(tg::size<3, ScalarT> const& v) const noexcept { return tg::detail::hash(v.width, v.height, v.depth); }
};
template <class ScalarT>
struct hash<tg::size<4, ScalarT>>
{
    std::size_t operator()(tg::size<4, ScalarT> const& v) const noexcept { return tg::detail::hash(v.width, v.height, v.depth, v.w); }
};

// -- color
template <class ScalarT>
struct hash<tg::color<3, ScalarT>>
{
    std::size_t operator()(tg::color<3, ScalarT> const& v) const noexcept { return tg::detail::hash(v.r, v.g, v.b); }
};
template <class ScalarT>
struct hash<tg::color<4, ScalarT>>
{
    std::size_t operator()(tg::color<4, ScalarT> const& v) const noexcept { return tg::detail::hash(v.r, v.g, v.b, v.a); }
};

// -- quat
template <class ScalarT>
struct hash<tg::quaternion<ScalarT>>
{
    std::size_t operator()(tg::quaternion<ScalarT> const& v) const noexcept { return tg::detail::hash(v.x, v.y, v.z, v.w); }
};

// -- mat
template <int R, class ScalarT>
struct hash<tg::mat<1, R, ScalarT>>
{
    std::size_t operator()(tg::mat<1, R, ScalarT> const& v) const noexcept { return tg::detail::hash(v[0]); }
};
template <int R, class ScalarT>
struct hash<tg::mat<2, R, ScalarT>>
{
    std::size_t operator()(tg::mat<2, R, ScalarT> const& v) const noexcept { return tg::detail::hash(v[0], v[1]); }
};
template <int R, class ScalarT>
struct hash<tg::mat<3, R, ScalarT>>
{
    std::size_t operator()(tg::mat<3, R, ScalarT> const& v) const noexcept { return tg::detail::hash(v[0], v[1], v[2]); }
};
template <int R, class ScalarT>
struct hash<tg::mat<4, R, ScalarT>>
{
    std::size_t operator()(tg::mat<4, R, ScalarT> const& v) const noexcept { return tg::detail::hash(v[0], v[1], v[2], v[3]); }
};

// TODO: f8 and f16

// -- objects
#define TG_IMPL_OBJECT_HASH1(obj, m0)                                                                          \
    template <int D, class ScalarT>                                                                            \
    struct hash<tg::obj<D, ScalarT>>                                                                           \
    {                                                                                                          \
        std::size_t operator()(tg::obj<D, ScalarT> const& v) const noexcept { return tg::detail::hash(v.m0); } \
    } // force ;
#define TG_IMPL_OBJECT_HASH2(obj, m0, m1)                                                                            \
    template <int D, class ScalarT>                                                                                  \
    struct hash<tg::obj<D, ScalarT>>                                                                                 \
    {                                                                                                                \
        std::size_t operator()(tg::obj<D, ScalarT> const& v) const noexcept { return tg::detail::hash(v.m0, v.m1); } \
    } // force ;
#define TG_IMPL_OBJECT_HASH3(obj, m0, m1, m2)                                                                              \
    template <int D, class ScalarT>                                                                                        \
    struct hash<tg::obj<D, ScalarT>>                                                                                       \
    {                                                                                                                      \
        std::size_t operator()(tg::obj<D, ScalarT> const& v) const noexcept { return tg::detail::hash(v.m0, v.m1, v.m2); } \
    } // force ;

TG_IMPL_OBJECT_HASH2(aabb, min, max);
TG_IMPL_OBJECT_HASH2(ball, center, radius);
TG_IMPL_OBJECT_HASH2(box, center, half_extents);
TG_IMPL_OBJECT_HASH2(capsule, axis, radius);
TG_IMPL_OBJECT_HASH2(cone, base, height);
TG_IMPL_OBJECT_HASH2(cylinder, axis, radius);
TG_IMPL_OBJECT_HASH2(halfspace, normal, dis);
TG_IMPL_OBJECT_HASH3(hemisphere, center, radius, normal);
TG_IMPL_OBJECT_HASH2(hyperplane, normal, dis);
TG_IMPL_OBJECT_HASH3(inf_cone, apex, opening_dir, opening_angle);
TG_IMPL_OBJECT_HASH2(inf_tube, axis, radius);
TG_IMPL_OBJECT_HASH2(line, pos, dir);
TG_IMPL_OBJECT_HASH2(pyramid, base, height);
TG_IMPL_OBJECT_HASH2(ray, origin, dir);
TG_IMPL_OBJECT_HASH2(segment, pos0, pos1);
TG_IMPL_OBJECT_HASH2(sphere, center, radius);
TG_IMPL_OBJECT_HASH3(triangle, pos0, pos1, pos2);
TG_IMPL_OBJECT_HASH2(tube, axis, radius);

template <class ScalarT>
struct hash<tg::circle<2, ScalarT>>
{
    std::size_t operator()(tg::circle<2, ScalarT> const& v) const noexcept { return tg::detail::hash(v.center, v.radius); }
};
template <class ScalarT>
struct hash<tg::circle<3, ScalarT>>
{
    std::size_t operator()(tg::circle<3, ScalarT> const& v) const noexcept { return tg::detail::hash(v.center, v.radius, v.normal); }
};

template <class ScalarT>
struct hash<tg::disk<2, ScalarT>>
{
    std::size_t operator()(tg::disk<2, ScalarT> const& v) const noexcept { return tg::detail::hash(v.center, v.radius); }
};
template <class ScalarT>
struct hash<tg::disk<3, ScalarT>>
{
    std::size_t operator()(tg::disk<3, ScalarT> const& v) const noexcept { return tg::detail::hash(v.center, v.radius, v.normal); }
};

template <class ScalarT>
struct hash<tg::rect<2, ScalarT>>
{
    std::size_t operator()(tg::rect<2, ScalarT> const& v) const noexcept { return tg::detail::hash(v.center, v.length, v.rotation); }
};
template <class ScalarT>
struct hash<tg::rect<3, ScalarT>>
{
    std::size_t operator()(tg::rect<3, ScalarT> const& v) const noexcept { return tg::detail::hash(v.center, v.length, v.normal, v.rotation); }
};

} // namespace std
