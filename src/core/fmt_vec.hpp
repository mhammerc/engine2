#include <fmt/format.h>

#include <glm/glm.hpp>

template<>
struct fmt::formatter<glm::vec2>: formatter<float> {
    template<typename formatcontext>
    auto format(glm::vec2 v, formatcontext& ctx) const {
        // will print (x, y)

        fmt::format_to(ctx.out(), "(");
        formatter<float>::format(v.x, ctx);
        fmt::format_to(ctx.out(), ",");
        formatter<float>::format(v.y, ctx);
        fmt::format_to(ctx.out(), ")");

        return ctx.out();
    }
};

template<>
struct fmt::formatter<glm::vec<2, int>>: formatter<int> {
    template<typename formatcontext>
    auto format(glm::vec2 v, formatcontext& ctx) const {
        // will print (x, y)

        fmt::format_to(ctx.out(), "(");
        formatter<int>::format(v.x, ctx);
        fmt::format_to(ctx.out(), ",");
        formatter<int>::format(v.y, ctx);
        fmt::format_to(ctx.out(), ")");

        return ctx.out();
    }
};

template<>
struct fmt::formatter<glm::vec3>: formatter<float> {
    template<typename formatcontext>
    auto format(glm::vec3 v, formatcontext& ctx) const {
        // will print (x, y, z)

        fmt::format_to(ctx.out(), "(");
        formatter<float>::format(v.x, ctx);
        fmt::format_to(ctx.out(), ",");
        formatter<float>::format(v.y, ctx);
        fmt::format_to(ctx.out(), ",");
        formatter<float>::format(v.z, ctx);
        fmt::format_to(ctx.out(), ")");

        return ctx.out();
    }
};

template<>
struct fmt::formatter<glm::vec4>: formatter<float> {
    template<typename FormatContext>
    auto format(glm::vec4 v, FormatContext& ctx) const {
        // Will print (x, y, z, w)

        fmt::format_to(ctx.out(), "(");
        formatter<float>::format(v.x, ctx);
        fmt::format_to(ctx.out(), ",");
        formatter<float>::format(v.y, ctx);
        fmt::format_to(ctx.out(), ",");
        formatter<float>::format(v.z, ctx);
        fmt::format_to(ctx.out(), ",");
        formatter<float>::format(v.w, ctx);
        fmt::format_to(ctx.out(), ")");

        return ctx.out();
    }
};