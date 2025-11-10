/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef __MEMBUS_HH__
#define __MEMBUS_HH__

#include <assert.h>
#include <math.h>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <list>
#include <map>
#include <sstream>
#include <tuple>
#include <vector>
#include "Callable.hh"
#include "Common.hh"

namespace AstraSim {
class Device;
class LogGP;
class MemBus {
 public:
  enum class Transmition { Fast, Usual };
  LogGP* NPU_side;
  LogGP* MA_side;
  Device* generator;
  int communication_delay;
  bool model_shared_bus;
  ~MemBus();
  MemBus(
      std::string side1,
      std::string side2,
      Device* generator,
      Tick L,
      Tick o,
      Tick g,
      double G,
      bool model_shared_bus,
      int communication_delay,
      bool attach);
  void send_from_NPU_to_MA(
      Transmition transmition,
      uint64_t bytes,
      bool processed,
      bool send_back,
      Callable* callable);
  void send_from_MA_to_NPU(
      Transmition transmition,
      uint64_t bytes,
      bool processed,
      bool send_back,
      Callable* callable);
};
} // namespace AstraSim
#endif
