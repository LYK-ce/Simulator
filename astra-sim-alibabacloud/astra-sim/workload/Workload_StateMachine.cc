
#include "Workload_StateMachine.hh"
#include "CSVWriter.hh"
#include "Layer.hh"
#include "astra-sim/system/MockNcclLog.h"
#include "Event.hh"
#include "Event_Queue.hh"

namespace AstraSim {

    Workload_StateMachine::Workload_StateMachine(
    std::string run_name,
    Sys* generator,
    std::string name,
    int TOTAL_PASS,
    int total_rows,
    int stat_row,
    std::string path,
    bool seprate_log)
    //先调用基类的构造函数，把基类部分搭建好
    : Workload(run_name, generator, name, TOTAL_PASS,
               total_rows, stat_row, path, seprate_log),
    //构造成员变量状态机，就是之前的泛型状态机，然后将初始状态设置为Forward_Pass，转移表通过buildTable函数生成
        sm_(State::Forward_Pass, buildTable(this)) ,
        event_queue(new Event_Queue()){}
    
    //重写call方法
    void Workload_StateMachine::call(EventType event, CallData* data) 
    {
        printf("workload_StateMachine call!\n");
        // AstraSim::Event myevent("MyEvent", 10);
        // myevent.Iterate();
        
        Event* ev = this->event_queue->getNext();
        if (ev != nullptr) {
            ev->process(generator,this);
        } else {
            printf("event queue is empty!\n");
        }

        // assert(index >= 0);
        // assert(index < SIZE);
        // check_for_sim_end();
        // switch (current_state)
        // {
        //     case LoopState::Forward_Pass:
        //         if (!layers[index]->is_weight_grad_comm_finished_blocking()) {
        //         return;
        //         }
        //         if (delay_loaded == false) {
        //         counter = layers[index]->get_fwd_pass_compute();
        //         delay_loaded = true;
        //         }
        //         if (counter > 0) {
        //         generator->try_register_event(
        //             this, EventType::Workload_Wait, NULL, counter);
        //         return;
        //         }
        //         if (!collective_issued) {
        //         collective_issued = true;
        //         if(layers[index]->fwd_pass_comm_size < 4096 && layers[index]->fwd_pass_comm_size >0){
        //             layers[index]->fwd_pass_comm_size = 4096;
        //         }
        //         layers[index]->issue_forward_pass_comm(
        //             SchedulingPolicy::None, CollectiveBarrier::Blocking);
        //         return;
        //         }
        //         index++;
        //         delay_loaded = false;
        //         collective_issued = false;
        //         if (index >= SIZE) {
        //         current_state = LoopState::Input_Gradient;
        //         index--;
        //         }
                
        //         generator->register_event(this, EventType::General, NULL, 1);
        //         return;
        //         break;
        //     case LoopState::Weight_Gradient:
        //         if (delay_loaded == false) {
        //         counter = layers[index]->get_weight_grad_compute();
        //         delay_loaded = true;
        //         }
        //         if (counter > 0) {
        //         generator->try_register_event(
        //             this, EventType::Workload_Wait, NULL, counter);
        //         return;
        //         }
        //         if (!collective_issued) {
        //         collective_issued = true;
        //         layers[index]->issue_weight_grad_comm(
        //             SchedulingPolicy::FIFO, CollectiveBarrier::Non_Blocking);
        //         }
        //         if (!layers[index]->is_input_grad_comm_finished_blocking()) {
        //         return;
        //         }
        //         collective_issued = false;
        //         delay_loaded = false;
        //         if (index >= 0) {
        //         index--;
        //         }
        //         if (index == -1) {
        //         index = 0;
        //         if (generator->id == 0) {
        //             std::cout << "pass: " << pass_counter
        //                     << " finished at time: " << Sys::boostedTick() << std::endl;
        //         }
        //         pass_counter++;
        //         current_state = LoopState::Forward_Pass;
        //         } else {
        //         current_state = LoopState::Input_Gradient;
        //         }
        //         generator->register_event(this, EventType::General, NULL, 1);
        //         return;
        //         break;
        //     case LoopState::Input_Gradient:
        //         if (layers[index]->needs_fwd_in_bckwd_initiation && !checkpoint_initiated) {
        //         int tmp = index;
        //         while (!layers[index--]->is_checkpoint)
        //             ;
        //         index++;
        //         current_state = LoopState::Forward_In_BackPass;
        //         checkpoint_initiated = true;
        //         generator->register_event(this, EventType::General, NULL, 1);
        //         if (generator->id == 0) {
        //             std::cout << "***** info, initiating fwd_in_bkwd starting from layer:"
        //                     << index << " to layer: " << tmp
        //                     << " ,at time: " << Sys::boostedTick() << std::endl;
        //         }
        //         return;
        //         }
        //         if (delay_loaded == false) {
        //         counter = layers[index]->get_input_grad_compute();
        //         delay_loaded = true;
        //         }
        //         if (counter > 0) {
        //         generator->try_register_event(
        //             this, EventType::Workload_Wait, NULL, counter);
        //         return;
        //         }
        //         if (!collective_issued) {
        //         collective_issued = true;
        //         layers[index]->issue_input_grad_comm(
        //             SchedulingPolicy::LIFO, CollectiveBarrier::Blocking);
        //         return;
        //         }
        //         checkpoint_initiated = false;
        //         collective_issued = false;
        //         delay_loaded = false;
        //         current_state = LoopState::Weight_Gradient;
        //         generator->register_event(this, EventType::General, NULL, 1);
        //         return;
        //     case LoopState::Forward_In_BackPass:
        //         if (!layers[index]->is_weight_grad_comm_finished_blocking()) {
        //         return;
        //         }
        //         if (delay_loaded == false) {
        //         counter = layers[index]->get_fwd_pass_compute();
        //         delay_loaded = true;
        //         }
        //         if (counter > 0) {
        //         generator->try_register_event(
        //             this, EventType::Workload_Wait, NULL, counter);
        //         return;
        //         }
        //         if (!collective_issued) {
        //         collective_issued = true;
        //         layers[index]->issue_forward_pass_comm(
        //             SchedulingPolicy::None, CollectiveBarrier::Blocking);
        //         return;
        //         }
        //         index++;
        //         delay_loaded = false;
        //         collective_issued = false;
        //         if (layers[index]->needs_fwd_in_bckwd_initiation) {
        //         current_state = LoopState::Input_Gradient;
        //         }
        //         generator->register_event(this, EventType::General, NULL, 1);
        //         return;

        //         break;
        //     default:
        //         cout<<"current state"<<static_cast<int>(current_state)<<" is invalid!"<<endl;
        // }
    }

    // 基类每调度一次就喂状态机
    // void Workload_StateMachine::fire() {
    //     printf("Workload_StateMachine::fire() called\n");
    //     return;
    // }

    // ------------------ 状态表 ------------------
    //buildTable函数负责生成一个状态转移表
    auto Workload_StateMachine::buildTable(Workload_StateMachine* self)
        -> std::vector<typename StateMachineT<State, EventType>::Transition> {
    using Trans = typename StateMachineT<State, EventType>::Transition;
    return {
        // {State::Forward_Pass,  EventType::General,  State::Forward_Pass,
        // [self](){ self->enterBwdWeight(); }},
        // {State::Weight_Gradient,EventType::General,State::Forward_Pass,
        // [self](){ self->enterBwdInput(); }},
        // {State::Input_Gradient, EventType::General,State::Forward_Pass,
        // [self](){ self->enterUpdate(); }},
        // {State::Wait_For_Sim_Finish,   EventType::General,State::Forward_Pass,
        // [self](){ self->enterWait(); }},
        // {State::Forward_In_BackPass,     EventType::General,State::Forward_Pass,
        // [self](){ self->enterFwd(); }},
    };
    }

} // namespace AstraSim