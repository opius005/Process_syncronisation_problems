#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdarg.h>
#include <unistd.h>
#define n 20
pthread_t smokers[n];
pthread_t agent;
int ing[n];
int agentsem;
int smoke_complete = 1;
void semwait(int *i)
{
    int num = *i;
    while (num <= 0)
    {
    }
    num--;
}
void semsignal(int *i)
{
    int num = *i;
    num = 1;
}
void *smoker(void *arg)
{
    int i = *(int*)arg;
    while (1)
    {

        if (ing[i] == 1)
        {
            semwait(&agentsem);
            printf("smoker %d is smoking\n", i);
            sleep(2);
            ing[i] = 0;
            semsignal(&agentsem);
            semsignal(&smoke_complete);
        }
    }
}
void *agency(void*args)
{
    while (1)
    {
        semwait(&smoke_complete);
        semwait(&agentsem);
        int number = rand() % n;
        ing[number] = 1;
        semsignal(&agentsem);
        sleep(1);
    }
}

int main()
{
    agentsem = 1;
    for (int i = 0; i < n; i++)
    {
        ing[i] = 0;
    }
    pthread_create(&agent, NULL, &agency, NULL);
    for (int i = 0; i < n; i++)
    {
        int *a =(int*) malloc(sizeof(int));
        *a = i;
        pthread_create(smokers + i, NULL, &smoker, a);
    }
    for (int i = 0; i < n; i++)
    {
        pthread_join(smokers[i], NULL);
    }
    pthread_join(agent, NULL);
}