#ifndef MIGRAPHX_GUARD_RTGLIB_MEMORY_COLORING2_HPP
#define MIGRAPHX_GUARD_RTGLIB_MEMORY_COLORING2_HPP

#include <string>
#include <migraphx/config.hpp>
#include <migraphx/instruction_ref.hpp>

namespace migraphx {
inline namespace MIGRAPH_INLINE_NS {
struct program;

struct memory_coloring2
{
    std::string allocation_op{};
    std::string name() const { return "memory_coloring2"; }
    void apply(program& p) const;
};

} // namespace MIGRAPH_INLINE_NS
} // namespace migraphx

#endif