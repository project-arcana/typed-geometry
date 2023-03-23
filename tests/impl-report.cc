#include <nexus/app.hh>
#include <nexus/test.hh>

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <tuple>
#include <vector>

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

template <class T, class F>
using try_contains = decltype(contains(std::declval<T const&>(), std::declval<F const&>()));

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

template <class T, class F>
using try_intersection = decltype(intersection(std::declval<T const&>(), std::declval<F const&>()));

template <class T, class F>
using try_intersects = decltype(intersects(std::declval<T const&>(), std::declval<F const&>()));

template <class T>
using try_closest_ray2_intersect_of = decltype(closest_intersection_parameter(tg::ray2(), std::declval<T const&>()));

template <class T>
using try_closest_ray3_intersect_of = decltype(closest_intersection_parameter(tg::ray3(), std::declval<T const&>()));

template <class T, class F>
using try_closest_points = decltype(closest_points(std::declval<T const&>(), std::declval<F const&>()));

template <class T>
using try_intersects_line3_of = decltype(intersects(std::declval<T const&>(), tg::line3()));

template <class T>
using try_intersects_ray3_of = decltype(intersects(std::declval<T const&>(), tg::ray3()));

template <class T>
using try_intersects_segment2_of = decltype(intersects(std::declval<T const&>(), tg::aabb2()));

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

template <class T>
using try_distance_line3_of = decltype(distance(std::declval<T const&>(), tg::line3()));

template <class T>
using try_distance_ray3_of = decltype(distance(std::declval<T const&>(), tg::ray3()));

template <class T>
using try_distance_segment3_of = decltype(distance(std::declval<T const&>(), tg::segment3()));

template <class T>
using try_distance_aabb2_of = decltype(distance(std::declval<T const&>(), tg::aabb2()));

template <class T>
using try_distance_aabb3_of = decltype(distance(std::declval<T const&>(), tg::aabb3()));

template <class T>
using try_distance_sphere3_of = decltype(distance(std::declval<T const&>(), tg::sphere3()));

template <class T>
using try_distance_box3_of = decltype(distance(std::declval<T const&>(), tg::box3()));

template <class T>
using try_distance_capsule3_of = decltype(distance(std::declval<T const&>(), tg::capsule3()));

template <class T>
using try_distance_cone3_of = decltype(distance(std::declval<T const&()>, tg::cone3()));

template <class T>
using try_distance_cylinder3_of = decltype(distance(std::declval<T const&>(), tg::cylinder3()));

template <class T>
using try_distance_ellipse3_of = decltype(distance(std::declval<T const&>(), tg::ellipse3()));

template <class T>
using try_distance_halfspace3_of = decltype(distance(std::declval<T const&>(), tg::halfspace3()));

template <class T>
using try_distance_hemisphere3_of = decltype(distance(std::declval<T const&>(), tg::hemisphere3()));

template <class T>
using try_distance_triangle3_of = decltype(distance(std::declval<T const&>(), tg::triangle3()));

template <class T>
using try_distance_plane3_of = decltype(distance(std::declval<T const&>(), tg::plane3()));

template <class T>
using try_distance_tube3_of = decltype(distance(std::declval<T const&>(), tg::tube3()));

template <class T>
using try_distance_sphere2in3_of = decltype(distance(std::declval<T const&>(), tg::sphere2in3()));

template <class T, class F>
using try_distance = decltype(distance(std::declval<T const&>(), std::declval<F const&>()));

template <class T, class F>
using try_distance_sqr = decltype(distance_sqr(std::declval<T const&>(), std::declval<F const&>()));

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


