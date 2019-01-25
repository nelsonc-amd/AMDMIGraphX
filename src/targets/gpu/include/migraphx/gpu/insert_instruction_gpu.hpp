#ifndef MIGRAPHX_GUARD_RTGLIB_INSERT_INSTRUCTION_GPU_HPP
#define MIGRAPHX_GUARD_RTGLIB_INSERT_INSTRUCTION_GPU_HPP

#include <migraphx/gpu/event.hpp>

namespace migraphx {
namespace gpu {

struct insert_instruction_gpu
{
    void insert_record_event(program* p,
                             instruction_ref ins,
                             int event)
    {
        p->insert_instruction(ins, record_event{event});
    }
    void insert_wait_event(program* p,
                            instruction_ref ins,
                            std::vector<instruction_ref> args)
    {
        p->insert_instruction(ins, wait_event{}, args);
    }
    void insert_stream(program* p,
                       instruction_ref ins,
                       int stream)
    {
        
        p->insert_instruction(ins, set_stream{stream});
    }
    
};

} // namespace gpu

} // namespace migraphx

#endif