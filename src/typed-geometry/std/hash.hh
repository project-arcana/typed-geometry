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
struct hash<tg::angle<T>>
{
    typedef tg::angle<T> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.radians()); }
};

// -- vec
template <class ScalarT>
struct hash<tg::vec<1, ScalarT>>
{
    typedef tg::vec<1, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.x); }
};
template <class ScalarT>
struct hash<tg::vec<2, ScalarT>>
{
    typedef tg::vec<2, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.x, v.y); }
};
template <class ScalarT>
struct hash<tg::vec<3, ScalarT>>
{
    typedef tg::vec<3, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.x, v.y, v.z); }
};
template <class ScalarT>
struct hash<tg::vec<4, ScalarT>>
{
    typedef tg::vec<4, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.x, v.y, v.z, v.w); }
};

// -- dir
template <class ScalarT>
struct hash<tg::dir<1, ScalarT>>
{
    typedef tg::dir<1, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.x); }
};
template <class ScalarT>
struct hash<tg::dir<2, ScalarT>>
{
    typedef tg::dir<2, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.x, v.y); }
};
template <class ScalarT>
struct hash<tg::dir<3, ScalarT>>
{
    typedef tg::dir<3, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.x, v.y, v.z); }
};
template <class ScalarT>
struct hash<tg::dir<4, ScalarT>>
{
    typedef tg::dir<4, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.x, v.y, v.z, v.w); }
};

// -- pos
template <class ScalarT>
struct hash<tg::pos<1, ScalarT>>
{
    typedef tg::pos<1, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.x); }
};
template <class ScalarT>
struct hash<tg::pos<2, ScalarT>>
{
    typedef tg::pos<2, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.x, v.y); }
};
template <class ScalarT>
struct hash<tg::pos<3, ScalarT>>
{
    typedef tg::pos<3, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.x, v.y, v.z); }
};
template <class ScalarT>
struct hash<tg::pos<4, ScalarT>>
{
    typedef tg::pos<4, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.x, v.y, v.z, v.w); }
};

// -- size
template <class ScalarT>
struct hash<tg::size<1, ScalarT>>
{
    typedef tg::size<1, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.width); }
};
template <class ScalarT>
struct hash<tg::size<2, ScalarT>>
{
    typedef tg::size<2, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.width, v.height); }
};
template <class ScalarT>
struct hash<tg::size<3, ScalarT>>
{
    typedef tg::size<3, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.width, v.height, v.depth); }
};
template <class ScalarT>
struct hash<tg::size<4, ScalarT>>
{
    typedef tg::size<4, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.width, v.height, v.depth, v.w); }
};

// -- mat
template <int R, class ScalarT>
struct hash<tg::mat<1, R, ScalarT>>
{
    typedef tg::mat<1, R, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v[0]); }
};
template <int R, class ScalarT>
struct hash<tg::mat<2, R, ScalarT>>
{
    typedef tg::mat<2, R, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v[0], v[1]); }
};
template <int R, class ScalarT>
struct hash<tg::mat<3, R, ScalarT>>
{
    typedef tg::mat<3, R, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v[0], v[1], v[2]); }
};
template <int R, class ScalarT>
struct hash<tg::mat<4, R, ScalarT>>
{
    typedef tg::mat<4, R, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v[0], v[1], v[2], v[3]); }
};

// TODO: f8 and f16

// -- objects
template <int D, class ScalarT>
struct hash<tg::triangle<D, ScalarT>>
{
    typedef tg::triangle<D, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.pos0, v.pos1, v.pos2); }
};
template <int D, class ScalarT>
struct hash<tg::box<D, ScalarT>>
{
    typedef tg::box<D, ScalarT> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& v) const noexcept { return tg::detail::hash(v.min, v.max); }
};

} // namespace std
