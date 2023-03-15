#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
using namespace std;

#define b 3
#define a 1
#define n 50
#define time 5

sem_t mut;
sem_t barrier;
sem_t oxygen_queue;
sem_t hydrogen_queue;
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
    index = index / 2 + 1;
    
    sleep(time);
    //printf("a hydrogen arrived. \n");
    sem_wait(&mut);
    hydrogen_count++;
    if (hydrogen_count >= 2 && oxygen_count >= 1)
    {
        sem_post(&hydrogen_queue);
        sem_post(&hydrogen_queue);
        hydrogen_count -= 2;
        sem_post(&oxygen_queue);
        oxygen_count--;
    }
    else
        sem_post(&mut);

    sem_wait(&hydrogen_queue);
    sem_wait(&barrier);
    //bond(index);
    sem_post(&barrier);
    //sleep(time + 2);
   // printf("leaving hydrogen. \n");
    free(arg);
    return NULL;
}

void *oxygen(void *arg)
{
    int index = *(int *)arg;
    index = index + 1;
    sleep(time);
    sem_wait(&mut);
    //printf("a oxygen arrived. \n");
    oxygen_count++;
    if (hydrogen_count >= 2)
    {
        sem_post(&hydrogen_queue);
        sem_post(&hydrogen_queue);
        hydrogen_count -= 2;
        sem_post(&oxygen_queue);
        oxygen_count--;
    }
    else
        sem_post(&mut);

    sem_wait(&oxygen_queue);
    sem_wait(&barrier);
    bond(index);
    sem_post(&barrier);
    sem_post(&mut);
    //sleep(time + 2);
    //printf("leaving oxygen. \n");
    free(arg);
    return NULL;
}

int main()
{
    pthread_t H[2 * n], O[n];
    printf("hello. \n");
    sem_init(&mut, 0, a);
    sem_init(&barrier, 0, b);
    sem_init(&hydrogen_queue, 0, 0);
    sem_init(&oxygen_queue, 0, 0);
    
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
    sem_destroy(&mut);
    sem_destroy(&barrier);
    sem_destroy(&hydrogen_queue);
    sem_destroy(&oxygen_queue);
    //sleep(time);
    printf("byee. \n");
}