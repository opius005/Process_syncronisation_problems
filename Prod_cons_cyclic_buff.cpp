#include <pthread.h>
#include <bits/stdc++.h>
using namespace std;

int prod = 150;
int con = 150;
int buff[5];
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;
int fil = 0;
int emp = 5;
int prod_index = 0;
int cons_index = 0;
int val = 0;

void *produce(void *args)
{
    pthread_mutex_lock(&lock3);
    while (emp <= 0)
        ;
    emp--;
    pthread_mutex_unlock(&lock3);
    pthread_mutex_lock(&lock1);
    // pos++;
    // pos=pos%5;
    val++;
    buff[prod_index] = val;
    fil++;
    cout << "produced " << val << " at index: " << prod_index << endl;
    prod_index++;
    prod_index %= 5;
    pthread_mutex_unlock(&lock1);
    return NULL;
}

void *consume(void *args)
{
    pthread_mutex_lock(&lock2);
    while (fil <= 0)
        ;
    fil--;
    pthread_mutex_unlock(&lock2);
    pthread_mutex_lock(&lock1);
    cout << "__consumed " << buff[cons_index] << " form index: " << cons_index << endl;
    cons_index += 11;
    cons_index %= 5;
    emp++;
    pthread_mutex_unlock(&lock1);
    return NULL;
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
