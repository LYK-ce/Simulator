/*
    Presented by LiYongKang
    2024.10.13
    This file is the Event type base class, all other events are supposed to be inherited from this class.
    New event types must confined to this implementatinon.  
*/

#ifndef EVENT_HH
#define EVENT_HH     
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include "astra-sim/system/Callable.hh"
#include "astra-sim/system/Sys.hh"
#include "Event.hh"

namespace AstraSim {
    class Event 
    {
        public:
        //属性
        std::string name; //事件名称
        Event * next;

        //函数
        Event(const std::string& name);

        // 虚析构函数，确保派生类的析构函数被正确调用
        virtual ~Event();
        virtual void process(Device* generator, Workload_StateMachine* workload) ;

  
    };    

}

#endif
