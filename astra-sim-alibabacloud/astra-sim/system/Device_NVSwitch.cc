#include "Device_NVSwitch.hh"
#include <iostream>

namespace AstraSim {

// 构造函数：初始化基类 + 初始化自己的成员
NVSwitchDevice::NVSwitchDevice(AstraNetworkAPI* NI, 
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
                     // 新增参数
                    int radix,
                    double crossbar_bw_GBps)
  // 调用基类构造函数，通过初始化列表传递所有基类需要的参数
  : Device(NI, MEM, id, npu_offset, num_passes,
           physical_dims, queues_per_dim, my_sys, my_workload,
           comm_scale, compute_scale, injection_scale,
           total_stat_rows, stat_row, path, run_name,
           seprate_log, rendezvous_enabled, _gpu_type,
           _all_gpus, _NVSwitchs, _ngpus_per_node)
    // 初始化派生类自己的成员（建议与构造函数参数同名）
 
{
    this->radix = radix;
    this->crossbar_bw_GBps = crossbar_bw_GBps;
  // 构造函数体：可以添加日志或额外初始化
  std::cout << "NVSwitch Device " << id << " created for NVSwitch device " 
            << std::endl;
}

} // namespace AstraSim