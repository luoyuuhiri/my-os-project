#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* print_id(void* arg) {
    long thread_id = (long)arg;
    pthread_mutex_lock(&mutex); // 加锁防止输出混乱
    printf("Hello from thread %ld\n", thread_id);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    pthread_t thread1, thread2;
    long id1 = 1, id2 = 2;

    // 创建两个线程
    pthread_create(&thread1, NULL, print_id, (void*)id1);
    pthread_create(&thread2, NULL, print_id, (void*)id2);

    // 等待线程结束
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("All threads completed.\n");
    return 0;

}