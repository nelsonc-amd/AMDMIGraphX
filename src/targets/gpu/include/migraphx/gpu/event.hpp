#ifndef MIGRAPHX_GUARD_MIGRAPHLIB_EVENT_HPP
#define MIGRAPHX_GUARD_MIGRAPHLIB_EVENT_HPP

//#include <migraphx/operators.hpp>
//#include <migraphx/config.hpp>
// #include <utility>

namespace migraphx {
inline namespace MIGRAPHX_INLINE_NS {
namespace gpu {

struct record_event
{
    int event = -1;
    std::string name() const { return "gpu::record_event"; }
    shape compute_shape(const std::vector<shape>& inputs) const
    {
        if(inputs.empty())
            return {};
        else
            return inputs.front();
    }
    
    argument compute(context& ctx, const shape&, const std::vector<argument>&)     {
        if (event == -1) {
            event = ctx.create_event();
        } else {
            ctx.record_event(event);
        }
        migraphx::shape s{migraphx::shape::int32_type, {1}};
        std::vector<int> data = {event};
        return migraphx::literal{s, data}.get_argument();
    }
};

struct wait_event
{
    std::string name() const { return "gpu::wait_event"; }
    shape compute_shape(const std::vector<shape>& inputs) const
    {
        if(inputs.empty())
            return {};
        else
            return inputs.front();
    }
    
    argument compute(context& ctx, const shape&, const std::vector<argument>& args)
    {
        for (auto arg : args)
        {
            ctx.wait_event(*(reinterpret_cast<int*> (arg.data())));
        }
        return {};
    }
};

struct set_stream
{
    int stream = -1;
    template <class Self, class F>
    static auto reflect(Self& self, F f)
    {
        return pack(f(self.stream, "stream"));
    }
    std::string name() const { return "gpu::set_stream"; }
    shape compute_shape(const std::vector<shape>& inputs) const
    {
        if(inputs.empty())
            return {};
        else
            return inputs.front();
    }
    
    argument compute(context& ctx, const shape&, const std::vector<argument>&) const
    {
        ctx.set_stream(stream);
        return {};
    }
    void finalize(context& ctx, const shape&, std::vector<shape>)
    {
        ctx.set_stream(stream);
    }
};    

} // namespace gpu
} // namespace MIGRAPHX_INLINE_NS
} // namespace migraphx

#endif