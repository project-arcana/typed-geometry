import os

# todo: separate object_functions + object_function_uncommon

common_types = [
    "pos",
    "ray",
    "line",
    "seg",
    "aabb",
    "sphere",
    "triangle",
    "plane"
]

advanced_types = [
    "aabb_boundary",
    "box",
    "box_boundary"
    "capsule", # currently only 3d
    "capsule_boundary",# currently only 3d
    "cone",
    "cone_boundary",
    "cone_boundary_no_caps",
    "cylinder",
    "cylinder_boundary",
    "cylinder_boundary_no_caps",
    "ellipse",
    "ellipse_boundary",
    "frustum", # currently only 3d
    "halfspace",
    "hemisphere",
    "hemisphere_boundary",
    "hemisphere_boundary_no_caps",
    "inf_cone",
    "inf_cone_boundary",
    "inf_cylinder",
    "inf_cylinder_boundary",
    "inf_frustum", # currently only 3d
    # "polygon", # not yet implemented
    # "polyline", # not yet implemented
    # "pyramid", # difficult due to varying base
    "quad", # difficult, because currently no guarantee that planar
    "sphere_boundary",
    "disk",
    "disk_boundary",
    # "circle", # same as disc_boundary
    # "tube", # same as cylinder_boundary_no_caps
    # "inf_tube", # same as inf_cylinder_boundary
    ] 

# todo: <type>2in3

all_types = common_types + advanced_types

print("Total types: {}".format(len(all_types)))

unary_functions = [
    "edges",
    "faces",
    "vertices",
    "volume",
    "area",
    "boundary",
    "rasterize",
    "triangulate",
    "triangulation",
    "centroid",
    "aabb_of",
    "any_point",
    "project",
    "signed_distance"
]

binary_symmetric_functions = [
    "intersects",
    "intersection",  # representation problem
    "closest_points",
    "distance",
    "distance_sqr"
]

binary_asymmetric_functions = [
    "intersection_parameter",
    "intersection_parameters",
    "contains"  # potentially impl difficulty
]

tg_src_root = "" # todo
default_object_functions_path ="typed-geometry/object_functions.hh" # todo
object_functions_dir = "typed-geometry/object-functions/"

if not os.path.exists(object_functions_dir):
    os.makedirs(object_functions_dir)

class code_generator:
    def __init__(self) -> None:
        self.string = ""
        self.current_indent = 0

    def __str__(self) -> str:
        return self.string

    def indent(self, spaces=4) -> None:
        self.current_indent += spaces

    def unindent(self, spaces=4)-> None:
        self.current_indent -= spaces

    def append_line(self, s : str)-> None:
        self.string += " "*self.current_indent + s + "\n"

    def newline(self) -> None:
        self.append_line("")



# Generate template default
default_object_functions = \
"""#pragma once

#include <clean-core/always_false.hh>

template <class T>
struct object_functions
{
    static_assert(cc::always_false<T>
    , \"include missing\");
};"""

with open(default_object_functions_path, "w") as f:
    f.write(default_object_functions)

def generate_function_unary(gen : code_generator, function_name : str):
    # todo: handle return types
    gen.append_line("static constexpr auto {function_name}({type}<D, ScalarT> const& obj)".format(function_name=function_name, type=type))
    gen.append_line("{")
    gen.indent()
    gen.append_line('static_assert(cc::always_false<{type}<D, ScalarT>>, "TODO: not yet implemented");'.format(type=type))
    gen.unindent()
    gen.append_line("}")

def generate_function_binary_symmetric(gen : code_generator, function_name : str, type_a : str, type_b : str):
    gen.append_line("static constexpr auto {function_name}({type_a}<D, ScalarT> const& a, {type_b}<D, ScalarT> const& b)".format(function_name=function_name, type_a=type_a, type_b=type_b))
    gen.append_line("{")
    gen.indent()
    gen.append_line('static_assert(cc::always_false<{type_a}<D, ScalarT>,{type_b}<D, ScalarT>>, "TODO: not yet implemented");'.format(type_a=type_a, type_b=type_b))
    gen.unindent()
    gen.append_line("}")

def generate_function_binary_asymmetric(gen : code_generator, function_name : str, type_a : str, type_b : str):
    gen.append_line("static constexpr auto {function_name}({type_a}<D, ScalarT> const& a, {type_b}<D, ScalarT> const& b)".format(function_name=function_name, type_a=type_a, type_b=type_b))
    gen.append_line("{")
    gen.indent()
    gen.append_line('static_assert(cc::always_false<{type_a}<D, ScalarT>,{type_b}<D, ScalarT>>, "TODO: not yet implemented");'.format(type_a=type_a, type_b=type_b))
    gen.unindent()
    gen.append_line("}")

def generate_object_functions(type : str):
    gen = code_generator()
    gen.append_line("#pragma once")
    gen.newline()
    gen.append_line("#include <{}>".format(default_object_functions_path))
    gen.newline()
    gen.append_line("template <int D, class ScalarT>")
    gen.append_line("struct object_functions<{}<D, ScalarT>>".format(type))
    gen.append_line("{")
    gen.indent()
    for function in unary_functions:
        generate_function_unary(gen, function)
        gen.newline()

    for function in binary_symmetric_functions:
        for other_type in all_types:
            generate_function_binary_symmetric(gen, function, type, other_type)
            gen.newline()        

    for function in binary_asymmetric_functions:
        for other_type in all_types:
            generate_function_binary_asymmetric(gen, function, type, other_type)
            gen.newline()        

    gen.unindent()
    gen.append_line("};")
    

    filepath = object_functions_dir + type + ".hh"
    with open(filepath, "w") as file:
        file.write(gen.string)
    
for type in common_types:
    generate_object_functions(type)
