#ifndef EVENT_QUEUE_HH
#define EVENT_QUEUE_HH

#include "Event.hh"

namespace AstraSim {

class Event_Queue {
public:
    Event_Queue();
    ~Event_Queue();                 // 释放剩余事件
    void append(Event* e);         // 尾部追加
    bool insert(Event* e, unsigned pos); // 指定位置插入
    Event* getNext();              // 弹出队首

    bool empty() const { return head == nullptr; }
    unsigned size() const { return count; }

private:
    Event* head;
    Event* tail;   // 尾指针，append O(1)
    unsigned count;
};

} // namespace AstraSim
#endif