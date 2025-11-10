#ifndef NCCL_EVENT_HH
#define NCCL_EVENT_HH

#include "Event.hh"

namespace AstraSim {

class NCCL_Event : public Event {
public:
    // 额外携带 NCCL 需要的参数，这里先留最常用三个
    int     src_rank;
    int     dst_rank;
    uint64_t bytes;

    NCCL_Event(const std::string& name);

    // 覆盖基类默认实现
    void process(Device* generator, Workload_StateMachine* workload) override;
};

} // namespace AstraSim
#endif