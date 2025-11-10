#ifndef __Connect_Port_HH__  
#define __Connect_Port_HH__  

#include "Sys.hh"  
#include <string>  
#include <vector>  

namespace AstraSim {  

class ConnectLink;  

enum class ConnectPortKind {  // 端口类型枚举
  PCIe,   
  NVLink, 
  IB      
};  

class ConnectPort {  // 设备端口类
 public: 
  // 构造函数
  ConnectPort(std::string id,            // 端口唯一标识
             ConnectPortKind kind,       // 端口类型
             double speed_Gbps,         // 端口速率（Gbps）
             double base_latency_us,    // 基础延迟（us）
             Device* owner);            // 所属硬件设备
  
  // 成员函数
  void attach_link(ConnectLink* link);             // 将链路对象登记到端口
  bool connects_to_device(int device_id) const;   // 判断端口是否连向指定设备
  
  // 公共属性
  std::string id;                 // 端口ID
  ConnectPortKind kind;            // 端口类型
  double speed_Gbps;              // 端口速率
  double base_latency_us;         // 基础延迟
  Device* owner;                  // 所属设备
  std::vector<ConnectLink*> links; // 关联链路集合
};

std::string DevicePortKindToString(ConnectPortKind kind);  // 将端口类型枚举转换为字符串

} // namespace AstraSim

#endif
