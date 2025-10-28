/*
    Presented by LiYongKang
    2024.10.13
    This file is the Event type base class, all other events are supposed to be inherited from this class.
    New event types must confined to this implementatinon.  
*/

#include "Event.hh" 
#include "Workload_StateMachine.hh" 
#include "Layer.hh"

namespace AstraSim {
    
    Event::Event(const std::string& name)
        {
            this->name = name;
        }

    Event::~Event() {}

    // 默认的Iterate函数实现，自定义的事件应该覆盖此函数
    // 目前默认行为将随机生成一个数值，然后将事件插入到事件列表当中。
    void AstraSim ::Event:: process(Sys* generator, Workload_StateMachine* workload)
    {
        //
        std::cout << "Warning: Default process() called for event [" << name << "]"
              << " - a specific implementation is expected!" << std::endl;
        // 默认行为将随机生成一个数值，然后将其事件插入到事件列表当中。
        //int random_tick = std::rand() * 100;
        //todo 
        AstraSim::Tick tickValue = 42;
        printf("hello workload event call function! \n");
        generator->try_register_event(
            workload, EventType::Workload_Wait, NULL, tickValue);

    }

}