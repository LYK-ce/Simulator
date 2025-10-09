#TOR ASW
top of rack指的是每个服务器机柜上部署的1~2台交换机，服务器直接接入到本机柜的交换机上。实现服务器与交换机在机柜内的互联。
Access Switch 接入交换机或称作机顶交换机TOR

dual TOR 每台服务器同时挂到两组TOR上，但这两组TOR仍然共用一个上联平面 single Plane

#PSW
Pod switch或者Aggregation switch
汇聚交换机，位于ASW上一层，把整个Pod（多个机柜）里的ASW收拢起来。

#上联平面
服务器/TOR往汇聚-核心层方向所连接的那一套（或两套）交换机+链接集合
单平面
服务器 ──ToR──┐
              ├─ 同一组汇聚交换机 ── 同一组核心交换机
服务器 ──ToR──┘

双平面
服务器 ──ToR──┬─ 汇聚平面 A ── 核心平面 A
              │
服务器 ──ToR──┴─ 汇聚平面 B ── 核心平面 B

#Rail
Rail-Optimized是一种网络布线/拓扑策略：
把不同服务器里“同一编号”的GPU全部收拢到同一台（同一组）轨道交换机上，形成一条“Rail”，让这些GPU之间通信只经过一层交换，延迟最低，带宽最高。

rail-optimized =“按 GPU 编号排轨道，把同一轨道 GPU 收进同一交换机，让训练流量走最短路径”，是专为大模型 AllReduce/集合通信设计的高性能拓扑法


最终会来到这里
/home/iochimari/SimAI/astra-sim-alibabacloud/extern/network_backend/ns3-interface/simulation/src/mtp/model/logical-process.cc:183