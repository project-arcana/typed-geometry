import json

in_file = "../src/typed-geometry/functions/objects/aabb.hh"

text = open(in_file, "r").read()

functions = [
    "aabb_of"
]

def index_of_closing(text: str, start: int) -> int:
    opening = text[start]
    assert opening == "{" or opening == "(" or opening == "<"
    closing = "}" if opening == "{" else ")" if opening == "(" else ">"
    count = 1
    for i in range(start + 1, len(text)):
        if text[i] == opening:
            count += 1
        elif text[i] == closing:
            count -= 1

        if count == 0:
            return i
    return -1


def read_type(s: str):
    if "<" in s:
        closing_index = index_of_closing(s, s.index("<"))
        return s[:(closing_index + 1)]
    return s.split()[0]


def parse_template_parameters(s: str):
    start = s.index("<")
    end = index_of_closing(s, start)
    params = s[start + 1:end].split(",")
    parsed_params = []
    for p in params:
        p = p.strip()
        pp = p.split()
        typename = pp[0]
        variable_name = pp[1]
        default_value = ""
        if "=" in variable_name:
            variable_name, default_value = variable_name.split("=")
        variable_name = variable_name.strip()
        default_value = default_value.strip()
        parsed_params.append(
            {"typename": typename, "variable_name": variable_name, "default_value": default_value})
    return parsed_params


def parse_function_declaration(s: str):
    # "[[nodiscard]] constexpr aabb<D, ScalarT> aabb_of(pos<D, ScalarT> const& v)",
    modifyers = []
    if s.startswith("[[nodiscard]]"):
        modifyers.append("[[nodiscard]]")
        s = s[len("[[nodiscard]]"):]
        s = s.strip()

    if s.startswith("constexpr"):
        modifyers.append("constexpr")
        s = s[len("constexpr"):]
        s = s.strip()

    end_idx = s.index("(")
    return_type_and_function_name = s[:end_idx].split()
    function_name = return_type_and_function_name[-1]
    return_type = "".join(return_type_and_function_name[:-1])
    s = s[(1+end_idx):]

    # function parameters
    closing_index = s.index(")")
    function_parameters_raw = s[:closing_index]
    function_parameters = []
    while function_parameters_raw:
        type_name = read_type(function_parameters_raw)
        function_parameters_raw = function_parameters_raw[len(
            type_name):].strip()

        if function_parameters_raw.startswith("const"):
            type_name += " " + "const"
            function_parameters_raw = function_parameters_raw[len(
                "const"):].strip()

        if function_parameters_raw.startswith("&"):
            type_name += "&"
            function_parameters_raw = function_parameters_raw[len(
                "&"):].strip()

        if function_parameters_raw.startswith("..."):
            type_name += "..."
            function_parameters_raw = function_parameters_raw[len(
                "..."):].strip()

        parameter_name = function_parameters_raw.split()[0]
        function_parameters_raw = function_parameters_raw[len(parameter_name):]
        function_parameters_raw = function_parameters_raw.strip()

        default_value = ""
        if function_parameters_raw.startswith("="):
            round_brackets_level = 0
            square_brackets_level = 0
            curly_brackets_level = 0
            angle_brackets_level = 0
            function_parameters_raw = function_parameters_raw[1:].strip()
            offset = 0
            for char in function_parameters_raw:
                if char == "{":
                    curly_brackets_level += 1
                if char == "}":
                    curly_brackets_level -= 1
                if char == "[":
                    square_brackets_level += 1
                if char == "]":
                    square_brackets_level -= 1
                if char == "<":
                    angle_brackets_level += 1
                if char == ">":
                    angle_brackets_level -= 1
                if char == "(":
                    round_brackets_level += 1
                if char == ")":
                    round_brackets_level -= 1
                if char == ",":
                    if curly_brackets_level == 0 and square_brackets_level == 0 and angle_brackets_level == 0 and round_brackets_level == 0:
                        break
                offset += 1
            
            default_value = function_parameters_raw[:offset]
            function_parameters_raw = function_parameters_raw[(offset + 1):].strip()


        if function_parameters_raw.startswith(","):
            function_parameters_raw = function_parameters_raw[1:].strip()
        function_parameters.append(
            {"type_name": type_name, "parameter_name": parameter_name, "default_value": default_value})

    s = s[(closing_index + 1):]
    s = s.strip()

    if return_type == "auto":
        assert (s.startswith("->"))
        s = s[2:]
        s = s.strip()
        return_type = s

    parsed_function = {}
    parsed_function["modyfiers"] = modifyers
    parsed_function["name"] = function_name
    parsed_function["parameters"] = function_parameters
    parsed_function["return_type"] = return_type

    return parsed_function

def collect_functions(text: str):
    lines = text.split('\n')

    functions = []

    line_index = 0
    while line_index < len(lines):
        line = lines[line_index]

        line = line.strip()

        if line == "":
            line_index += 1
            continue

        if line.startswith("[[nodiscard]] constexpr"):
            template_declaration = lines[line_index-1]
            function_declaration = line
            assert lines[line_index+1].strip() == "{"
            body_start = line_index + 2
            body_end = line_index + 2
            while lines[body_end].strip() != "}":
                body_end += 1

            body = ""
            for i in range(body_start, body_end):
                body += lines[i] + "\n"
            line_index = body_end

            function = {}
            function["template-parameters"] = parse_template_parameters(
                template_declaration)
            function["function_declaration"] = parse_function_declaration(
                function_declaration)
            function["body"] = body

            functions.append(function)

        line_index += 1

    print(json.dumps(functions))


collect_functions(text)

# previous_line = ""
# for line in lines:
#     if("aabb_of" in line):
#         print(line)

#     previous_line = line
