#ifndef __Connect_Link_HH__  
#define __Connect_Link_HH__  

#include "Connect_Port.hh"  
#include <string>          

namespace AstraSim {

class ConnectLink {  // 定义链路类
 public:
  // 构造函数 
  ConnectLink(std::string id,           // 链路唯一ID
             ConnectPort* first,        // 端点A
             ConnectPort* second,       // 端点B
             double bw_Gbps,           // 带宽（Gbps）
             double latency_us,        // 延迟（微秒）
             int mtu = 0,              // MTU，可选
             double error_rate = 0.0); // 误码率，可选
  
  // 成员函数
  ConnectPort* other_end(const ConnectPort* port) const; // 根据输入端口返回另一端
  
  // 公共属性
  std::string id;      
  ConnectPort* first;   
  ConnectPort* second;  
  double bw_Gbps;      
  double latency_us;   
  int mtu;             
  double error_rate;   
};  

} // namespace AstraSim

#endif  
