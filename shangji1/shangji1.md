***上机作业1***

***任务1：完成示例代码的编译和执行***

[![pEjPe0K.png](https://s21.ax1x.com/2025/05/13/pEjPe0K.png)](https://imgse.com/i/pEjPe0K)

[![pEjPmTO.png](https://s21.ax1x.com/2025/05/13/pEjPmTO.png)](https://imgse.com/i/pEjPmTO)

***第一个代码因为没有信号量和互斥锁，sum在自增的时候如果两个线程同时执行，有一个自增会被覆盖掉，所以最后的值远小于2000000***

[![pEjP11A.png](https://s21.ax1x.com/2025/05/13/pEjP11A.png)](https://imgse.com/i/pEjP11A)

[![pEjP36I.png](https://s21.ax1x.com/2025/05/13/pEjP36I.png)](https://imgse.com/i/pEjP36I)

***第二个代码加了互斥锁，在一个线程执行的时候，另一个无法执行，因此能保证sum自增可靠性，结果是2000000***

[![pEjP8Xt.png](https://s21.ax1x.com/2025/05/13/pEjP8Xt.png)](https://imgse.com/i/pEjP8Xt)

[![pEjPJnP.png](https://s21.ax1x.com/2025/05/13/pEjPJnP.png)](https://imgse.com/i/pEjPJnP)

***第三个代码加入了信号量，因为是两个线程，情况比较简单，和互斥锁的代码类似，在一个线程执行的时候另一个也不能执行***

***第二个任务***
***代码部分***

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5  

int buffer[BUFFER_SIZE]; 
int in = 0;  
int out = 0;  

pthread_mutex_t mutex;  
sem_t empty;  
sem_t full;    

// 生产者线程
void *producer(void *arg) {
    for (int i = 1; i <= 5; i++) {  
        sem_wait(&empty);  
        pthread_mutex_lock(&mutex);  

        buffer[in] = i;  
        printf("Producer → Inserted %d at slot %d\n", i, in);
        in = (in + 1) % BUFFER_SIZE;  

        pthread_mutex_unlock(&mutex);  
        sem_post(&full);  
    }
    return NULL;
}

// 消费者线程
void *consumer(void *arg) {
    for (int i = 1; i <= 5; i++) {  
        sem_wait(&full);  
        pthread_mutex_lock(&mutex);  

        int item = buffer[out];  
        printf("Consumer → Removed %d from slot %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;  

        pthread_mutex_unlock(&mutex);  
        sem_post(&empty);  
    }
    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread;

    // 初始化
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);  // 空 = 5
    sem_init(&full, 0, 0);             // 已填充 = 0

    // 创建
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

   
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}

***运行截图***

[![pEjPUAS.png](https://s21.ax1x.com/2025/05/13/pEjPUAS.png)](https://imgse.com/i/pEjPUAS)

***需要两个信号量和一个互斥锁，先弄一个缓冲区，本代码设置为5，即生产者最多生产5个（如果消费者不消费）。在生产者代码部分添加empty信号量来确保类似第一个任务第一个代码的问题不会发生（即两个生产线程同时执行）；同理，消费者线程部分也有full信号量确保不会发生错误。互斥锁来保证in，out和缓冲区不会发生错误。***

***第三个任务中，我着重第二个代码的exit(42)语句。此语句是进程级别的退出，包括所有线程，i的值也不会被赋为42；只有如第三个任务第一个代码的pthread_exit((void *)42)才能使线程正常退出***