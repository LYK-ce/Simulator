#include "Connect_Link.hh"  
#include <iostream>        

namespace AstraSim {

// 构造函数实现
ConnectLink::ConnectLink(std::string id,         // 链路ID
                       ConnectPort* first,      // 端点A
                       ConnectPort* second,     // 端点B
                       double bw_Gbps,         // 带宽
                       double latency_us,      // 延迟
                       int mtu,                // MTU
                       double error_rate) {    // 误码率
  this->id = id;                               
  this->first = first;                         
  this->second = second;                       
  this->bw_Gbps = bw_Gbps;                     
  this->latency_us = latency_us;               
  this->mtu = mtu;                             
  this->error_rate = error_rate;               
  if (this->first != nullptr) {                // 若端点A存在，将链路附加到端点A
    this->first->attach_link(this);            
  }
  if (this->second != nullptr) {               // 若端点B存在，将链路附加到端点B
    this->second->attach_link(this);           
  }
  std::cout << "ConnectLink " << this->id << " created." << std::endl;  // 打印创建日志
}

// 成员函数实现
ConnectPort* ConnectLink::other_end(const ConnectPort* port) const {  // 获取另一端端口
  if (port == first) {                                             
    return second;                                                 
  }
  if (port == second) {                                            
    return first;                                                  
  }
  return nullptr;                                                  
}

} // namespace AstraSim
