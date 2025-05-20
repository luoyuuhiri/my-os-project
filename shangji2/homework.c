#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

/* 函数原型声明 */
int fx(int x);
int fy(int y);
int fxy(int fx, int fy);

int fx(int x) {
    if (x == 1) {
        return 1;
    }
    return fx(x - 1) * x;
}

int fy(int y) {
    if (y == 1 || y == 2) {
        return 1;
    }
    return fy(y - 1) + fy(y - 2);
}

int fxy(int fx, int fy) {
    return fx + fy;
}

int main() {
    const int x = 6;
    const int y = 7;
    int pipe_f1[2], pipe_f2[2];

    /* 创建第一个管道 */
    if (pipe(pipe_f1) < 0) {
        perror("pipe1 creation failed");
        exit(EXIT_FAILURE);
    }

    /* 处理第一个子进程（计算fx） */
    int pid_f1 = fork();
    if (pid_f1 < 0) {
        perror("fork1 failed");
        exit(EXIT_FAILURE);
    }

    if (pid_f1 == 0) {  // 子进程1
        close(pipe_f1[0]);  // 关闭读端
        
        const int result = fx(x);
        write(pipe_f1[1], &result, sizeof(int));
        
        close(pipe_f1[1]);
        _exit(EXIT_SUCCESS);  // 使用_exit避免刷新I/O缓冲区
    } 
    else {  // 父进程
        wait(NULL);  // 等待子进程1结束
        
        int res_fx;
        close(pipe_f1[1]);  // 关闭写端
        read(pipe_f1[0], &res_fx, sizeof(int));
        close(pipe_f1[0]);
        
        printf("f(%d) = %d\n", x, res_fx);
    }

    /* 创建第二个管道 */
    if (pipe(pipe_f2) < 0) {
        perror("pipe2 creation failed");
        exit(EXIT_FAILURE);
    }

    /* 处理第二个子进程（计算fy） */
    int pid_f2 = fork();
    if (pid_f2 < 0) {
        perror("fork2 failed");
        exit(EXIT_FAILURE);
    }

    if (pid_f2 == 0) {  // 子进程2
        close(pipe_f2[0]);  // 关闭读端
        
        const int result = fy(y);
        write(pipe_f2[1], &result, sizeof(int));
        
        close(pipe_f2[1]);
        _exit(EXIT_SUCCESS);
    } 
    else {  // 父进程
        wait(NULL);  // 等待子进程2结束
        
        int res_fy;
        close(pipe_f2[1]);  // 关闭写端
        read(pipe_f2[0], &res_fy, sizeof(int));
        close(pipe_f2[0]);
        
        printf("f(%d) = %d\n", y, res_fy);
    }

    /* 最终结果计算 */
    const int final_res = fxy(res_fx, res_fy);
    printf("f(%d, %d) = %d\n", x, y, final_res);

    return EXIT_SUCCESS;
}