// LATEX FILE GENERATOR:
// APP "ImplReport_LATEX" auto generates latex file and compiled .pdf containing implementation report tabulars for supported tg methods.
//
// Implementation guideline for new tabulars for method m:
//  - Check if typedef for m for type_pair check is implemented, otherwise implement according to following example (on top of file):
//       Example:
//           template <class T, class F>
//           using try_intersects = decltype(intersects(std::declval<T const&>(), std::declval<F const&>()));
//
//  - Add TG_APPLY_PAIR struct for m according to following example:
//       Example:
//           TG_APPLY_PAIR(try_intersects, pair_intersects);
//               -> 2nd argument: Name of defined struct
//
//  - In APP:
//       - Add new scope at the end, before "// end document"
//       - std::vector<std::pair<std::string, std::string>> should_not_impl_NEW_METHOD
//          -> containing type pairs where implementation of m would not be meaningful
//       - std::vector<std::pair<std::string, bool>> data_matrix;
//          -> stores implementation information:
//              - data_matrix[0]: first type of tuple (types_table_3D || types_table_2D) with itself
//              - data_matrix[1]: first type of tuple (types_table_3D || types_table_2D) with 2nd type of tuple, ..
//       - fill in data by applying m on all possible type pairs of the chosen tuple (types_table_3D || types_table_2D).
//              - Example:
//                          std::apply([&data_matrix](auto... tuple_args)
//                              { (apply_expander<types_table_3D>(data_matrix, tuple_args, pair_intersects<decltype(tuple_args)>()), ...); },
//                                  types_table_3D{});
//       - write data_matrix: write_tabular(...)
//
// Notes:
//  - Add new type: Add to tuple (types_table_3D || types_table_2D) and add TG_TYPE_NAME_OF macro for new type


// structs for name retrieval of tg types
template <class T>
struct type_name
{
    char const* value = "undefined";
};

#define TG_TYPE_NAME_OF(type, name) \
    template <>                     \
    struct type_name<type>          \
    {                               \
        char const* value = name;   \
    };

TG_TYPE_NAME_OF(tg::segment3, "segment3");
TG_TYPE_NAME_OF(tg::line3, "line3");
TG_TYPE_NAME_OF(tg::ray3, "ray3");
TG_TYPE_NAME_OF(tg::box3, "box3");
TG_TYPE_NAME_OF(tg::sphere3, "sphere3");
TG_TYPE_NAME_OF(tg::aabb3, "aabb3");
TG_TYPE_NAME_OF(tg::capsule3, "capusle3");
TG_TYPE_NAME_OF(tg::cone3, "cone3");
TG_TYPE_NAME_OF(tg::cylinder3, "cylinder3");
TG_TYPE_NAME_OF(tg::ellipse3, "ellipse3");
TG_TYPE_NAME_OF(tg::halfspace3, "halfspace3");
TG_TYPE_NAME_OF(tg::hemisphere3, "hemisphere3");
TG_TYPE_NAME_OF(tg::triangle3, "triangle3");
TG_TYPE_NAME_OF(tg::plane3, "plane3");
TG_TYPE_NAME_OF(tg::tube3, "tube3");
TG_TYPE_NAME_OF(tg::sphere2in3, "sphere2in3");
TG_TYPE_NAME_OF(tg::pos3, "pos3");
TG_TYPE_NAME_OF(tg::segment2, "segment2");
TG_TYPE_NAME_OF(tg::ray2, "ray2");
TG_TYPE_NAME_OF(tg::line2, "line2");
TG_TYPE_NAME_OF(tg::circle2, "circle2");
TG_TYPE_NAME_OF(tg::box2, "box2");
TG_TYPE_NAME_OF(tg::triangle2, "triangle2");
TG_TYPE_NAME_OF(tg::aabb2, "aabb2");
TG_TYPE_NAME_OF(tg::disk2, "disk2");
TG_TYPE_NAME_OF(tg::pos2, "pos2");

#undef TG_TYPE_NAME_OF

template <class T>
char const* type_name_of(T t)
{
    return type_name<T>().value;
}

using types_table_3D = std::tuple< //
    tg::segment3,                  //
    tg::line3,
    tg::ray3,
    tg::box3,
    tg::sphere3,
    tg::aabb3,
    tg::capsule3,
    tg::cone3,
    tg::cylinder3,
    tg::ellipse3,
    tg::halfspace3,
    tg::hemisphere3,
    tg::triangle3,
    tg::plane3,
    tg::tube3,
    tg::sphere2in3,
    tg::pos3>;

