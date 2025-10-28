#include "NCCL_Event.hh"
#include "astra-sim/system/DataSet.hh"
#include <iostream>

namespace AstraSim {

NCCL_Event::NCCL_Event(const std::string& name)
    : Event(name)
{
    // 链表字段 next 由基类默认构造初始化为 nullptr
}

void NCCL_Event::process(Sys* generator, Workload_StateMachine* workload)
{
    std::cout << "[NCCL_Event] "
              << name
              << "  src=" << src_rank
              << "  dst=" << dst_rank
              << "  bytes=" << bytes
             << std::endl;

    // 这里写 NCCL 具体语义：schedule send/recv、reduce 等
    // 示例：直接注册一个等待事件，保持接口不变
    
}

} // namespace AstraSim