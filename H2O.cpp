#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include "sem.h"
using namespace std;

#define b 3
#define a 1
#define n 50
#define time 5

struct Semaphore mut;
struct Semaphore barrier;
struct Semaphore oxygen_queue;
struct Semaphore hydrogen_queue;
int oxygen_count = 0;
int hydrogen_count = 0;

void bond(int index)
{
    printf("Bonding in process for molecule-(%d). \n", index);
    //sleep(time - 2);
    //printf("Bonding completed for molecule-(%d). \n", index);
}

void *hydrogen(void *arg)
{
    int index = *(int *)arg;
    
    sleep(time);
    //printf("a hydrogen arrived. \n");
    P(&mut,index);
    hydrogen_count++;
    if (hydrogen_count >= 2 && oxygen_count >= 1)
    {
        V(&hydrogen_queue);
        V(&hydrogen_queue);
        hydrogen_count -= 2;
        V(&oxygen_queue);
        oxygen_count--;
    }
    else
        V(&mut);

    P(&hydrogen_queue,index);
    P(&barrier,index);
    //bond(index);
    V(&barrier);
    //sleep(time + 2);
   // printf("leaving hydrogen. \n");
    free(arg);
    return NULL;
}

void *oxygen(void *arg)
{
    int index = *(int *)arg;
    sleep(time);
    P(&mut,index);
    //printf("a oxygen arrived. \n");
    oxygen_count++;
    if (hydrogen_count >= 2)
    {
        V(&hydrogen_queue);
        V(&hydrogen_queue);
        hydrogen_count -= 2;
        V(&oxygen_queue);
        oxygen_count--;
    }
    else
        V(&mut);

    P(&oxygen_queue,index);
    P(&barrier,index);
    bond(index+1);
    V(&barrier);
    V(&mut);
    //sleep(time + 2);
    //printf("leaving oxygen. \n");
    free(arg);
    return NULL;
}

int main()
{
    pthread_t H[2 * n], O[n];
    printf("hello. \n");
    Semaphore_init(&mut,  a);
    Semaphore_init(&barrier,  b);
    Semaphore_init(&hydrogen_queue,  0);
    Semaphore_init(&oxygen_queue,  0);
    
    for (int i = 0; i < 2 * n; i++)
    {
        int *index = (int *)malloc(sizeof(int));
        *index = i;
        if (pthread_create(H + i, NULL, &hydrogen, index))
        {
            
            printf("Error in creating Hydrogen threads. \n");
        }
    }
    for (int i = 0; i < n; i++)
    {
        int *index = (int *)malloc(sizeof(int));
        *index = i;
        if (pthread_create(O + i, NULL, &oxygen, index))
        {
            printf("Error in creating Oxygen threads. \n");
        }
    }
    for (int i = 0; i < 2 * n; i++)
    {
        if (pthread_join(H[i], NULL))
        {
            printf("Error in joining Hydrogen threads. \n");
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (pthread_join(O[i], NULL))
        {
            printf("Error in joining Oxygen threads. \n");
        }
    }
    
    //sleep(time);
    printf("byee. \n");
}