using types_table_2D = std::tuple< //
    tg::segment2,
    tg::line2,
    tg::ray2,
    tg::circle2,
    tg::box2,
    tg::triangle2,
    tg::aabb2,
    tg::disk2,
    tg::pos2>;

// returns vector containing information about which pairs to implement for a given function
// 0 -> do implement, 1 -> do NOT implement
std::vector<bool> make_mask_matrix(std::vector<std::pair<std::string, bool>> const& data, std::vector<std::pair<std::string, std::string>> const& not_impl_pairs, int nbr_types)
{
    auto const contains_pair = [&](std::string const& a, std::string const& b) -> bool
    {
        for (auto& z : not_impl_pairs)
        {
            if ((a == z.first && b == z.second) || (a == z.second && b == z.first) || ((a == z.first || b == z.first) && z.second == "*"))
                return true;
        }

        return false;
    };

    std::vector<bool> should_not_impl;

    for (auto x = 0; x < nbr_types; x++)
        for (auto y = 0; y < nbr_types; y++)
        {
            std::cout << data[x].first << " " << data[y].first << std::endl;
            if (contains_pair(data[x].first, data[y].first))
            {
                should_not_impl.push_back(true);
                continue;
            }

            should_not_impl.push_back(false);
        }

    return should_not_impl;
}


#define TG_APPLY_PAIR(type_check, struct_name)                                      \
    template <class T0>                                                             \
    struct struct_name                                                              \
    {                                                                               \
        template <class T1>                                                         \
        static std::pair<std::string, bool> handle_pair(T0 const& t0, T1 const& t1) \
        {                                                                           \
            bool is_impl = true;                                                    \
            if constexpr (!tg::can_apply<type_check, T0, T1>)                       \
                is_impl = false;                                                    \
            auto type_name = type_name_of(T1());                                    \
            return std::pair<std::string, bool>(type_name, is_impl);                \
        }                                                                           \
    };

TG_APPLY_PAIR(try_distance_sqr, pair_distance_sqr);
TG_APPLY_PAIR(try_intersects, pair_intersects);
TG_APPLY_PAIR(try_distance, pair_distance);
TG_APPLY_PAIR(try_intersection, pair_intersection);
TG_APPLY_PAIR(try_closest_points, pair_closest_points);
TG_APPLY_PAIR(try_contains, pair_contains);

#undef TG_APPLY_PAIR

// expander function to enable testing certain method with every type_pair
// check every type in Tuple with T0 according to function F
template <class Tuple, class T0, template <typename P> class F>
void apply_expander(std::vector<std::pair<std::string, bool>>& data_matrix, T0 const& type_pair, F<T0> const& f)
{
    std::apply([&data_matrix](auto... tuple_args) { (data_matrix.push_back(F<T0>::handle_pair(T0{}, tuple_args)), ...); }, Tuple{});
}

