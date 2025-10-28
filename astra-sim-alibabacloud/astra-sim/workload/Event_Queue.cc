#include "Event_Queue.hh"

namespace AstraSim {

Event_Queue::Event_Queue() : head(nullptr), tail(nullptr), count(0) {}

Event_Queue::~Event_Queue() {
    while (Event* e = getNext()) {
        delete e;          // 假设事件所有权归队列
    }
}

void Event_Queue::append(Event* e) {
    e->next = nullptr;     // 断链，防止用户传进来还带尾巴
    if (!head) {
        head = tail = e;
    } else {
        tail->next = e;
        tail = e;
    }
    ++count;
}

bool Event_Queue::insert(Event* e, unsigned pos) {
    if (pos > count) return false;   // 允许插在尾部，pos==count等价于append
    e->next = nullptr;

    if (pos == 0) {                  // 插头部
        e->next = head;
        head = e;
        if (!tail) tail = e;
    } else if (pos == count) {       // 插尾部
        append(e);
        return true;
    } else {                         // 中间插入
        Event* cur = head;
        for (unsigned i = 0; i < pos - 1; ++i) cur = cur->next;
        e->next = cur->next;
        cur->next = e;
    }
    ++count;
    return true;
}

Event* Event_Queue::getNext() {
    if (!head) return nullptr;
    Event* e = head;
    head = head->next;
    if (head == nullptr) tail = nullptr;
    e->next = nullptr;   // 弹出的节点断链
    --count;
    return e;
}

} // namespace AstraSim