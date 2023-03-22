# CodeCraft2023-CTSU

## 项目框架

根据任务书中的描述，初步思路是设计一个中心化算法和一个去中心化算法：

- 中心化算法接收所有状态信息，维护一个任务队列，调度所有机器人执行的宏观任务；
- 去中心化算法由每一个机器人分别独自完成，从任务队列中取得任务并自行执行，执行过程中自行考虑避障等细节因素。

项目初期可以让中心化算法和去中心化算法均是贪婪的，等第一个能跑的版本出来了之后，再考虑在此框架上更新算法内容。

## TODO

- 陈泰然
  - [x] 整体框架
  - [x] PID算法实现
  - [x] 延迟反馈（forward）
  - [ ] 底层计算
  - [ ] 避障算法
  - [ ] 墙壁建模
  - [ ] ETA
- 蔡恩光
  - [x] 学习git（doge）
  - [x] 学习该项目的代码结构
  - [x] 写出一个最简单的任务分配算法保证机器人的基本运行
  - [ ] 调度算法优化
    - [x] 让机器人们在制造货物时优先让4，5，6保持持平状态不要死命的制造6号物品 1，2，3同理
    - [x] 让机器人优先考虑最近的工作台
    - [ ] 当时间不够时停止执行无法完成任务
      - [ ] 测算任务所需时间
    - [ ] 尽量避免把1 2 3 4 5 6号物品放入9号工作台
    - [ ] 优先填充距离7 和 9 比较近的 4 5 6 号工作台。
    - [ ] 如果当前要接的Task有跟合适的人选就让给他 （如果现在要接的Task的Supply工作台是其他机器人的Demand工作台，那么就把任务让给那个机器人）
  - [x] 在初始化阶段initialize一个 有权无向图

## Brainstorm

- 机器人的控制算法
  - ~~PID算法~~
  - 等机器人方向对准之后再进行加速
- 工作台的状态有两种更新方式
  - [x] 仅依靠帧交互数据更新
  - ~~自行更新~~
- 调度优化内容
  - 让一个receiver工作台可以同时接受来自多个机器人的货物
  - 让机器人们在制造货物时优先让4，5，6保持持平状态不要死命的制造6号物品
  - 还有1，2，3

## 相关问题

- 现阶段的center.decide()在每次调用时都会设置每个robot的target，而这导致每一帧的PIDController.memory都会被清空，导致积分项无效。
