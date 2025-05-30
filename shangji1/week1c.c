#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


pthread_mutex_t mutex;
int sum = 0;
sem_t sem;

void *thread(){
    int i;
    for(i = 0; i < 1000000; i++)
    {
        sem_wait(&sem);
        sum += 1;
        sem_post(&sem);
    }
}

int main(void)
{
    pthread_t tid1, tid2;
    sem_init(&sem, 0 ,1);

    pthread_create(&tid1, NULL, thread, NULL);
    pthread_create(&tid2, NULL, thread, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    printf("1000000 + 100000 = %d\n", sum);
    return (0);
}