#include <nexus/test.hh>

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>

#if defined(TG_IMPLEMENTATION_REPORT)

#include <typed-geometry/tg-std.hh>

namespace
{

template <class T>
using try_print = decltype(std::declval<std::ostream&>() << std::declval<T>());

template <class T>
using try_equal = decltype(std::declval<T>() == std::declval<T>() || std::declval<T>() != std::declval<T>());

template <class T>
using try_less = decltype(std::map<T, int>().find(std::declval<T>()));

template <class T>
using try_hash = decltype(std::hash<T>()(std::declval<T const&>()));

template <class T>
using try_any_point = decltype(any_point(std::declval<T const&>()));

template <class T>
using try_project_pos2 = decltype(project(tg::pos2(), std::declval<T const&>()));

template <class T>
using try_contains_pos2 = decltype(contains(std::declval<T const&>(), tg::pos2()));

template <class T>
using try_project_pos3 = decltype(project(tg::pos3(), std::declval<T const&>()));

template <class T>
using try_contains_pos3 = decltype(contains(std::declval<T const&>(), tg::pos3()));

template <class T>
using try_uniform = decltype(uniform(std::declval<tg::rng>(), std::declval<T const&>()));

template <class T>
using try_aabb_of = decltype(aabb_of(std::declval<T const&>()));

template <class T>
using try_centroid_of = decltype(centroid_of(std::declval<T const&>()));

template <class T>
using try_length_of = decltype(length(std::declval<T const&>()));

template <class T>
using try_area_of = decltype(area_of(std::declval<T const&>()));

template <class T>
using try_volume_of = decltype(volume_of(std::declval<T const&>()));

template <class T>
using try_perimeter_of = decltype(perimeter_of(std::declval<T const&>()));

template <class T>
using try_closest_ray2_intersect_of = decltype(closest_intersection_parameter(tg::ray2(), std::declval<T const&>()));

template <class T>
using try_closest_ray3_intersect_of = decltype(closest_intersection_parameter(tg::ray3(), std::declval<T const&>()));

template <class T>
using try_intersects_aabb2_of = decltype(intersects(std::declval<T const&>(), tg::aabb2()));

template <class T>
using try_intersects_aabb3_of = decltype(intersects(std::declval<T const&>(), tg::aabb3()));

template <class T>
using try_intersects_segment3_of = decltype(intersects(std::declval<T const&>(), tg::segment3()));

template <class T>
using try_intersects_sphere3_of = decltype(intersects(std::declval<T const&>(), tg::sphere3()));

template <class T>
using try_intersects_box3_of = decltype(intersects(std::declval<T const&>(), tg::box3()));

template <class T>
using try_intersects_capsule3_of = decltype(intersects(std::declval<T const&>(), tg::capsule3()));

template <class T>
using try_intersects_cone3_of = decltype(intersects(std::declval<T const&()>, tg::cone3()));

template <class T>
using try_intersects_cylinder3_of = decltype(intersects(std::declval<T const&>(), tg::cylinder3()));

template <class T>
using try_intersects_ellipse3_of = decltype(intersects(std::declval<T const&>(), tg::ellipse3()));

template <class T>
using try_intersects_halfspace3_of = decltype(intersects(std::declval<T const&>(), tg::halfspace3()));

template <class T>
using try_intersects_hemisphere3_of = decltype(intersects(std::declval<T const&>(), tg::hemisphere3()));

template <class T>
using try_intersects_triangle3_of = decltype(intersects(std::declval<T const&>(), tg::triangle3()));

template <class T>
using try_intersects_plane3_of = decltype(intersects(std::declval<T const&>(), tg::plane3()));

template <class T>
using try_intersects_tube3_of = decltype(intersects(std::declval<T const&>(), tg::tube3()));

template <class T>
using try_intersects_sphere2in3_of = decltype(intersects(std::declval<T const&>(), tg::sphere2in3()));

template <class T>
using try_solid_of = decltype(solid_of(std::declval<T const&>()));

template <class ObjT>
void test_single_object_type(std::string name)
{
    static auto constexpr domainD = tg::object_traits<ObjT>::domain_dimension;
    static auto constexpr objectD = tg::object_traits<ObjT>::object_dimension;
    static auto constexpr solidD = []
    {
        if constexpr (tg::can_apply<try_solid_of, ObjT>)
            return tg::object_traits<try_solid_of<ObjT>>::object_dimension;
        return objectD;
    }(); // ternary constexpr of solid domain with fallback to object domain

    if constexpr (!std::is_default_constructible_v<ObjT>)
        std::cerr << "cannot default construct tg::" << name << std::endl;

    if constexpr (tg::can_apply<try_print, ObjT>)
    {
        std::stringstream ss;
        ss << ObjT{};
        const auto s = ss.str();
        const auto test_name = name;
        if (name.size() >= 7 && std::string_view(name).substr(0, 7) == "pyramid")
            name += ")"; // closing brace for nested types
        if (s.find(test_name) == std::string::npos)
            std::cerr << "possible error in outputting tg::" << name << ": '" << s << "'" << std::endl;
    }
    else
        std::cerr << "cannot print tg::" << name << std::endl;

    if constexpr (!tg::can_apply<try_equal, ObjT>)
        std::cerr << "equality not implemented for tg::" << name << std::endl;

    // TODO: somehow doesnt work?
    if constexpr (!tg::can_apply<try_less, ObjT>)
        std::cerr << "std::less not specialized for tg::" << name << std::endl;

    if constexpr (!tg::can_apply<try_hash, ObjT>)
        std::cerr << "std::hash not specialized for tg::" << name << std::endl;

    if constexpr (!tg::can_apply<try_any_point, ObjT>)
        std::cerr << "no any_point(tg::" << name << ")" << std::endl;

    if constexpr (domainD == 2)
    {
        if constexpr (!tg::can_apply<try_project_pos2, ObjT>)
        {
            if (!(name.size() >= 7 && std::string_view(name).substr(0, 7) == "ellipse")) // project(ellipse) is not planned
                std::cerr << "no project(tg::pos2, tg::" << name << ")" << std::endl;
        }

        if constexpr (!tg::can_apply<try_contains_pos2, ObjT>)
            std::cerr << "no contains(tg::" << name << ", tg::pos2)" << std::endl;
    }
    else if constexpr (domainD == 3)
    {
        if constexpr (!tg::can_apply<try_project_pos3, ObjT>)
        {
            if (!(name.size() >= 7 && std::string_view(name).substr(0, 7) == "ellipse")) // project(ellipse) is not planned
                std::cerr << "no project(tg::pos3, tg::" << name << ")" << std::endl;
        }

        if constexpr (!tg::can_apply<try_contains_pos3, ObjT>)
            std::cerr << "no contains(tg::" << name << ", tg::pos3)" << std::endl;
    }
    else
        static_assert(tg::always_false_v<domainD>, "not implemented");

    // operations for finite objects
    if constexpr (tg::object_traits<ObjT>::is_finite)
    {
        if constexpr (!tg::can_apply<try_uniform, ObjT>)
            std::cerr << "no uniform(tg::rng, tg::" << name << ")" << std::endl;

        if constexpr (!tg::can_apply<try_aabb_of, ObjT>)
            std::cerr << "no aabb_of(tg::" << name << ")" << std::endl;

        if constexpr (!tg::can_apply<try_centroid_of, ObjT>)
            std::cerr << "no centroid_of(tg::" << name << ")" << std::endl;

        if constexpr (objectD == 1)
        {
            if constexpr (solidD == 1 && !tg::can_apply<try_length_of, ObjT>)
                std::cerr << "no length(tg::" << name << ")" << std::endl;

            if constexpr (solidD != 1 && !tg::can_apply<try_perimeter_of, ObjT>)
                std::cerr << "no perimeter_of(tg::" << name << ")" << std::endl;
        }
        else if constexpr (objectD == 2)
        {
            if constexpr (!tg::can_apply<try_area_of, ObjT>)
                std::cerr << "no area_of(tg::" << name << ")" << std::endl;

            if constexpr (solidD == 2 && !tg::can_apply<try_perimeter_of, ObjT>)
                std::cerr << "no perimeter_of(tg::" << name << ")" << std::endl;
        }
        else if constexpr (objectD == 3)
        {
            if constexpr (!tg::can_apply<try_volume_of, ObjT>)
                std::cerr << "no volume_of(tg::" << name << ")" << std::endl;

            if constexpr (solidD == 3 && !tg::can_apply<try_area_of, ObjT>)
                std::cerr << "no area_of(tg::" << name << ")" << std::endl;
        }
        else
            static_assert(tg::always_false_v<objectD>, "not implemented");
    }

    // ray intersections
    if constexpr (objectD >= domainD - 1)
    {
        if constexpr (domainD == 2 && !tg::can_apply<try_closest_ray2_intersect_of, ObjT>)
            std::cerr << "no closest_intersection_parameter(tg::ray2, tg::" << name << ")" << std::endl;
        if constexpr (domainD == 3 && !tg::can_apply<try_closest_ray3_intersect_of, ObjT>)
            std::cerr << "no closest_intersection_parameter(tg::ray3, tg::" << name << ")" << std::endl;

        if constexpr (domainD == 2 && !tg::can_apply<try_intersects_aabb2_of, ObjT>)
            std::cerr << "no intersects(tg::" << name << ", tg::aabb2)" << std::endl;
        if constexpr (domainD == 3 && !tg::can_apply<try_intersects_aabb3_of, ObjT>)
            std::cerr << "no intersects(tg::" << name << ", tg::aabb3)" << std::endl;

        if constexpr (domainD == 3 && !tg::can_apply<try_intersects_box3_of, ObjT>)
            std::cerr << "no intersects(tg::" << name << ", tg::box3)" << std::endl;

        if constexpr (domainD == 3 && !tg::can_apply<try_intersects_segment3_of, ObjT>)
            std::cerr << "no intersects(tg::" << name << ", tg::segment3)" << std::endl;

        if constexpr (domainD == 3 && !tg::can_apply<try_intersects_sphere3_of, ObjT>)
            std::cerr << "no intersects(tg::" << name << ", tg::sphere3)" << std::endl;

        if constexpr (domainD == 3 && !tg::can_apply<try_intersects_capsule3_of, ObjT>)
            std::cerr << "no intersects(tg::" << name << ", tg::capsule3)" << std::endl;

        if constexpr (domainD == 3 && !tg::can_apply<try_intersects_cone3_of, ObjT>)
            std::cerr << "no intersects(tg::" << name << ", tg::cone3)" << std::endl;

        if constexpr (domainD == 3 && !tg::can_apply<try_intersects_cylinder3_of, ObjT>)
            std::cerr << "no intersects(tg::" << name << ", tg::cylinder3)" << std::endl;

        if constexpr (domainD == 3 && !tg::can_apply<try_intersects_ellipse3_of, ObjT>)
            std::cerr << "no intersects(tg::" << name << ", tg::ellipse3)" << std::endl;

        if constexpr (domainD == 3 && !tg::can_apply<try_intersects_halfspace3_of, ObjT>)
            std::cerr << "no intersects(tg::" << name << ", tg::halfspace3)" << std::endl;

        if constexpr (domainD == 3 && !tg::can_apply<try_intersects_hemisphere3_of, ObjT>)
            std::cerr << "no intersects(tg::" << name << ", tg::hemisphere3)" << std::endl;

        if constexpr (domainD == 3 && !tg::can_apply<try_intersects_triangle3_of, ObjT>)
            std::cerr << "no intersects(tg::" << name << ", tg::triangle3)" << std::endl;

        if constexpr (domainD == 3 && !tg::can_apply<try_intersects_plane3_of, ObjT>)
            std::cerr << "no intersects(tg::" << name << ", tg::plane3)" << std::endl;

        if constexpr (domainD == 3 && !tg::can_apply<try_intersects_tube3_of, ObjT>)
            std::cerr << "no intersects(tg::" << name << ", tg::tube3)" << std::endl;

        if constexpr (domainD == 3 && !tg::can_apply<try_intersects_sphere2in3_of, ObjT>)
            std::cerr << "no intersects(tg::" << name << ", tg::sphere2in3)" << std::endl;

        // TODO: more
    }
}

template <template <int, class> class ObjT>
void test_object_type(std::string const& basename)
{
    test_single_object_type<ObjT<2, float>>(basename + "2");
    test_single_object_type<ObjT<3, float>>(basename + "3");
}
template <template <int, class, class> class ObjT>
void test_object_type_boundary(std::string const& basename)
{
    test_single_object_type<ObjT<2, float, tg::default_object_tag>>(basename + "2");
    test_single_object_type<ObjT<3, float, tg::default_object_tag>>(basename + "3");
    test_single_object_type<ObjT<2, float, tg::boundary_tag>>(basename + "2_boundary");
    test_single_object_type<ObjT<3, float, tg::boundary_tag>>(basename + "3_boundary");
}
template <template <int, class, class> class ObjT>
void test_object_type_3_boundary(std::string const& basename)
{
    test_single_object_type<ObjT<3, float, tg::default_object_tag>>(basename + "3");
    test_single_object_type<ObjT<3, float, tg::boundary_tag>>(basename + "3_boundary");
}
template <template <int, class, class> class ObjT>
void test_object_type_boundary_caps(std::string const& basename)
{
    test_single_object_type<ObjT<2, float, tg::default_object_tag>>(basename + "2");
    test_single_object_type<ObjT<3, float, tg::default_object_tag>>(basename + "3");
    test_single_object_type<ObjT<2, float, tg::boundary_tag>>(basename + "2_boundary");
    test_single_object_type<ObjT<3, float, tg::boundary_tag>>(basename + "3_boundary");
    test_single_object_type<ObjT<2, float, tg::boundary_no_caps_tag>>(basename + "2_boundary_no_caps");
    test_single_object_type<ObjT<3, float, tg::boundary_no_caps_tag>>(basename + "3_boundary_no_caps");
}
template <template <int, class, class> class ObjT>
void test_object_type_3_boundary_caps(std::string const& basename)
{
    test_single_object_type<ObjT<3, float, tg::default_object_tag>>(basename + "3");
    test_single_object_type<ObjT<3, float, tg::boundary_tag>>(basename + "3_boundary");
    test_single_object_type<ObjT<3, float, tg::boundary_no_caps_tag>>(basename + "3_boundary_no_caps");
}
template <template <int, class, int, class> class ObjT>
void test_object_type_23_boundary(std::string const& basename)
{
    test_single_object_type<ObjT<2, float, 2, tg::default_object_tag>>(basename + "2");
    test_single_object_type<ObjT<2, float, 3, tg::default_object_tag>>(basename + "2in3");
    test_single_object_type<ObjT<3, float, 3, tg::default_object_tag>>(basename + "3");
    test_single_object_type<ObjT<2, float, 2, tg::boundary_tag>>(basename + "2_boundary");
    test_single_object_type<ObjT<2, float, 3, tg::boundary_tag>>(basename + "2in3_boundary");
    test_single_object_type<ObjT<3, float, 3, tg::boundary_tag>>(basename + "3_boundary");
}
template <template <class, class> class ObjT, class NestedT>
void test_object_type_nested_boundary_caps(std::string const& basename, std::string const& nestedName)
{
    test_single_object_type<ObjT<NestedT, tg::default_object_tag>>(basename + "(" + nestedName);
    test_single_object_type<ObjT<NestedT, tg::boundary_tag>>(basename + "_boundary(" + nestedName);
    test_single_object_type<ObjT<NestedT, tg::boundary_no_caps_tag>>(basename + "_boundary_no_caps(" + nestedName);
}
}

