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
  - [ ] 底层计算
- 蔡恩光
  - [x] 学习git（doge）
  - [ ] 学习该项目的代码结构
  - [ ] 写出一个最简单的任务分配算法保证机器人的基本运行
  - [ ] 算法优化

## Brainstorm

- 机器人的控制算法可以参考PID算法
- 工作台的状态有两种更新方式
  - 仅依靠帧交互数据更新
  - ~~自行更新~~
