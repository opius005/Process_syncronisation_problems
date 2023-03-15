#include <bits/stdc++.h>
#include <unistd.h>
#include <stdlib.h>
#include <atomic>
using namespace std;
const int N=1e8;


void block(atomic<int> *spv)
{
    *spv = 0;
    while (*spv == 0)
        ;
}
void wakeup(atomic<int> *spv)
{
    *spv = 1;
}
struct Semaphore
{

    atomic<int> value;
    atomic<int> spv[N];
    // q contains all Process Control Blocks(PCBs)
    // corresponding to processes got blocked
    // while performing do+wn operation.
    queue<int> q;

    
};
void Semaphore_init(struct Semaphore *s, int init)
{
    (*s).value = init;
    
}
void get_thread_id(struct Semaphore *s,int tid)
{
    (*s).spv[tid] = 1;
}
void P(Semaphore (*s),int tid)
{
    (*s).value = (*s).value - 1;
    if ((*s).value < 0)
    {

        // add process to queue
        // here p is a process which is currently executing
        (*s).q.push(tid);
        block(&((*s).spv[tid]));
    }
     else
         return;
}

void V(Semaphore(*s))
{
    (*s).value = (*s).value + 1;
    if ((*s).value <= 0)
    {

        // remove process p from queue
        int tid=(*s).q.front();
        (*s).q.pop();
        wakeup(&(*s).spv[tid]);
    }
     else
         return;
}
