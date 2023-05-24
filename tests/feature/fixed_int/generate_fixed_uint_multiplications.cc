#include <nexus/app.hh>

#include <fstream>
#include <string>
#include <vector>

namespace
{
[[maybe_unused]] std::string imul()
{
    return "inline u64 imul(u64 lhs, u64 rhs, u64* high)\n"
           "{\n"
           "#ifdef _MSC_VER\n"
           "    return _mul128(lhs, rhs, &(i64(high)));\n"
           "#else\n"
           "    __int128 res_i = __int128(i64(lhs)) * i64(rhs);\n"
           "    u64 v[2] = {};\n"
           "    memcpy(&v, &res_i, sizeof(__int128));\n"
           "    *high = v[1];\n"
           "    return v[0];\n"
           "#endif\n"
           "}\n";
}

[[maybe_unused]] std::string mul()
{
    return "inline u64 mul(u64 lhs, u64 rhs, u64* high)\n"
           "{\n"
           "    return TG_MUL_U128(lhs, rhs, high);\n"
           "}\n";
}

std::string generate_mul(int w_r, int w_a, int w_b)
{
    auto const out_type = "u" + std::to_string(w_r * 64);
    auto const a_type = "u" + std::to_string(w_a * 64);
    auto const b_type = "u" + std::to_string(w_b * 64);

    std::vector<std::pair<int, int>> vars_l;
    std::vector<std::pair<int, int>> vars_h;
    std::vector<std::pair<int, int>> mul_intrin;
    std::vector<std::pair<int, int>> mul;
    std::vector<std::vector<std::string>> summations;
    summations.resize(size_t(w_r));

    auto const l_of = [](std::pair<int, int> const& pair) { return "l" + std::to_string(pair.first) + std::to_string(pair.second); };
    auto const h_of = [](std::pair<int, int> const& pair) { return "h" + std::to_string(pair.first) + std::to_string(pair.second); };
    auto const lhs_of = [&](int v) {
        if (w_a == 1)
            return std::string("lhs");
        else
            return "lhs.d[" + std::to_string(v) + "]";
    };
    auto const rhs_of = [&](int v) {
        if (w_b == 1)
            return std::string("rhs");
        else
            return "rhs.d[" + std::to_string(v) + "]";
    };

    for (auto i = 0; i < w_a; ++i)
    {
        for (auto j = 0; j < w_b; ++j)
        {
            auto const pos_l = i + j;
            auto const pos_h = pos_l + 1;

            if (pos_h < w_r)
            {
                vars_h.push_back({i, j});
                vars_l.push_back({i, j});
                mul_intrin.push_back({i, j});
                summations[size_t(pos_l)].push_back(l_of({i, j}));
                summations[size_t(pos_h)].push_back(h_of({i, j}));
            }
            else if (pos_l < w_r)
            {
                vars_l.push_back({i, j});
                mul.push_back({i, j});
                summations[size_t(pos_l)].push_back(l_of({i, j}));
            }
        }
    }

    std::string result;
    result += "template <>\n";
    result += "inline " + out_type + " mul(" + a_type + " lhs, " + b_type + " rhs)\n{\n";
    result += "    " + out_type + " res;\n";

    for (auto i = 0u; i < vars_l.size(); ++i)
        result += "    u64 " + l_of(vars_l[i]) + " = 0;\n";
    for (auto i = 0u; i < vars_h.size(); ++i)
        result += "    u64 " + h_of(vars_h[i]) + " = 0;\n";

    for (auto const& m : mul_intrin)
    {
        result += "    " + l_of(m) + " = TG_MUL_U128(" + lhs_of(m.first) + ", " + rhs_of(m.second) + ", &" + h_of(m) + ");\n";
    }
    for (auto const& m : mul)
    {
        result += "    " + l_of(m) + " = " + lhs_of(m.first) + " * " + rhs_of(m.second) + ";\n";
    }

    result += "    unsigned char c = 0;\n";
    for (auto i = 0; i < w_r - 1; ++i)
    {
        if (i != 0)
        {
            result += "    c += _addcarry_u64(0, res.d[" + std::to_string(i) + "], c, &res.d[" + std::to_string(i) + "]);\n";
            result += "    c = 0;\n";
        }

        for (auto const& s : summations[size_t(i)])
        {
            result += "    c += _addcarry_u64(0, res.d[" + std::to_string(i) + "], " + s + ", &res.d[" + std::to_string(i) + "]);\n";
        }
    }
    result += "    res.d[" + std::to_string(w_r - 1) + "] = c";
    for (auto i = 0u; i < summations[size_t(w_r - 1)].size(); ++i)
    {
        auto const& s = summations[size_t(w_r - 1)][i];
        result += " + ";
        result += s;
    }
    result += ";\n";


    result += "    return res;\n";
    result += "}\n";
    return result;
}

/// special case
std::string imul_128_64_128()
{
    return "template <>\n"
           "inline i128 imul(i64 lhs, i128 rhs)\n"
           "{\n"
           "#ifdef _MSC_VER\n"
           "    return imul<2>(i128(lhs), rhs);\n"
           "#else\n"
           "    intrinsic_i128 l = lhs;\n"
           "    intrinsic_i128 r;\n"
           "    memcpy(&r, &rhs, sizeof(intrinsic_i128));\n"
           "    intrinsic_i128 inres = l * r;\n"
           "    i128 res;\n"
           "    memcpy(&res, &inres, sizeof(intrinsic_i128));\n"
           "    return res;\n"
           "#endif\n"
           "}\n";
}

std::string imul_128_128_64()
{
    return "template <>\n"
           "inline i128 imul(i128 lhs, i64 rhs)\n"
           "{\n"
           "#ifdef _MSC_VER\n"
           "    return imul<2>(lhs, i128(rhs));\n"
           "#else\n"
           "    intrinsic_i128 l;\n"
           "    intrinsic_i128 r = rhs;\n"
           "    memcpy(&l, &lhs, sizeof(intrinsic_i128));\n"
           "    intrinsic_i128 inres = l * r;\n"
           "    i128 res;\n"
           "    memcpy(&res, &inres, sizeof(intrinsic_i128));\n"
           "    return res;\n"
           "#endif\n"
           "}\n";
}

std::string imul_128_64_64()
{
    return "template <>\n"
           "inline i128 imul(i64 lhs, i64 rhs)\n"
           "{\n"
           "#ifdef _MSC_VER\n"
           "    return imul<2>(i128(lhs), i128(rhs));\n"
           "#else\n"
           "    intrinsic_i128 l = lhs;\n"
           "    intrinsic_i128 r = rhs;\n"
           "    intrinsic_i128 inres = l * r;\n"
           "    i128 res;\n"
           "    memcpy(&res, &inres, sizeof(intrinsic_i128));\n"
           "    return res;\n"
           "#endif\n"
           "}\n";
}

std::string generate_imul(int w_r, int w_a, int w_b)
{
    auto const out_type = "i" + std::to_string(w_r * 64);
    auto const a_type = "i" + std::to_string(w_a * 64);
    auto const b_type = "i" + std::to_string(w_b * 64);

    std::vector<std::pair<int, int>> vars_l;
    std::vector<std::pair<int, int>> vars_h;
    std::vector<std::pair<int, int>> mul_intrin;
    std::vector<std::pair<int, int>> mul;
    std::vector<std::vector<std::string>> summations;
    summations.resize(size_t(w_r));

    auto const l_of = [](std::pair<int, int> const& pair) { return "l" + std::to_string(pair.first) + std::to_string(pair.second); };
    auto const h_of = [](std::pair<int, int> const& pair) { return "h" + std::to_string(pair.first) + std::to_string(pair.second); };
    auto const lhs_of = [&](int v) {
        if (w_a == 1)
            return std::string("lhs");
        else
            return "lhs.d[" + std::to_string(v) + "]";
    };
    auto const rhs_of = [&](int v) {
        if (w_b == 1)
            return std::string("rhs");
        else
            return "rhs.d[" + std::to_string(v) + "]";
    };

    for (auto i = 0; i < w_a; ++i)
    {
        for (auto j = 0; j < w_b; ++j)
        {
            auto const pos_l = i + j;
            auto const pos_h = pos_l + 1;

            if (pos_h < w_r)
            {
                vars_h.push_back({i, j});
                vars_l.push_back({i, j});
                mul_intrin.push_back({i, j});
                summations[size_t(pos_l)].push_back(l_of({i, j}));
                summations[size_t(pos_h)].push_back(h_of({i, j}));
            }
            else if (pos_l < w_r)
            {
                vars_l.push_back({i, j});
                mul.push_back({i, j});
                summations[size_t(pos_l)].push_back(l_of({i, j}));
            }
        }
    }

    std::string result;

    auto const conditional_invert = [&](auto const& var_gen, std::string mask, int size) {
        result += "    { // conditional inversion\n";
        result += "        " + var_gen(0) + " = " + (size == 1 ? "i64" : "") + "((u64(" + var_gen(0) + ") ^ " + mask + ") - " + mask + ");\n";
        for (auto i = 1; i < size; ++i)
        {
            result += "        u64 c" + std::to_string(i - 1) + " = (" + var_gen(i - 1) + " == 0) & " + mask
                      + (i > 1 ? " & c" + std::to_string(i - 2) : "") + ";\n";
            result += "        " + var_gen(i) + " = (u64(" + var_gen(i) + ") ^ " + mask + ") + c" + std::to_string(i - 1) + ";\n";
        }
        result += "    }\n";
    };

    result += "template <>\n";
    result += "inline " + out_type + " imul(" + a_type + " lhs, " + b_type + " rhs)\n{\n";
    //    result += "fixed_int<" + std::to_string(w_r) + "> imul(" + a_type + (w_a > 1 ? " const&" : "") + " lhs, " + b_type + (w_b > 1 ? " const&" : "") + " rhs)\n{\n";
    result += "    " + out_type + " res;\n";

    // conditional inversion
    if (w_a != w_b || w_a != w_r)
    {
        result += "    u64 s_l = u64(i64(" + lhs_of(w_a - 1) + ") >> 63); // 0 iff > 0, -1 otherwise\n";
        result += "    u64 s_r = u64(i64(" + rhs_of(w_b - 1) + ") >> 63); // 0 iff > 0, -1 otherwise\n";
        result += "    u64 s_res = s_l ^ s_r;\n";
        conditional_invert(lhs_of, "s_l", w_a);
        conditional_invert(rhs_of, "s_r", w_b);
    }

    for (auto i = 0u; i < vars_l.size(); ++i)
        result += "    u64 " + l_of(vars_l[i]) + " = 0;\n";
    for (auto i = 0u; i < vars_h.size(); ++i)
        result += "    u64 " + h_of(vars_h[i]) + " = 0;\n";

    for (auto const& m : mul_intrin)
    {
        result += "    " + l_of(m) + " = TG_MUL_U128(u64(" + lhs_of(m.first) + "), u64(" + rhs_of(m.second) + "), &" + h_of(m) + ");\n";
    }
    for (auto const& m : mul)
    {
        result += "    " + l_of(m) + " = u64(" + lhs_of(m.first) + ") * u64(" + rhs_of(m.second) + ");\n";
    }

    result += "    unsigned char c = 0;\n";
    for (auto i = 0; i < w_r - 1; ++i)
    {
        if (i != 0)
        {
            result += "    c += _addcarry_u64(0, res.d[" + std::to_string(i) + "], c, &res.d[" + std::to_string(i) + "]);\n";
            result += "    c = 0;\n";
        }

        for (auto const& s : summations[size_t(i)])
        {
            result += "    c += _addcarry_u64(0, res.d[" + std::to_string(i) + "], " + s + ", &res.d[" + std::to_string(i) + "]);\n";
        }
    }
    result += "    res.d[" + std::to_string(w_r - 1) + "] = c";
    for (auto i = 0u; i < summations[size_t(w_r - 1)].size(); ++i)
    {
        auto const& s = summations[size_t(w_r - 1)][i];
        result += " + ";
        result += s;
    }
    result += ";\n";

    if (w_a != w_b || w_a != w_r)
        conditional_invert([](int i) { return "res.d[" + std::to_string(i) + "]"; }, "s_res", w_r);

    result += "    return res;\n";
    result += "}\n";
    return result;
}


void generate_mul_file()
{
    std::string filepath("fixed_uint_gen.hh");
    std::ofstream file(filepath);

    file << "#pragma once\n\n";

    file << "// This file was generated by generate_fixed_uint_multiplications.cc in TGSamples.\n\n";

    file << "#ifdef _MSC_VER\n";
    file << "#include <intrin.h>\n";
    file << "#else\n";
    file << "#include <x86intrin.h>\n";
    file << "#endif\n\n";

    file << "#include <typed-geometry/feature/fixed_int.hh>\n\n";
    file << "namespace tg::detail\n{\n";
    for (auto r = 2; r <= 4; ++r)
        for (auto j = 1; j <= r; ++j)
            for (auto i = 1; i <= r; ++i)
                if (i + j >= r)
                {
                    file << generate_mul(r, i, j);
                    file << "\n";
                }


    file << "} // namespace tg::detail";
}

void generate_imul_file()
{
    std::string filepath("fixed_int_gen.hh");
    std::ofstream file(filepath);

    file << "#pragma once\n\n";

    file << "// This file was generated by generate_fixed_uint_multiplications.cc in TGSamples.\n\n";

    file << "#ifdef _MSC_VER\n";
    file << "#include <intrin.h>\n";
    file << "#else\n";
    file << "#include <x86intrin.h>\n";
    file << "#include <cstring>\n";
    file << "#endif\n\n";

    file << "#include <typed-geometry/feature/fixed_int.hh>\n\n";
    file << "namespace tg::detail\n{\n";

    // gcc / msvc special case
    file << "/// GCC warns that __int128 is not iso-c++\n"
            "#ifndef _MSC_VER // MSVC does not support __int128\n"
            "#pragma GCC diagnostic push\n"
            "#pragma GCC diagnostic ignored \"-Wpedantic\"\n"
            "using intrinsic_i128 = __int128;\n"
            "#pragma GCC diagnostic pop\n"
            "#endif\n\n";

    // special cases
    // msvc requires this one to come first
    file << generate_imul(2, 2, 2) << "\n";
    file << imul_128_64_64() << "\n";
    file << imul_128_128_64() << "\n";
    file << imul_128_64_128() << "\n";

    for (auto r = 3; r <= 4; ++r)
        for (auto j = 1; j <= r; ++j)
            for (auto i = 1; i <= r; ++i)
                if (i + j >= r)
                {
                    file << generate_imul(r, i, j);
                    file << "\n";
                }

    file << "} // namespace tg::detail";
}

} // namespace

APP("tg generate multiplications")
{
    generate_mul_file();
    generate_imul_file();
}
