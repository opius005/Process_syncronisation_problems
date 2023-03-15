#include <iostream>
#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "sem.h"
#define b 3
#define c 12
#define n 4
#define time 13
#define ctime 2
#define rtime 15
using namespace std;
int freeseats = n;
// sem_t bar;
// sem_t cus;
struct Semaphore seat_mutex ;
struct Semaphore sit_mut ;
struct Semaphore cut_mut ;
struct Semaphore cus_mutex ;
struct Semaphore bar_mutex ;
struct Semaphore bar2_mutex ;
struct Semaphore cus2_mutex ;
int seats[n];
int sit = 0;
int cut = 0;
int bar = b;
int cus = 0;
void *customer(void *arg)
{
    int l=*(int*)arg;
    P(&seat_mutex,l);
    P(&sit_mut,l);
    if (freeseats > 0)
    {
        printf("Customer %d has entered.\n", *(int*)arg + 1);
        freeseats--;
        V(&seat_mutex);
        seats[sit % n] = *(int *)arg + 1;
        printf("Customer %d sits on the seat %d.\n", *(int *)arg + 1, sit % n + 1);
        sit++;
        V(&sit_mut);
        //  sem_post(&cus);
        P(&cus2_mutex,l);
        cus++;
        V(&cus2_mutex);
        //  sem_wait(&bar);
        P(&bar2_mutex,l);
        while (bar <= 0)
            ;
        bar--;
        V(&bar2_mutex);
    }
    else
    {
        printf("Customer %d has entered.\n", *(int *)arg + 1);
        V(&seat_mutex);
        V(&sit_mut);
        cout << "Customer " << *(int *)arg + 1 << " leaves the shop as no seats are available" << endl;
        sleep(rtime);
        customer(arg);
    }
    return NULL;
}
void *barber(void *arg)
{
    int l = *(int *)arg;
    while (c)
    {
        P(&cus_mutex,l);
        if (cus <= 0)
            printf("Barber %d sleeps.\n", *(int*)arg + 1);
        while (cus <= 0)
            ;
        printf("Barber %d wakes.\n", *(int*)arg + 1);
        cus--;
        V(&cus_mutex);
        //   sem_wait(&cus);
        P(&seat_mutex,l);
        freeseats++;
        //   sem_post(&bar);
        P(&bar_mutex,l);
        bar++;
        V(&bar_mutex);
        P(&cut_mut,l);
        printf("Barber %d cuts hair of the customer %d.\n", *(int *)arg + 1, seats[cut % n]);
        cut++;
        V(&cut_mut);
        V(&seat_mutex);
        sleep(time);
    }
    free(arg);
}
int main()
{
    pthread_t ba[b], cu[c];
    Semaphore_init(&seat_mutex,1);
    Semaphore_init(&sit_mut,1);
    Semaphore_init(&cut_mut,1);
    Semaphore_init(&cus_mutex,1);
    Semaphore_init(&bar_mutex,1);
    Semaphore_init(&bar2_mutex,1);
    Semaphore_init(&cus2_mutex,1);

    cout << "Shop opens" << endl;
    for (int i = 0; i < b; i++)
    {
        int *index = (int *)malloc(sizeof(int));
        *index = i;
        get_thread_id(&seat_mutex,i);
        get_thread_id(&sit_mut, i);
        get_thread_id(&cut_mut, i);
        get_thread_id(&cus_mutex, i);
        get_thread_id(&bar_mutex, i);
        get_thread_id(&cus2_mutex, i);
        get_thread_id(&bar2_mutex, i);
        if (pthread_create(&ba[i], NULL, barber, index) != 0)
        {
            cout << "error" << endl;
        }
    }
    for (int i = 0; i < c; i++)
    {
        int *index = (int *)malloc(sizeof(int));
        *index = i;
        get_thread_id(&seat_mutex, i);
        get_thread_id(&sit_mut, i);
        get_thread_id(&cut_mut, i);
        get_thread_id(&cus_mutex, i);
        get_thread_id(&bar_mutex, i);
        get_thread_id(&cus2_mutex, i);
        get_thread_id(&bar2_mutex, i);
        if (pthread_create(&cu[i], NULL, customer, index) != 0)
        {
            cout << "error" << endl;
        }
        sleep(ctime);
    }
    for (int i = 0; i < c; i++)
    {
        if (pthread_join(cu[i], NULL) != 0)
        {
            cout << "error" << endl;
        }
    }
    sleep(time + 1);
    cout << "Shop closes" << endl;
    return 0;
}