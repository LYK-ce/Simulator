# 硬件建模 Schema（ASCII 图）

以下为 5 类硬件（Host CPU、GPU、NVSwitch、NIC、Switch）继承统一 `Hardware` 基类的类图与两台主机 + 5 跳 IB 网络的实例拓扑，供实现方参考。

---

## 1) 类关系（继承 + 组合）

```
                         +-------------------------------------+
                         |            Hardware (abstract)      |
                         +-------------------------------------+
                         | id: string                          |
                         | name: string                        |
                         | host_id: string (可空, Switch 可为空)|
                         | ports: vector<Port>                 |
                         | power_w: double (可选)              |
                         | metrics: KeyValue (可选)            |
                         +-------------------------------------+
                         | +type(): enum {CPU,GPU,NVSW,NIC,SW} |
                         | +add_port(p: Port)                  |
                         | +get_ports(kind?): vector<Port*>    |
                         | +info(): string                     |
                         +------------------^----^-----^----^--+
                                            |    |     |    |
        +-----------------------------------+    |     |    +---------------------+
        |                                        |     |                          |
+----------------------+               +-----------------+              +------------------+
| HostCPU : Hardware   |               | GPU : Hardware  |              | NIC : Hardware   |
+----------------------+               +-----------------+              +------------------+
| cores: int           |               | vram_bytes: u64 |              | pci_gen: int     |
| numa_id: int         |               | tflops: double  |              | lanes: int       |
| mem_bw_GBps: double  |               | nvlink_lanes:int|              | proto: enum{IB,..}|
+----------------------+               +-----------------+              +------------------+
| 端口: PCIe           |               | 端口: NVLink,PCIe|              | 端口: PCIe, IB   |
+----------------------+               +-----------------+              +------------------+

        +-----------------------------------+
        |                                   |
+--------------------------+        +------------------------+
| NVSwitch : Hardware      |        | Switch : Hardware      |
+--------------------------+        +------------------------+
| radix: int               |        | role: enum{Leaf,Spine,Core} |
| crossbar_bw_GBps: double |        | radix: int             |
+--------------------------+        +------------------------+
| 端口: NVLink             |        | 端口: IB               |
+--------------------------+        +------------------------+
```

### 端口与链路（建议独立抽象）

```
+-------------------------+            +--------------------------+
| Port                    |            | Link                     |
+-------------------------+            +--------------------------+
| id: string              |            | id: string               |
| kind: enum{PCIe,NVLink,IB} |         | a: Port*                 |
| speed_Gbps: double      |            | b: Port*                 |
| base_latency_us: double |            | bw_Gbps: double          |
| owner: Hardware*        |            | latency_us: double       |
+-------------------------+            | mtu: int (可选)          |
                                       | error_rate: double(可选) |
                                       +--------------------------+
```

### 路径与代价（供仿真/估算使用）

```
+---------------------------+
| Path                      |
+---------------------------+
| steps: vector<Link*>      |  // 逐跳链路（端到端）
| signature: string         |  // 例: "leaf-spine-core-spine-leaf"
+---------------------------+
| +one_way_latency_us()     |
| +bottleneck_GBps()        |
+---------------------------+
```

> 说明：
> - 端口 Port 归属于某个 Hardware；链路 Link 连接两个 Port。
> - 所有传输都“走链路集合”，由 Path 聚合，不在 Hardware 里硬编码路由。
> - 带宽/时延放在 Link（链路）层，利于表达“同类设备不同连接”的差异。

---

## 2) 实例拓扑（两台主机 + 5 跳 IB 网络）

```
          ┌──────────────────────── Host A ────────────────────────┐
          │                                                        │
          │   CPU_A (HostCPU)                                      │
          │     │  PCIe                                            │
          │   ┌─┴───────────┐                                      │
          │   │             │                                      │
          │  NIC_A0       NIC_A1        NIC_A2        NIC_A3       │
          │ (mlx5_0)     (mlx5_2)      (mlx5_5)      (mlx5_8)      │
          │   │ IB         │ IB          │ IB          │ IB        │
          │   │            │             │             │           │
          │  GPU_A0..7 (GPU) ── NVLink ── NVSW_A (NVSwitch)         │
          │       (多 GPU 以 NVLink/NVSwitch 形成组网)              │
          └─────────────────────────────────────────────────────────┘
                  │          │             │             │
                  │          │             │             │
                  ▼          ▼             ▼             ▼
           [Leaf_A0]   [Leaf_A1]     [Leaf_A2]     [Leaf_A3]     ← Switch(role=Leaf)

                    ╲        ╲            ╲           ╲
                     ▼        ▼            ▼           ▼
                   [Spine_X] [Spine_Y]   [Spine_Z]  ...           ← Switch(role=Spine)

                           ▼            （可经多条等价脊）
                          [Core]                                         ← Switch(role=Core)

                     ▲         ▲           ▲          ▲
                    /         /           /          /
             [Spine_X'] [Spine_Y']  [Spine_Z']   ...
                  ▲         ▲           ▲
                  │         │           │
           [Leaf_B0]  [Leaf_B1]   [Leaf_B2]   [Leaf_B3]               ← Switch(role=Leaf)
                  │         │           │          │
                  │         │           │          │
          ┌──────────────────────── Host B ────────────────────────┐
          │  NIC_B0      NIC_B1         NIC_B2        NIC_B3      │
          │  (mlx5_0)    (mlx5_2)       (mlx5_5)      (mlx5_8)    │
          │    │ PCIe       │ PCIe         │ PCIe        │ PCIe    │
          │  CPU_B (HostCPU)                                  │    │
          │                                                   │    │
          │      GPU_B0..7 (GPU) ── NVLink ── NVSW_B (NVSwitch)    │
          └────────────────────────────────────────────────────────┘

注：
- A↔B 任意一对 NIC 的端到端 Path：Leaf → Spine → Core → Spine → Leaf（5 跳，均为 Switch 硬件）。
- GPU→NIC 的数据走 PCIe（P2P/经 PHB），表现为 GPU 的 PCIe Port ↔ NIC 的 PCIe Port 之间的一条或多条 Link。
```

---

## 3) 典型连接关系（落到类/端口）

- **HostCPU**：提供 *PCIe* 端口（到多个 NIC、到 GPU/Root Complex）；可带主存带宽指标。
- **GPU**：提供 *NVLink* 端口（到 NVSwitch 或其它 GPU）与 *PCIe* 端口（到 CPU/NIC/Root）。
- **NVSwitch**：提供 *NVLink* 端口（高径向，连接多块 GPU）。
- **NIC**：提供 *PCIe* 端口（到 CPU/Root/GPU P2P）与 *IB* 端口（到 Leaf 交换机）。
- **Switch**（Leaf/Spine/Core 由 `role` 表示）：提供 *IB* 端口（互联其它 Switch 或 NIC）。



