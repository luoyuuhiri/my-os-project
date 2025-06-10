***本次实验主要完成了对于FCFS，SSTF，SCAN、C-SCAN、LOOK五种算法的学习和对比***

***部分输出截图***

[![pVkSrrD.png](https://s21.ax1x.com/2025/06/10/pVkSrrD.png)](https://imgse.com/i/pVkSrrD)

[![pVkSDKO.png](https://s21.ax1x.com/2025/06/10/pVkSDKO.png)](https://imgse.com/i/pVkSDKO)

***辨析五种算法:FCFS(先来先服务)，最简单，但是平均寻道距离一般最大，而且折返方向的情况较为频繁;SSTF(最短寻道时间优先)，选择当前距离磁头最近的请求，但是假如在某一个区域持续有请求的话，对于远处的请求就会很难顾及;SCAN(电梯)，磁头沿一个方向移动，直到到磁盘一段，再折返(折返途中也处理请求)，平均寻道距离比较好，而且没有SSTF所说的问题(饥饿);C-SCAN(循环扫描)，在SCAN的基础上，到达一端之后直接跳到另一端，这样能保证更平均的等待时间，但是寻道时间可能大于SCAN;LOOK，类似SCAN，区别在于:SCAN磁头要走到磁盘的某一端，但是LOOK只需要走到目前请求的最远段，可以减少不必要的移动，大部分情景更加高效***