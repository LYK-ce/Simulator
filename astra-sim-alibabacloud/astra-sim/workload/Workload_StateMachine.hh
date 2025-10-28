/*
    Presented by LiYongKang
    2024.10.20
    This file is the state machine inherited from Workload class.
    Now we will use the state machine to replace the original implementation of workload iteration functions.
*/

#ifndef WORKLOAD_STATEMACHINE_HH
#define WORKLOAD_STATEMACHINE_HH
#include "Workload.hh"
#include <functional>
#include <unordered_map>
#include <vector>
#include "Event_Queue.hh"
namespace AstraSim 
{
    enum State 
    {
        Forward_Pass,
        Weight_Gradient,
        Input_Gradient,
        Wait_For_Sim_Finish,
        Forward_In_BackPass
    };

    //泛型有限状态机FSM模板类，名字叫StateMachineT，不依赖具体状态/事件类型，可以复用做任何状态机
    template <typename State, typename Event>
    class StateMachineT {
    public:
    //内部结构体
    //从哪个状态+什么事件 ————————转移到哪个状态，执行什么动作
    struct Transition {
        State from;                 //源状态
        Event event;                //触发事件
        State to;                   //目标状态
        std::function<void()> onEnter;  //进入目标状态时的动作（可选）
    };
    //构造函数，给定初始状态，转移规则，狗制造一个状态机
    StateMachineT(State initial,
                    std::vector<Transition> table)
        : cur_(initial) {
        //使用unordered map把from event ————Transition存成哈希表，后续feed可以常数时间进行查表。
        for (auto& t : table) map_[{t.from, t.event}] = t;
    }
    //当前状态
    State current() const { return cur_; }
    //状态机发动机
    bool feed(Event e) {
        auto it = map_.find({cur_, e});     //查表：当前状态，+ 收到的事件
        if (it == map_.end()) return false; //没有这条转移规则，拒绝
        const Transition& t = it->second;   //it->first 是 key，it->second 是 Transition，也就是值
        cur_ = t.to;                        //进行状态变更
        if (t.onEnter) t.onEnter();         //如果存在对应动作，那就执行
        return true;
    }
    private:
    State cur_;                             //当前状态
    //让pair<State, Event>可以作为unordered_map的key，给哈希表提供一个取哈希值的办法
    //现在的键是<State, Event>对，标准库没有对应的哈希函数，因此自行实现一个哈希函数PairHash
    struct PairHash {
        template <class A, class B>
        std::size_t operator()(const std::pair<A, B>& p) const {
        return std::hash<A>{}(p.first) ^ (std::hash<B>{}(p.second) << 1);
        }
    };
    //构建出来的哈希表。
    std::unordered_map<std::pair<State, Event>, Transition, PairHash> map_;
    };

    class Workload_StateMachine : public Workload {
    public:
    Workload_StateMachine(std::string run_name,
                Sys* generator,
                std::string name,
                int TOTAL_PASS,
                int total_rows,
                int stat_row,
                std::string path,
                bool seprate_log);
    void call(EventType event, CallData* data) override;
    // 把基类入口 hijack 到状态机
    // void fire() override;

    Event_Queue * event_queue;
    private:
    // 状态机实例
    StateMachineT<State, EventType> sm_;
    
    // 生成状态表
    static std::vector<typename StateMachineT<State, EventType>::Transition>
    buildTable(Workload_StateMachine* self);

    // 以下 5 个是真正的「工作状态」
    // void enterFwd();
    // void enterBwdWeight();
    // void enterBwdInput();
    // void enterUpdate();
    // void enterWait();
    };


}



#endif