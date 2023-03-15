#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdarg.h>
#include <semaphore.h>
#include "sem.h"
#define c 4
#define p 23
#define cap 4

Semaphore sem_load;
Semaphore sem_pass;
Semaphore sem_car;
int boarders = 0;
int pas = 0;
int car_num;
int a[cap];

void *car(void *args)
{
    int num = *(int *)args;
    while (1)
    {
        P(&sem_car, num);
        printf("car %d is ready to load.\n", num);
        car_num = num;
        V(&sem_load);
    }
}

void *passenger(void *args)
{
    int nn = *(int*)args;
    P(&sem_pass, nn);
    printf("passenger %d is waiting in boarding queue.\n", nn);
    a[boarders] = nn;
    boarders += 1;
    if (boarders == cap)
    {
        P(&sem_load, nn);
        printf("passengers %d, ", a[0]);
        for (int i = 1; i < cap; i++)
        {
            printf("%d, ", a[i]);
        }
        printf("has boarded the car %d.\n", (car_num));
        boarders = 0;
        printf("passengers %d, ", a[0]);
        for (int i = 1; i < cap; i++)
        {
            printf("%d, ", a[i]);
        }
        printf("has unboarded the car %d.\n", (car_num));
        V(&sem_car);
    }
    V(&sem_pass);
    return NULL;
}
int main()
{
    pthread_t ca[c], pa[p];
    Semaphore_init(&sem_load, 0);
    Semaphore_init(&sem_pass, 1);
    Semaphore_init(&sem_car, 1);
    for (int i = 0; i < c; i++)
    {
        get_thread_id(&sem_load, i);
        get_thread_id(&sem_pass, i);
        get_thread_id(&sem_car, i);
        int *a = (int*)malloc(sizeof(int));
        *a = i;
        pthread_create(&ca[i], NULL, car, a);
    }
    for (int i = 0; i < p; i++)
    {
        get_thread_id(&sem_load, i);
        get_thread_id(&sem_pass, i);
        get_thread_id(&sem_car, i);
        int* b = (int*)malloc(sizeof(int));
        *b = i;
        (pthread_create(&pa[i], NULL, passenger, b));
    }
    for (int i = 0; i < p; i++)
    {
        pthread_join(pa[i], NULL);
    }

    return 0;
}