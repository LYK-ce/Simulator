#include "Connect_Port.hh"  
#include "Connect_Link.hh"  
#include <algorithm>  
#include <iostream>   

namespace AstraSim {  

  // 构造函数实现
  ConnectPort::ConnectPort(std::string id,        // 端口ID
                       ConnectPortKind kind,     // 端口类型
                       double speed_Gbps,       // 端口速率
                       double base_latency_us,  // 端口延迟
                       Device* owner) {         // 所属设备
  this->id = id;                                
  this->kind = kind;                            
  this->speed_Gbps = speed_Gbps;                
  this->base_latency_us = base_latency_us;      
  this->owner = owner;                          
  // 输出调试信息
  std::cout << "ConnectPort " << this->id << " created." << std::endl;
}

// 链路管理方法
void ConnectPort::attach_link(ConnectLink* link) {  // 附加链路到端口
  if (link == nullptr) {                                  // 若链路为空，则直接返回
    return;                                       
  }
  auto it = std::find(links.begin(), links.end(), link);  // 查找是否已存在，如果未找到，将链路加入列表
  if (it == links.end()) {                                 
    links.push_back(link);                                 
  }
}

// 连通性查询
bool ConnectPort::connects_to_device(int device_id) const {  // 判断是否连向指定设备
  if (owner != nullptr && owner->id == device_id) {         // 若自身设备即目标，返回true
    return true;                                            
  }
  for (auto* link : links) {                                // 遍历所有链路
    ConnectPort* peer = link->other_end(this);               // 查找对端端口
    if (peer != nullptr && peer->owner != nullptr &&        // 确保对端存在
        peer->owner->id == device_id) {                     // 对端设备匹配目标
      return true;                                          // 返回true
    }
  }
  return false;                                             // 未找到则返回false
}

// 辅助函数：将端口类型转字符串
std::string ConnectPortKindToString(ConnectPortKind kind) {  // 
  switch (kind) {                                          
    case ConnectPortKind::PCIe:                             
      return "PCIe";                                       
    case ConnectPortKind::NVLink:                           
      return "NVLink";                                     
    case ConnectPortKind::IB:                              
      return "IB";                                        
    default:                                               
      return "Unknown";                                    
  }
}

} // namespace AstraSim
