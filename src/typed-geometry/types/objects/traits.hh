#pragma once

namespace tg
{
/// the full object is meant (i.e. sphere with insides)
struct default_object_tag
{
    static constexpr char const* suffix = "";
};

/// only the object boundary is meant (i.e. only sphere surface)
struct boundary_tag
{
    static constexpr char const* suffix = "_boundary";
};

/// same as boundary_t but without caps (i.e. only cylinder mantle without caps)
struct boundary_no_caps_tag
{
    static constexpr char const* suffix = "_boundary_no_caps";
};
}
