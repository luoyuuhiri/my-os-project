#include<iostream>
#include<iomanip>
#include<ctime>
using namespace std;

class Replace { 
public:
    Replace();
    ~Replace();
    void InitSpace(char* MethodName);
    void Report(void);
    void Fifo(void);
    void Lru(void);
    void Clock(void);
    void Eclock(void);
    void Lfu(void);
    void Mfu(void);

private:
    int* ReferencePage;
    int* EliminatePage;
    int* PageFrames;
    int PageNumber;
    int FrameNumber;
    int FaultNumber;
};
