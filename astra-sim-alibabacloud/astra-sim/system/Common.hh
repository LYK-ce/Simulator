/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __COMMON_HH__
#define __COMMON_HH__
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include "AstraNetworkAPI.hh"

enum class GPUType { A100, A800, H100, H800, NONE, H20};

namespace AstraSim {
#define CLOCK_PERIOD 1
#define FREQ (1000.0 / CLOCK_PERIOD)
#define GBps 1.0 / (1024 * 1024 * 1024)
typedef unsigned long long Tick;
enum class ComType {
  None,
  Reduce_Scatter,
  All_Gather,
  All_Reduce,
  All_to_All,
  All_Reduce_All_to_All,
  All_Reduce_NVLS
};
enum class CollectiveOptimization { Baseline, LocalBWAware };
enum class CollectiveImplementationType {
  Ring, 
  OneRing,
  Direct, 
  OneDirect,
  AllToAll,
  DoubleBinaryTreeLocalAllToAll,
  LocalRingNodeA2AGlobalDBT,
  HierarchicalRing,
  DoubleBinaryTree,
  HalvingDoubling,  
  OneHalvingDoubling,
  NcclFlowModel,
  NcclTreeFlowModel,
};
enum class CollectiveBarrier { Blocking, Non_Blocking };
enum class SchedulingPolicy { LIFO, FIFO, HIGHEST, None };
enum class IntraDimensionScheduling {
  FIFO,
  RG,
  SmallestFirst,
  LessRemainingPhaseFirst
};
enum class InterDimensionScheduling {
  Ascending,
  OnlineGreedy,
  RoundRobin,
  OfflineGreedy,
  OfflineGreedyFlex
};
enum class InjectionPolicy {
  Infinite,
  Aggressive,
  SemiAggressive,
  ExtraAggressive,
  Normal
};
enum class PacketRouting { Hardware, Software };
enum class BusType { Both, Shared, Mem };
enum class StreamState {
  Created,
  Transferring,
  Ready,
  Executing,
  Zombie,
  Dead
};
enum class EventType {
  NONE,
  RendezvousSend,
  RendezvousRecv,
  CallEvents,
  PacketReceived,
  PacketSent,
  PacketSentFinshed,
  WaitForVnetTurn,
  NCCL_General,
  General,
  TX_DMA,
  RX_DMA,
  Wight_Grad_Comm_Finished,
  Input_Grad_Comm_Finished,
  Fwd_Comm_Finished,
  Wight_Grad_Comm_Finished_After_Delay,
  Input_Grad_Comm_Finished_After_Delay,
  Fwd_Comm_Finished_After_Delay,
  Workload_Wait,
  Reduction_Ready,
  Rec_Finished,
  Send_Finished,
  Processing_Finished,
  Delivered,
  NPU_to_MA,
  MA_to_NPU,
  Read_Port_Free,
  Write_Port_Free,
  Apply_Boost,
  Stream_Transfer_Started,
  Stream_Ready,
  Consider_Process,
  Consider_Retire,
  Consider_Send_Back,
  StreamInit,
  StreamsFinishedIncrease,
  CommProcessingFinished,
  NotInitialized
};
inline std::ostream& operator<<(std::ostream& os, const EventType& event) {
        switch (event) {
            case EventType::NONE: os << "NONE"; break;
            case EventType::RendezvousSend: os << "RendezvousSend"; break;
            case EventType::RendezvousRecv: os << "RendezvousRecv"; break;
            case EventType::CallEvents: os << "CallEvents"; break;
            case EventType::PacketReceived: os << "PacketReceived"; break;
            case EventType::PacketSent: os << "PacketSent"; break;
            case EventType::PacketSentFinshed: os << "PacketSentFinshed"; break;
            case EventType::WaitForVnetTurn: os << "WaitForVnetTurn"; break;
            case EventType::NCCL_General: os << "NCCL_General"; break;
            case EventType::General: os << "General"; break;
            case EventType::TX_DMA: os << "TX_DMA"; break;
            case EventType::RX_DMA: os << "RX_DMA"; break;
            case EventType::Wight_Grad_Comm_Finished: os << "Wight_Grad_Comm_Finished"; break;
            case EventType::Input_Grad_Comm_Finished: os << "Input_Grad_Comm_Finished"; break;
            case EventType::Fwd_Comm_Finished: os << "Fwd_Comm_Finished"; break;
            case EventType::Wight_Grad_Comm_Finished_After_Delay: os << "Wight_Grad_Comm_Finished_After_Delay"; break;
            case EventType::Input_Grad_Comm_Finished_After_Delay: os << "Input_Grad_Comm_Finished_After_Delay"; break;
            case EventType::Fwd_Comm_Finished_After_Delay: os << "Fwd_Comm_Finished_After_Delay"; break;
            case EventType::Workload_Wait: os << "Workload_Wait"; break;
            case EventType::Reduction_Ready: os << "Reduction_Ready"; break;
            case EventType::Rec_Finished: os << "Rec_Finished"; break;
            case EventType::Send_Finished: os << "Send_Finished"; break;
            case EventType::Processing_Finished: os << "Processing_Finished"; break;
            case EventType::Delivered: os << "Delivered"; break;
            case EventType::NPU_to_MA: os << "NPU_to_MA"; break;
            case EventType::MA_to_NPU: os << "MA_to_NPU"; break;
            case EventType::Read_Port_Free: os << "Read_Port_Free"; break;
            case EventType::Write_Port_Free: os << "Write_Port_Free"; break;
            case EventType::Apply_Boost: os << "Apply_Boost"; break;
            case EventType::Stream_Transfer_Started: os << "Stream_Transfer_Started"; break;
            case EventType::Stream_Ready: os << "Stream_Ready"; break;
            case EventType::Consider_Process: os << "Consider_Process"; break;
            case EventType::Consider_Retire: os << "Consider_Retire"; break;
            case EventType::Consider_Send_Back: os << "Consider_Send_Back"; break;
            case EventType::StreamInit: os << "StreamInit"; break;
            case EventType::StreamsFinishedIncrease: os << "StreamsFinishedIncrease"; break;
            case EventType::CommProcessingFinished: os << "CommProcessingFinished"; break;
            case EventType::NotInitialized: os << "NotInitialized"; break;
            default: os << "UNKNOWN"; break;
        }
        return os;
    }


class CloneInterface {
 public:
  virtual CloneInterface* clone() const = 0;
  virtual ~CloneInterface() = default;
};
class CollectiveImplementation : public CloneInterface {
 public:
  CollectiveImplementationType type;
  CollectiveImplementation(CollectiveImplementationType type) {
    this->type = type;
  };
  virtual CloneInterface* clone() const {
    return new CollectiveImplementation(*this);
  }
};
class DirectCollectiveImplementation : public CollectiveImplementation {
 public:
  int direct_collective_window;
  CloneInterface* clone() const {
    return new DirectCollectiveImplementation(*this);
  };
  DirectCollectiveImplementation(
      CollectiveImplementationType type,
      int direct_collective_window)
      : CollectiveImplementation(type) {
    this->direct_collective_window = direct_collective_window;
  }
};
} // namespace AstraSim
#endif