APP("ImplReport_LATEX")
{
    // file generation
    std::ofstream("impl_report.tex");
    std::fstream f;
    f.open("impl_report.tex", std::ios::trunc);
    f.close();
    f.open("impl_report.tex", std::ios::out);

    // header
    f << "\\documentclass{scrartcl}\n";
    f << "\\usepackage[utf8]{inputenc}\n";
    f << "\\usepackage[table]{xcolor}\n";
    f << "\\usepackage{graphicx}\n";
    f << "\\usepackage{multirow}\n";
    f << "\\usepackage{geometry}\n";
    f << "\\geometry{\n"
      << "a4paper,\n"
      << "left=10mm,\n"
      << "right=10mm,\n"
      << "top=20mm,\n"
      << "}\n";
    f << "\\usepackage[headsepline]{scrlayer-scrpage}\n";
    f << "\\pagestyle{scrheadings}\n";
    f << "\\clearpairofpagestyles\n";
    f << "\\chead{IMPL REPORT}\n";
    f << "\\ihead{\\today}\n";
    f << "\\setlength{\\arrayrulewidth}{0.5mm}\n";
    f << "\\renewcommand{\\arraystretch}{1.5}\n" << std::endl;
    f << "\\newcolumntype{M}[1]{>{\\centering\\arraybackslash}m{#1}}\n" << std::endl;
    f << "\\newcolumntype{L}[1]{>{\\raggedright\\arraybackslash}m{#1}}\n" << std::endl;
    f << "\\newcolumntype{s}{>{\\columncolor[HTML]{E6E6E6}} L{2.3cm}}\n" << std::endl;
    f << "\\newcolumntype{e}{>{\\columncolor[HTML]{E6E6E6}} M{3cm}}\n" << std::endl;
    f << "\\newcommand{\\nondefCol}[1]{\\cellcolor[HTML]{FF3F16}}\n";
    f << "\\newcommand{\\defCol}[1]{\\cellcolor[HTML]{97E26F}}\n";
    f << "\\newcommand{\\notplannedCol}[1]{\\cellcolor[HTML]{3B3B3B}}\n" << std::endl;
    f << "\\title{IMPL REPORT}\n";
    f << "\\begin{document}\n" << std::endl;
    f.close();

    // test tables

    // legend
    f.open("impl_report.tex", std::ios::out | std::ios::app);
    f << "\\begin{tabular}{|e|e|e|} \\hline \n";
    f << "\\cellcolor[HTML]{E6E6E6} DEFINED & \\cellcolor[HTML]{E6E6E6} UNDEFINED & \\cellcolor[HTML]{E6E6E6} SHOULD NOT \\\\ \n";
    f << "\\defCol{} & \\nondefCol{} & \\notplannedCol{} \\\\ \\hline \n";
    f << "\\end{tabular}" << std::endl;
    f << "\\vspace{1.5cm} \n" << std::endl;

    f.close();

    // Func writing tabular based on arguments
    auto const write_tabular
        = [&](std::vector<std::pair<std::string, bool>> matrix, std::string tabular_name, std::vector<bool> should_not_impl, int nbr_elements)
    {
        // cell_width
        float cell_width = 0.4f / (nbr_elements + 1);

        f.open("impl_report.tex", std::ios::out | std::ios::app);
        f << "\\begin{tabular}{|s|";

        for (auto i = 0; i < int(nbr_elements); i++)
            f << "b{" << std::to_string(cell_width) << "\\linewidth}|";

        f << "} \\hline" << std::endl;

        // header row
        f << "\\cellcolor[HTML]{FF9D88} " << tabular_name;

        for (auto i = 0; i < nbr_elements; i++)
        {
            if (cell_width > 0.15)
                f << "& \\cellcolor[HTML]{E6E6E6} " << matrix[i].first;

            else
                f << "& \\cellcolor[HTML]{E6E6E6} "
                  << "\\rotatebox{90}{" << matrix[i].first << " "
                  << "}";
        }
        f << " \\\\ \\hline" << std::endl;

        // data rows
        for (auto i = 0; i < nbr_elements; i++)
        {
            auto& data = matrix[i];
            int offset = nbr_elements * i;

            f << "\\cellcolor[HTML]{E6E6E6} " << matrix[offset + i].first << " ";

            for (auto row_it = 0; row_it < nbr_elements; row_it++)
            {
                if (matrix[offset + row_it].second)
                    f << "& \\defCol{} ";
                else if (!matrix[offset + row_it].second && should_not_impl[offset + row_it])
                    f << "& \\notplannedCol{} ";
                else
                    f << "& \\nondefCol{} ";
            }

            f << "\\\\ \\hline" << std::endl;
        }

        // end of TABLE
        f << "\\end{tabular}" << std::endl;
        f << "\\newline" << std::endl;
        f << "\\vspace{1.5cm} \n" << std::endl;
        f.close();
    };

    // ### Writing Tables ###

    // TABLE distance 3D
    {
        // List of not-impl pairs, gets overwritten when already implemented
        // * -> wildcard (only applicable on 2nd position)
        std::vector<std::pair<std::string, std::string>> should_not_impl_distance3D_pairs = {
            {"line3", "halfspace3"}, //
            {"line3", "plane3"}      //
        };

        // data matrix
        std::vector<std::pair<std::string, bool>> data_matrix;

        // fill up data matrix
        std::apply([&data_matrix](auto... tuple_args)
                   { (apply_expander<types_table_3D>(data_matrix, tuple_args, pair_distance<decltype(tuple_args)>()), ...); },
                   types_table_3D{});

        // write tabular distance 3D
        write_tabular(data_matrix, "\\centering{distance 3D}",
                      make_mask_matrix(data_matrix, should_not_impl_distance3D_pairs, std::tuple_size_v<types_table_3D>), std::tuple_size_v<types_table_3D>);
    }

    // TABLE distance_sqr 3D
    {
        // List of not-impl pairs, gets overwritten when already implemented
        // * -> wildcard (only applicable on 2nd position)
        std::vector<std::pair<std::string, std::string>> should_not_impl_distance_sqr3D_pairs = {};

        // data matrix
        std::vector<std::pair<std::string, bool>> data_matrix;

        // fill up data matrix
        std::apply([&data_matrix](auto... tuple_args)
                   { (apply_expander<types_table_3D>(data_matrix, tuple_args, pair_distance_sqr<decltype(tuple_args)>()), ...); },
                   types_table_3D{});

        // write tabular distance_sqr 3D
        write_tabular(data_matrix, "\\centering{distance sqr 3D}",
                      make_mask_matrix(data_matrix, should_not_impl_distance_sqr3D_pairs, std::tuple_size_v<types_table_3D>), std::tuple_size_v<types_table_3D>);
    }

    // TABLE intersects 3D
    {
        // List of not-impl pairs, gets overwritten when already implemented
        // * -> wildcard (only applicable on 2nd position)
        std::vector<std::pair<std::string, std::string>> should_not_impl_intersects3D_pairs = {
            {"segment3", "segment3"}, //
            {"segment3", "line3"},    //
            {"segment3", "ray3"},     //
            {"line3", "line3"},       //
            {"line3", "ray3"},        //
            {"ray3", "ray3"}          //
        };

        // data matrix
        std::vector<std::pair<std::string, bool>> data_matrix;

        // fill up data matrix
        std::apply([&data_matrix](auto... tuple_args)
                   { (apply_expander<types_table_3D>(data_matrix, tuple_args, pair_intersects<decltype(tuple_args)>()), ...); },
                   types_table_3D{});

        // write tabular intersects 3D
        write_tabular(data_matrix, "\\centering{intersects 3D}",
                      make_mask_matrix(data_matrix, should_not_impl_intersects3D_pairs, std::tuple_size_v<types_table_3D>), std::tuple_size_v<types_table_3D>);
    }

    // Table intersection 3D
    {
        // List of not-impl pairs, gets overwritten when already implemented
        // * -> wildcard (only applicable on 2nd position)
        std::vector<std::pair<std::string, std::string>> should_not_impl_intersection3D_pairs = {
            {"segment3", "segment3"}, //
            {"segment3", "line3"},    //
            {"segment3", "ray3"},     //
            {"line3", "line3"},       //
            {"line3", "ray3"},        //
            {"ray3", "ray3"},         //
            {"pos3", "*"}             //
        };

        // data matrix
        std::vector<std::pair<std::string, bool>> data_matrix;

        // fill up data matrix
        std::apply([&data_matrix](auto... tuple_args)
                   { (apply_expander<types_table_3D>(data_matrix, tuple_args, pair_intersection<decltype(tuple_args)>()), ...); },
                   types_table_3D{});

        // write tabular intersection 3D
        write_tabular(data_matrix, "\\centering{intersection 3D}",
                      make_mask_matrix(data_matrix, should_not_impl_intersection3D_pairs, std::tuple_size_v<types_table_3D>), std::tuple_size_v<types_table_3D>);
    }

    // TABLE closest_points 3D
    {
        // List of not-impl pairs, gets overwritten when already implemented
        // * -> wildcard (only applicable on 2nd position)
        std::vector<std::pair<std::string, std::string>> should_not_impl_closest_points3D_pairs = {};

        // data matrix
        std::vector<std::pair<std::string, bool>> data_matrix;

        // fill up data matrix
        std::apply([&data_matrix](auto... tuple_args)
                   { (apply_expander<types_table_3D>(data_matrix, tuple_args, pair_closest_points<decltype(tuple_args)>()), ...); },
                   types_table_3D{});

        // write tabular closest_points 3D
        write_tabular(data_matrix, "\\centering{closest{\\_}points 3D}",
                      make_mask_matrix(data_matrix, should_not_impl_closest_points3D_pairs, std::tuple_size_v<types_table_3D>),
                      std::tuple_size_v<types_table_3D>);
    }

    // TABLE contains 3D
    {
        // List of not-impl pairs, gets overwritten when already implemented
        // * -> wildcard (only applicable on 2nd position)
        std::vector<std::pair<std::string, std::string>> should_not_impl_contains3D = {};

        // data matrix
        std::vector<std::pair<std::string, bool>> data_matrix;

        // fill up data matrix
        std::apply([&data_matrix](auto... tuple_args)
                   { (apply_expander<types_table_3D>(data_matrix, tuple_args, pair_contains<decltype(tuple_args)>()), ...); },
                   types_table_3D{});

        // write tabular closest_points 3D
        write_tabular(data_matrix, "\\centering{contains 3D}",
                      make_mask_matrix(data_matrix, should_not_impl_contains3D, std::tuple_size_v<types_table_3D>), std::tuple_size_v<types_table_3D>);
    }

    // TABLE distance 2D
    {
        // List of not-impl pairs, gets overwritten when already implemented
        // * -> wildcard (only applicable on 2nd position)
        std::vector<std::pair<std::string, std::string>> should_not_impl_distance2D_pairs = {};

        // data matrix
        std::vector<std::pair<std::string, bool>> data_matrix;

        // fill up data matrix
        std::apply([&data_matrix](auto... tuple_args)
                   { (apply_expander<types_table_2D>(data_matrix, tuple_args, pair_distance<decltype(tuple_args)>()), ...); },
                   types_table_2D{});

        // write tabular distance 2D
        write_tabular(data_matrix, "\\centering{distance 2D}",
                      make_mask_matrix(data_matrix, should_not_impl_distance2D_pairs, std::tuple_size_v<types_table_2D>), std::tuple_size_v<types_table_2D>);
    }

    // TABLE intersects 2D
    {
        // List of not-impl pairs, gets overwritten when already implemented
        // * -> wildcard (only applicable on 2nd position)
        std::vector<std::pair<std::string, std::string>> should_not_impl_intersects2D_pairs = {};

        // data matrix
        std::vector<std::pair<std::string, bool>> data_matrix;

        // fill up data matrix
        std::apply([&data_matrix](auto... tuple_args)
                   { (apply_expander<types_table_2D>(data_matrix, tuple_args, pair_intersects<decltype(tuple_args)>()), ...); },
                   types_table_2D{});

        // write tabular intersects 2D
        write_tabular(data_matrix, "\\centering{intersects 2D}",
                      make_mask_matrix(data_matrix, should_not_impl_intersects2D_pairs, std::tuple_size_v<types_table_2D>), std::tuple_size_v<types_table_2D>);
    }

    // TABLE intersection 2D
    {
        // List of not-impl pairs, gets overwritten when already implemented
        // * -> wildcard (only applicable on 2nd position)
        std::vector<std::pair<std::string, std::string>> should_not_impl_intersection2D_pairs = {
            {"pos2", "*"} //
        };

        // data matrix
        std::vector<std::pair<std::string, bool>> data_matrix;

        // fill_up data matrix
        std::apply([&data_matrix](auto... tuple_args)
                   { (apply_expander<types_table_2D>(data_matrix, tuple_args, pair_intersection<decltype(tuple_args)>()), ...); },
                   types_table_2D{});

        // write tabular intersection 2D
        write_tabular(data_matrix, "\\centering{intersection 2D}",
                      make_mask_matrix(data_matrix, should_not_impl_intersection2D_pairs, std::tuple_size_v<types_table_2D>), std::tuple_size_v<types_table_2D>);
    }

    // end document
    f.open("impl_report.tex", std::ios::out | std::ios::app);
    f << "\\end{document}" << std::endl;
    f.close();


    // compile
    std::system("pdflatex impl_report.tex");
}

#endif
