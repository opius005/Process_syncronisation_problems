#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<unistd.h>
#define n 20
#define THINKING 2
#define HUNGRY 1
#define EATING 0
int chopsticks[n];
pthread_t tid[n];
int state[n];

void semwait(int i)
{
    if (i <= 0)
    {
        while (1)
        {
        }
    }
    else
    {
        i--;
    }
}
void semsignal(int i)
{
    i++;
}

void *left(void *i)
{
    int index = *(int*)i;
    int l = (index + 1) % n;
    if (index != n - 1)
    {
        while (1)
        {
            semwait(chopsticks[index]);
            semwait(chopsticks[l]);
            printf("%d is eating\n", index);
            semsignal(chopsticks[l]);
            semsignal(chopsticks[index]);
            sleep(2);
        }
    }
    else
    {
        while (1)
        {
            semwait(chopsticks[l]);
            semwait(chopsticks[index]);
            printf("%d is eating\n", index);
            semsignal(chopsticks[index]);
            semsignal(chopsticks[l]);
            sleep(2);
        }
    }
}
int main()
{
    for (int i = 0; i < n; i++)
    {
        chopsticks[i] = 1;
        state[i] = HUNGRY;
    }

    for (int i = 0; i < n; i++)
    {
        pthread_create(tid + i, NULL, &left, (void *)i);
    }
    for (int i = 0; i < n; i++)
    {
        pthread_join(tid[i], NULL);
    }
    return 0;
}