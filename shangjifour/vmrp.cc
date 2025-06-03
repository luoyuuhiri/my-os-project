// vmrp.cc
#include "vmrp.h"
#include <ctime>
#include <cstdlib>
using namespace std;

Replace::Replace()
{
    srand(time(0));  // 初始化随机种子

    cout << "Please input page numbers (-1 for random): ";
    cin >> PageNumber;

    if (PageNumber == -1) {
        PageNumber = 20;
        ReferencePage = new int[PageNumber];
        cout << "Random reference page string: ";
        for (int i = 0; i < PageNumber; i++) {
            ReferencePage[i] = rand() % 10;
            cout << ReferencePage[i] << " ";
        }
        cout << endl;
    } else {
        ReferencePage = new int[PageNumber];
        cout << "Please input reference page string: ";
        for (int i = 0; i < PageNumber; i++)
            cin >> ReferencePage[i];
    }

    cout << "Please input page frames: ";
    cin >> FrameNumber;

    EliminatePage = new int[PageNumber];
    PageFrames = new int[FrameNumber];
}

Replace::~Replace()
{
    delete[] ReferencePage;
    delete[] EliminatePage;
    delete[] PageFrames;
}

void Replace::InitSpace(char *MethodName)
{
    cout << endl << MethodName << endl;
    FaultNumber = 0;
    for (int i = 0; i < PageNumber; i++)
        EliminatePage[i] = -1;
    for (int i = 0; i < FrameNumber; i++)
        PageFrames[i] = -1;
}

void Replace::Report(void)
{
    cout << endl << "Eliminate page:";
    for (int i = 0; EliminatePage[i] != -1; i++)
        cout << EliminatePage[i] << " ";
    cout << endl << "Number of page faults = " << FaultNumber << endl;
    cout << fixed << setprecision(1);
    cout << "Rate of page faults = " << 100.0 * FaultNumber / PageNumber << "%" << endl;
}

void Replace::Fifo(void)
{
    int pointer = 0, l = 0;
    InitSpace("FIFO");

    for (int k = 0; k < PageNumber; k++) {
        int next = ReferencePage[k];
        bool found = false;
        int eliminated = -1;

        for (int i = 0; i < FrameNumber; i++) {
            if (PageFrames[i] == next) {
                found = true;
                break;
            }
        }

        if (!found) {
            FaultNumber++;
            if (PageFrames[pointer] != -1)
                EliminatePage[l++] = PageFrames[pointer];
            PageFrames[pointer] = next;
            pointer = (pointer + 1) % FrameNumber;
        }

        for (int j = 0; j < FrameNumber; j++) {
            if (PageFrames[j] != -1) {
                cout << PageFrames[j];
                if (j < FrameNumber - 1 && PageFrames[j + 1] != -1)
                    cout << " ";
            }
        }
        if (!found && eliminated != -1)
            cout << "->" << eliminated;
        cout << endl;
    }

    Report();
}

void Replace::Lru(void)
{
    InitSpace("LRU");
    int l = 0;

    for (int k = 0; k < PageNumber; k++) {
        int next = ReferencePage[k];
        bool found = false;
        int position = -1;

        for (int i = 0; i < FrameNumber; i++) {
            if (PageFrames[i] == next) {
                found = true;
                position = i;
                break;
            }
        }

        if (found) {
            int temp = PageFrames[position];
            for (int j = position; j > 0; j--)
                PageFrames[j] = PageFrames[j - 1];
            PageFrames[0] = temp;
        } else {
            FaultNumber++;
            if (PageFrames[FrameNumber - 1] != -1)
                EliminatePage[l++] = PageFrames[FrameNumber - 1];
            for (int j = FrameNumber - 1; j > 0; j--)
                PageFrames[j] = PageFrames[j - 1];
            PageFrames[0] = next;
        }

        for (int j = 0; j < FrameNumber; j++) {
            if (PageFrames[j] != -1) {
                cout << PageFrames[j];
                if (j < FrameNumber - 1 && PageFrames[j + 1] != -1)
                    cout << " ";
            }
        }

        if (!found && PageFrames[FrameNumber - 1] != -1)
            cout << "->" << PageFrames[FrameNumber - 1];
        cout << endl;
    }

    Report();
}

