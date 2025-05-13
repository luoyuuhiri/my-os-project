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