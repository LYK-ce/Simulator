#ifndef __GPU_DEVICE_HH__
#define __GPU_DEVICE_HH__

#include "Sys.hh"

namespace AstraSim {

class GPUDevice : public Device {
 public:
  // 构造函数：接收基类所有参数，并添加自己的参数（可选）
  GPUDevice(AstraNetworkAPI* NI, 
            AstraMemoryAPI* MEM,
            int id,
            int npu_offset,
            int num_passes,
            std::vector<int> physical_dims,
            std::vector<int> queues_per_dim,
            std::string my_sys,
            std::string my_workload,
            float comm_scale,
            float compute_scale,
            float injection_scale,
            int total_stat_rows,
            int stat_row,
            std::string path,
            std::string run_name,
            bool seprate_log,
            bool rendezvous_enabled,
            GPUType _gpu_type,
            std::vector<int> _all_gpus,
            std::vector<int> _NVSwitchs,
            int _ngpus_per_node,
            // 新增参数（可选）
            unsigned long long vram_bytes,
            double tflops,
            int nvlink_lanes);

  // 添加你需要的成员变量
  // 建议：public 用于简单数据，private 用 getter/setter 访问
  
  // 新增GPU的独有属性，其余全部继承自device
  unsigned long long vram_bytes; // 显存大小    
  double            tflops;        // 计算性能
  int               nvlink_lanes;         // NVLink通道
};

} // namespace AstraSim
#endif