#include "dask.h"

int main() {
    DiskArm diskArm;
    
    // 测试固定序列
    diskArm.FCFS();
    diskArm.SSTF();
    diskArm.SCAN();
    diskArm.CSCAN();
    diskArm.LOOK();
    
    // 测试随机序列
    std::cout << "\nTesting with random requests:\n";
    diskArm.GenerateRequests(10); // 生成10个随机请求
    diskArm.FCFS();
    diskArm.SSTF();
    diskArm.SCAN();
    diskArm.CSCAN();
    diskArm.LOOK();
    
    return 0;
}