void Replace::Eclock(void)
{
    InitSpace("Enhanced Clock (Eclock)");
    int* Rbit = new int[FrameNumber];
    int* Mbit = new int[FrameNumber];
    int pointer = 0, l = 0;

    for (int i = 0; i < FrameNumber; i++) {
        PageFrames[i] = -1;
        Rbit[i] = 0;
        Mbit[i] = 0;
    }

    for (int k = 0; k < PageNumber; k++) {
        int next = ReferencePage[k];
        bool found = false;

        // 是否命中
        for (int i = 0; i < FrameNumber; i++) {
            if (PageFrames[i] == next) {
                Rbit[i] = 1;
                found = true;
                break;
            }
        }

        if (!found) {
            FaultNumber++;

            // 寻找替换页
            bool replaced = false;
            int round = 0;
            while (!replaced && round < 4) {
                for (int i = 0; i < FrameNumber && !replaced; i++) {
                    int idx = (pointer + i) % FrameNumber;
                    switch (round) {
                        case 0:
                            if (PageFrames[idx] == -1 ||
                                (Rbit[idx] == 0 && Mbit[idx] == 0)) {
                                if (PageFrames[idx] != -1)
                                    EliminatePage[l++] = PageFrames[idx];
                                PageFrames[idx] = next;
                                Rbit[idx] = 1;
                                Mbit[idx] = rand() % 2;
                                pointer = (idx + 1) % FrameNumber;
                                replaced = true;
                            }
                            break;
                        case 1:
                            if (Rbit[idx] == 0 && Mbit[idx] == 1) {
                                EliminatePage[l++] = PageFrames[idx];
                                PageFrames[idx] = next;
                                Rbit[idx] = 1;
                                Mbit[idx] = rand() % 2;
                                pointer = (idx + 1) % FrameNumber;
                                replaced = true;
                            }
                            break;
                        case 2:
                            if (Rbit[idx] == 1 && Mbit[idx] == 0)
                                Rbit[idx] = 0;
                            break;
                        case 3:
                            if (Rbit[idx] == 1 && Mbit[idx] == 1)
                                Rbit[idx] = 0;
                            break;
                    }
                }
                round++;
            }
        }

        for (int j = 0; j < FrameNumber; j++) {
            if (PageFrames[j] != -1) {
                cout << PageFrames[j];
                if (j < FrameNumber - 1 && PageFrames[j + 1] != -1)
                    cout << " ";
            }
        }
        cout << endl;
    }

    delete[] Rbit;
    delete[] Mbit;

    Report();
}

void Replace::Clock(void)
{
    int pointer = 0; // 指向当前扫描页框
    int *Rbit = new int[FrameNumber](); // 访问位，初始化为0
    int l = 0;

    InitSpace("Clock");

    for (int k = 0; k < PageNumber; k++)
    {
        int next = ReferencePage[k];
        bool found = false;
        int eliminated = -1;

        // 检查页面是否已经在内存
        for (int i = 0; i < FrameNumber; i++)
        {
            if (PageFrames[i] == next)
            {
                found = true;
                Rbit[i] = 1; // 设置访问位
                break;
            }
        }

        if (!found)
        {
            FaultNumber++;
            // 找替换位置
            while (Rbit[pointer] == 1)
            {
                Rbit[pointer] = 0; // 清0，第二次机会
                pointer = (pointer + 1) % FrameNumber;
            }
            // 淘汰页面
            eliminated = PageFrames[pointer];
            if (eliminated != -1)
                EliminatePage[l++] = eliminated;

            PageFrames[pointer] = next;
            Rbit[pointer] = 1; // 新页面访问位置1
            pointer = (pointer + 1) % FrameNumber;
        }

        // 输出内存状态
        for (int i = 0; i < FrameNumber; i++)
        {
            if (PageFrames[i] != -1)
            {
                cout << PageFrames[i];
                if (i < FrameNumber - 1 && PageFrames[i + 1] != -1)
                    cout << " ";
            }
        }
        if (!found && eliminated != -1)
            cout << "->" << eliminated;
        cout << endl;
    }
    delete[] Rbit;
    Report();
}