TEST("implementation report")
{
    // objects
    test_object_type<tg::line>("line");
    test_object_type<tg::ray>("ray");
    test_object_type<tg::segment>("segment");
    test_object_type<tg::triangle>("triangle");
    test_object_type<tg::quad>("quad");
    test_object_type<tg::halfspace>("halfspace");
    test_object_type<tg::plane>("plane");

    test_object_type_boundary<tg::aabb>("aabb");
    test_object_type_boundary<tg::inf_cone>("inf_cone");
    test_object_type_boundary<tg::inf_cylinder>("inf_cylinder");

    test_object_type_boundary_caps<tg::hemisphere>("hemisphere");

    test_object_type_3_boundary<tg::capsule>("capsule");
    test_object_type_3_boundary_caps<tg::cylinder>("cylinder");

    test_object_type_23_boundary<tg::box>("box");
    test_object_type_23_boundary<tg::ellipse>("ellipse");
    test_object_type_23_boundary<tg::sphere>("sphere");

    test_object_type_nested_boundary_caps<tg::pyramid, tg::sphere2in3>("pyramid", "sphere2in3");
    test_object_type_nested_boundary_caps<tg::pyramid, tg::box2in3>("pyramid", "box2in3");
    test_object_type_nested_boundary_caps<tg::pyramid, tg::triangle3>("pyramid", "triangle3");
    // test_object_type_nested_boundary_caps<tg::pyramid, tg::quad3>("pyramid", "quad3");
}

#endif
