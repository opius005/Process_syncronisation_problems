#include <pthread.h>
#include <bits/stdc++.h>
using namespace std;

int prod = 150;
int con = 150;
queue<int> buff;
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
int fil = 0;
int j = 0;

void *produce(void *args)
{
    pthread_mutex_lock(&lock1);
    j++;
    buff.push(j);
    fil++;
    cout << "produced " << j << endl;
    pthread_mutex_unlock(&lock1);
}

void *consume(void *args)
{
    pthread_mutex_lock(&lock2);
    while (fil <= 0)
        ;
    fil--;
    pthread_mutex_unlock(&lock2);
    pthread_mutex_lock(&lock1);
    cout << "__consumed " << buff.front() << endl;
    buff.pop();
    pthread_mutex_unlock(&lock1);
}

int main()
{
    pthread_t p[prod], c[con];
    for (int i = 0; i < con; i++)
    {
        if (pthread_create(&c[i], NULL, consume, NULL) != 0)
        {
            cout << "error" << endl;
        }
    }
    for (int i = 0; i < prod; i++)
    {
        if (pthread_create(&p[i], NULL, produce, NULL) != 0)
        {
            cout << "error" << endl;
        }
    }
    for (int i = 0; i < con; i++)
    {
        if (pthread_join(c[i], NULL) != 0)
        {
            cout << "error" << endl;
        }
    }
    for (int i = 0; i < prod; i++)
    {
        if (pthread_join(p[i], NULL) != 0)
        {
            cout << "error" << endl;
        }
    }
    return 0;
}