void Replace::Lfu(void)
{
    int *count = new int[FrameNumber](); // 记录各页使用频率
    int l = 0;

    InitSpace("LFU");

    for (int k = 0; k < PageNumber; k++)
    {
        int next = ReferencePage[k];
        bool found = false;
        int eliminated = -1;

        // 检查页面是否在内存
        for (int i = 0; i < FrameNumber; i++)
        {
            if (PageFrames[i] == next)
            {
                found = true;
                count[i]++;
                break;
            }
        }

        if (!found)
        {
            FaultNumber++;
            // 如果内存未满，放入空位
            bool placed = false;
            for (int i = 0; i < FrameNumber; i++)
            {
                if (PageFrames[i] == -1)
                {
                    PageFrames[i] = next;
                    count[i] = 1;
                    placed = true;
                    break;
                }
            }

            if (!placed)
            {
                // 找使用次数最少的页面淘汰
                int minCount = count[0];
                int idx = 0;
                for (int i = 1; i < FrameNumber; i++)
                {
                    if (count[i] < minCount)
                    {
                        minCount = count[i];
                        idx = i;
                    }
                }
                eliminated = PageFrames[idx];
                EliminatePage[l++] = eliminated;
                PageFrames[idx] = next;
                count[idx] = 1;
            }
        }

        // 输出内存状态
        for (int i = 0; i < FrameNumber; i++)
        {
            if (PageFrames[i] != -1)
            {
                cout << PageFrames[i];
                if (i < FrameNumber - 1 && PageFrames[i + 1] != -1)
                    cout << " ";
            }
        }
        if (!found && eliminated != -1)
            cout << "->" << eliminated;
        cout << endl;
    }
    delete[] count;
    Report();
}

void Replace::Mfu(void)
{
    int *count = new int[FrameNumber](); // 记录各页使用频率
    int l = 0;

    InitSpace("MFU");

    for (int k = 0; k < PageNumber; k++)
    {
        int next = ReferencePage[k];
        bool found = false;
        int eliminated = -1;

        // 检查页面是否在内存
        for (int i = 0; i < FrameNumber; i++)
        {
            if (PageFrames[i] == next)
            {
                found = true;
                count[i]++;
                break;
            }
        }

        if (!found)
        {
            FaultNumber++;
            // 如果内存未满，放入空位
            bool placed = false;
            for (int i = 0; i < FrameNumber; i++)
            {
                if (PageFrames[i] == -1)
                {
                    PageFrames[i] = next;
                    count[i] = 1;
                    placed = true;
                    break;
                }
            }

            if (!placed)
            {
                // 找使用次数最多的页面淘汰
                int maxCount = count[0];
                int idx = 0;
                for (int i = 1; i < FrameNumber; i++)
                {
                    if (count[i] > maxCount)
                    {
                        maxCount = count[i];
                        idx = i;
                    }
                }
                eliminated = PageFrames[idx];
                EliminatePage[l++] = eliminated;
                PageFrames[idx] = next;
                count[idx] = 1;
            }
        }

        // 输出内存状态
        for (int i = 0; i < FrameNumber; i++)
        {
            if (PageFrames[i] != -1)
            {
                cout << PageFrames[i];
                if (i < FrameNumber - 1 && PageFrames[i + 1] != -1)
                    cout << " ";
            }
        }
        if (!found && eliminated != -1)
            cout << "->" << eliminated;
        cout << endl;
    }
    delete[] count;
    Report();
}


int main()
{
    Replace *vmpr = new Replace();

    vmpr->Fifo();
    vmpr->Lru();
    vmpr->Clock();
    vmpr->Eclock();
    vmpr->Lfu();
    vmpr->Mfu();

    delete vmpr;
    return 0;
}

