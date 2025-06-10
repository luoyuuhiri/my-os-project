#include "dask.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>

DiskArm::DiskArm() {
    int temp = 0;
    std::cout << "Please input current Cylinder:";
    std::cin >> temp;
    CurrentCylinder = temp;
    
    std::cout << "Please input current direction (0/1):";
    std::cin >> temp;
    SeekDirection = temp;
    
    std::cout << "Please input request numbers:";
    std::cin >> temp;
    RequestNumber = temp;
    
    Request = new int[RequestNumber];
    Cylinder = new int[RequestNumber];
    
    std::cout << "Please input request cylinder string:";
    for (int i = 0; i < RequestNumber; i++) {
        std::cin >> temp;
        Request[i] = temp;
    }
}

DiskArm::~DiskArm() {
    delete[] Request;
    delete[] Cylinder;
}

void DiskArm::GenerateRequests(int count) {
    RequestNumber = count;
    delete[] Request;
    delete[] Cylinder;
    Request = new int[RequestNumber];
    Cylinder = new int[RequestNumber];
    
    srand(time(0));
    for (int i = 0; i < RequestNumber; i++) {
        Request[i] = rand() % 200; // 假设柱面号范围0-199
    }
    
    std::cout << "Generated request sequence: ";
    for (int i = 0; i < RequestNumber; i++) {
        std::cout << Request[i] << " ";
    }
    std::cout << std::endl;
}

void DiskArm::InitSpace(char *MethodName) {
    std::cout << std::endl << MethodName << std::endl;
    SeekNumber = 0;
    SeekChange = 0;
    for (int i = 0; i < RequestNumber; i++)
        Cylinder[i] = Request[i];
}

void DiskArm::Report() {
    std::cout << std::endl;
    std::cout << "Seek Number: " << SeekNumber << std::endl;
    std::cout << "Change Direction: " << SeekChange << std::endl << std::endl;
}

void DiskArm::FCFS() {
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    InitSpace("FCFS");
    std::cout << Current;
    
    for (int i = 0; i < RequestNumber; i++) {
        int target = Cylinder[i];
        int diff = target - Current;
        bool needChangeDirection = (diff >= 0 && !Direction) || (diff < 0 && Direction);
        
        if (needChangeDirection) {
            Direction = !Direction;
            SeekChange++;
            std::cout << std::endl << Current << "->" << target;
        } else {
            std::cout << "->" << target;
        }
        SeekNumber += std::abs(diff);
        Current = target;
    }
    Report();
}

void DiskArm::SSTF() {
    InitSpace("SSTF");
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    std::cout << Current;
    
    std::vector<int> requests(Request, Request + RequestNumber);
    std::vector<bool> processed(RequestNumber, false);
    
    while (std::find(processed.begin(), processed.end(), false) != processed.end()) {
        int min_dist = INT_MAX;
        int next_idx = -1;
        
        for (int i = 0; i < RequestNumber; i++) {
            if (!processed[i]) {
                int dist = std::abs(requests[i] - Current);
                if (dist < min_dist) {
                    min_dist = dist;
                    next_idx = i;
                }
            }
        }
        
        int next = requests[next_idx];
        processed[next_idx] = true;
        
        bool needChangeDirection = ((next >= Current) && !Direction) ||
                                  ((next < Current) && Direction);
        if (needChangeDirection) {
            Direction = !Direction;
            SeekChange++;
            std::cout << std::endl << Current << "->" << next;
        } else {
            std::cout << "->" << next;
        }
        
        SeekNumber += std::abs(Current - next);
        Current = next;
    }
    Report();
}

void DiskArm::SCAN() {
    InitSpace("SCAN");
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    std::cout << Current;
    
    std::vector<int> requests(Request, Request + RequestNumber);
    std::sort(requests.begin(), requests.end());
    std::vector<bool> processed(RequestNumber, false);
    
    auto processRequests = [&](bool forward) {
        int step = forward ? 1 : -1;
        int start = forward ? 0 : RequestNumber - 1;
        int end = forward ? RequestNumber : -1;
        
        for (int i = start; i != end; i += step) {
            if (!processed[i] && ((forward && requests[i] >= Current) || (!forward && requests[i] <= Current))) {
                std::cout << "->" << requests[i];
                SeekNumber += std::abs(Current - requests[i]);
                Current = requests[i];
                processed[i] = true;
            }
        }
    };
    
    processRequests(Direction == 1);
    
    if (Direction == 1) {
        int max_req = *std::max_element(Request, Request + RequestNumber);
        if (Current != max_req) {
            std::cout << "->" << max_req;
            SeekNumber += std::abs(Current - max_req);
            Current = max_req;
        }
    } else {
        int min_req = *std::min_element(Request, Request + RequestNumber);
        if (Current != min_req) {
            std::cout << "->" << min_req;
            SeekNumber += std::abs(Current - min_req);
            Current = min_req;
        }
    }
    
    Direction = !Direction;
    SeekChange++;
    std::cout << std::endl << Current;
    
    processRequests(Direction == 1);
    Report();
}

void DiskArm::CSCAN() {
    InitSpace("C-SCAN");
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    std::cout << Current;
    
    std::vector<int> requests(Request, Request + RequestNumber);
    std::sort(requests.begin(), requests.end());
    std::vector<bool> processed(RequestNumber, false);
    
    auto processRequests = [&](bool forward) {
        int step = forward ? 1 : -1;
        int start = forward ? 0 : RequestNumber - 1;
        int end = forward ? RequestNumber : -1;
        
        for (int i = start; i != end; i += step) {
            if (!processed[i] && ((forward && requests[i] >= Current) || (!forward && requests[i] <= Current))) {
                std::cout << "->" << requests[i];
                SeekNumber += std::abs(Current - requests[i]);
                Current = requests[i];
                processed[i] = true;
            }
        }
    };
    
    processRequests(Direction == 1);
    
    if (Direction == 1) {
        std::cout << "->0";
        SeekNumber += Current;
        Current = 0;
    } else {
        std::cout << "->199";
        SeekNumber += (199 - Current);
        Current = 199;
    }
    
    SeekChange++;
    processRequests(Direction == 1);
    Report();
}

void DiskArm::LOOK() {
    InitSpace("LOOK");
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    std::cout << Current;
    
    std::vector<int> requests(Request, Request + RequestNumber);
    std::sort(requests.begin(), requests.end());
    std::vector<bool> processed(RequestNumber, false);
    
    auto processRequests = [&](bool forward) {
        int step = forward ? 1 : -1;
        int start = forward ? 0 : RequestNumber - 1;
        int end = forward ? RequestNumber : -1;
        
        for (int i = start; i != end; i += step) {
            if (!processed[i] && ((forward && requests[i] >= Current) || (!forward && requests[i] <= Current))) {
                std::cout << "->" << requests[i];
                SeekNumber += std::abs(Current - requests[i]);
                Current = requests[i];
                processed[i] = true;
            }
        }
    };
    
    processRequests(Direction == 1);
    
    Direction = !Direction;
    SeekChange++;
    std::cout << std::endl << Current;
    
    processRequests(Direction == 1);
    Report();
}