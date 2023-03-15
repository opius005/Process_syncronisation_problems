# Process_syncronisation_problems

readers-writers-using-pthreads
## Problem Statement:
Reader-Writer Problem, pertains to any situation where a data structure, database, or file system is read and modified by concurrent threads. While the data structure is being written or modified it is often necessary to bar other threads from reading, in order to prevent a reader from interrupting a modification in progress and reading inconsistent or invalid data.The synchronization constraints are:
1. Any number of readers can be in the critical section simultaneously.
2. Writers must have exclusive access to the critical section.

In other words, a writer cannot enter the critical section while any other thread (reader or writer) is there, and while the writer is there, no other thread may enter.
## Solution:
1. Semaphore_mut(To make reader processes those which entered the queue after writer processes wait).
2. Semaphore_rd (Mututal exclusion for  readers count).
3. Semaphore_wrt (To make writer wait while reading).\
All these semaphores are intialized to 1 at the start.\
There is an integer variable(readerscount) intialized to 0 at the start.\
There is a integer type common_variable which is being read by reader() process and incremented by writer() process. common_variable is initialised to 0 at the start.

Consider the following threads to be created : 
rrrwwrrrrw (r- reader, w- writer).

>Let's assume that reader first enters critical section making the value of readerscount 1 and so makes semaphore_wrt to 0 so that writer   doesn't access the            critical section.\
>Multiple readers may enter critical section as semaphore_mut and semaphore_rd are set to 1 after incrementing readers_count.\
>When a writer thread arrives while reader is in critical section, writer cannot enter critical section as semaphore_wrt is set to 0 when the value of                    readerscount became 1.\
>When all the readers in critical section exit, value of readerscount is made to 0 and so semaphore_wrt is set to 1,so that writer can enter critical section.\
>when writer process is in critical section, the reader processes which come after the writer process will be stopped as semaphore_mut is set to 0 by the writer    thread.This ensures the code to be starve free.\
>When writer is in critical section then remaining readers and writers have equal chance to enter the critical section when the writer exits from the critical section.\
>If we start with writer in critical section first,it will be executed from the state mentioned in point 5.

#cigarrette_smokers_problem
In this cigarette smokers problem there are three smokers , one Agent. Agent acts like an Operating System and manages the resources./

For making a cigarette a smoker requires three ingredients , tobacco , matches and paper. A smoker can only make a cigarette and smoke if he/she has all the three ingredients with them. There will be three smokers will be sitting around a table each with an infinite supply of one of the ingredients . A non smoking Agent seperately , will be providing i.e will keep randomly two of the ingredients on the table.The smoker who has the third ingredient should remove the two items from the table,and using them (along with their own ingredient) to make a cigarette, which they smoke for a while. Once the smoker has finished his cigarette, the agent places two new random ingredients on the table. This process continues forever./

For a generalized n smokers problem their will be n ingredients and n-1 items will kept on the table./

#Approach for generalised n smokers:/

First create n threads for each smoker. And also a thread for the Agent.I defined two functions semwait and semsignal which works as V and P operations./

I used two functions to create and join threads./

pthread_create() - is a function used to create threads. It requires four parameters: pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);/

pthread_join(): is also a function that shall suspend execution of the calling thread until the target thread terminates, unless the target thread has already terminated. pthread_join(pthread_t thread, void **value_ptr);/

I defined an array of n ingredients with initial value 0 in all . And a variable agentsem and smoke_complete which acts as semaphores for signalling to agent to provide ingredients and signal that smoke is complete./

Two void pointer functions for smoker and agent./

Basically the approach is that, the smoker with the ingredient which is not present on the tabke should pick up all the items . so remaining smokers shoul wait untill this smoker pickups all items and make the cigarette and signal the agent to again keep the items on the table. so in the array of ingredients , the ingredient which is not present on table is set to 1. And the smoker with this index takes all the items and smoke and again that ingredient is set to 0 ./

#Dining_philosophers_problem

#Problem statement

This problem is an extension of the standard dining philosophers problem which has 5 philosophers but here we can have n number of philosophers.\There are n philosophers and n chopsticks present in our problem.\They are placed in such a circular way such that each philosopher has one chopstick on both left and right sides of him.\Philosophers become hungry randomly and need both the chopsticks to eat the noodles in the bowl present in front of him.\ The philosopher can only eat if both the left and right chopsticks are available.\Assume that noodles are always present whenever philosopher is hungry.\ We need to write code with certain restrictions such that the solution is starve-free i.e no deadlock and no starvation.\

#Solution

We can put restriction to any one of the philosopher that he should pick up the right chopstick first, then the left chopstick, whenever he is hungry.\Rest of the philosophers will be picking up the left chopstick first, then the right chopstick, whenever he is hungry.\ This ensures that there is no deadlock because in the normal solution every person picks up either left and then right or\right and then left chopstick whenever he is hungry.\But here the circular chain breaks because one philosopher is picking up the chopsticks in reverse order than the other philosophers.\Here we are taking that one different philosopher to be the last philosopher i.e nth philosopher. As we know that every philosopher will complete eating in a bounded time, so every philosopher can only\ be waiting for bounded time and not for infinite time. \Let's suppose every philosopher gets hungry at the same time, all of them start picking up the left chopsticks and wait because their right\
chopsticks have been picked up by their neighbour philosopher.\ But the nth(last philosopher) has reverse order picking and doesn't have the right chopstick empty because the first philosopher has\
already picked it up. He also doesn't pick up the left chopstick because he has not yet picked up the right one and as per the restriction,\ he has to pick right one first and then only left one.
So the n-1 th philospher starts eating first and so after he has completed eating then n-2 th\ philosopher and so on every one can complete eating without leading to deadlock. So there would be no\ deadlock due to breaking the circular chain and no starvation due to bounded time of eating leading to a starve free solution.\

#Builduing H2O Problem<br>
There are two kinds of threads, oxygen and hydrogen. In order to assemble these threads into water molecules, we have to create a barrier that makes each thread wait until a complete molecule is ready to proceed.<br>
As each thread passes the barrier, it should invoke bond. You must guarantee that all the threads from one molecule invoke bond before any of the threads from the next molecule do.<br>
In other words:<br>
If an oxygen thread arrives at the barrier when no hydrogen threads are present, it has to wait for two hydrogen threads.<br>
If a hydrogen thread arrives at the barrier when no other threads are present, it has to wait for an oxygen thread and another hydrogen thread.<br>
#Solution<br>

oxygen_count and hydrogen_count are counters to track oxygen and hydrogen atoms, protected by mut.<br>
barrier is where
each set of three threads meets after invoking bond and before allowing the next
set of threads to proceed.<br>
oxygen_queue is the semaphore oxygen threads wait on<br>
hydrogen_queue is the semaphore oxygen threads wait on<br>

Initially hydrogen_queue and oxygen_queue are locked. When an oxygen thread arrives
it signals hydroQueue twice, allowing two hydrogens to proceed. Then the
oxygen thread waits for the hydrogen threads to arrive.<br>
As each oxygen thread enters, it gets the mutex and checks the scoreboard.
If there are at least two hydrogen threads waiting, it signals two of them and
itself and then bonds. If not, it releases the mutex and waits.<br>
After bonding, threads wait at the barrier until all three threads have
bonded, and then the oxygen thread releases the mutex. Since there is only
one oxygen thread in each set of three, we are guaranteed to signal mutex once.<br>

##Starve-free Multi-car Roller Coaster problem<br>

Suppose there are n passenger threads and m car threads. The
passengers repeatedly wait to take rides in the car, which can hold
C passengers, where C < n. The car can go around the tracks only
when it is full.<br>
<br>
Here are some additional details:<br>
Passengers cannot board until the car has invoked load.<br>
The car cannot depart until C passengers have boarded.<br>
Only one car can be boarding at a time.<br>
Multiple cars can be on the track concurrently.<br>
Since cars can’t pass each other, they have to unload in the same order they boarded.<br>
All the threads from one carload must disembark before any of the threads from subsequent carloads.<br>
<br>
#Solution<br>

We will make use of three semaphores(sem_car,sem_pass,sem_load), two integer variables(,car_num,boarders) and an integer array(a[]).<br>
1)sem_car, a semaphore(initialised to 1) which is used to ensure mutual exclusion of car threads.<br>
2)sem_pass, a semaphore(initialised to 1) which is used to ensure mutual exclusion of passenger threads.<br>
3)sem_load, a semaphore(initialised to 0) which is used to ensure that passengers board the car only after the car is ready to load.<br>
4)boarders, an integer variable(initialised to 0) that keeps track of number of boarders, once its value equals capacity, passengers are made to board the car and again boarders value is made zero.<br>
5)car_num, an integer variable which takes the value of index of car thread.<br> 
6)a[], an integer array which is used to store index of passenger threads that are about to board.<br>
7)c,p,cap are defined which give number of cars,total number of passengers, capacity of each car respectively.<br>
<br>

Initially,<br>
when a car thread executes, semaphore sem_car is used to make sure that only one car is on the track and other cars which request to access the critical section are made to wait in the queue in the order of thier requests(ensuring no starvation), semaphore sem_load is used to indicate that car is ready is load.<br>
when a passenger thread executes, semaphore sem_pass is used make sure that other passengers who request to access the critical section are made to wait in the queue in the order of their requests(ensuring no starvation), whenever boarders value equals cap, they are made to board the car that is ready to load.<br>

#Producer_consumer_problem
Problem statement-The producer and consumer must be synchronized so that the consumer does not try to consume an item that has not yet been produced.\
There are two types of buffers :\
Unbounded buffer: It has no limit on the size of the buffer. The consumer may have to wait for new items, but the producer can always procure new items. \
Bounded buffer: It assumes a fixed buffer size. The consumer must wait if the buffer is empty, and the producer must wait if the buffer is full.\
Cyclic Buffer: Like the bounded buffer, it is also of fixed size. Here, items are pushed and consumed from the buffer from a range of indices. Produced items are pushed into the buffer in cyclic order.\
\
we used three binary semaphores to achieve this\
They are:\
a)lock1-This semaphore is to ensure the mutual exclusion between producer and consumer working on the buffer simultaneously.\
b)lock2-This ensures the mutual exclusion among the consumer threads waiting for fil to be greater than zero.\
c)lock3-This ensures the mutual exclusion among the producer threads waiting for emp to become greater than zero(only for bounded and cyclic).\
\
variables used:
a)fil-no of elements filled
b)emp-no of elements empty
\
1.Unbounded buffer:\
There are 150 producers and 150 consumers.\
producers will be pushing the integer j into the queue where j is the number of producers starting from 1.\
Consumer threads are created first, and they will be waiting for the buffer to become non-empty.\
As soon as access to the buffer is granted, they start executing.\
consumer thread will be executed only when the buffer is non-empty.\
This is acheived by using a semaphore to access the queue either for producing or consuming an item from the buffer.\
Access for others will be denied when a producer or consumer is already using the buffer.\
\
2.Bounded buffer:\
There are 150 producers and 150 consumers.\
producers will be pushing the integer j into the queue where j is the number of producer starting from 1.\
Consumer threads are created first and they will be waiting for the buffer to become non-empty.\
As soon as the access to the buffer is granted, they start executing.\
consumer thread will be executed only when the buffer is non-empty.\
producer thread will be executed only when the number of items in the buffer is less than or equal to the maximum limit.\
This is acheived by using a semaphore to access the queue either for producing or consuming an item from the buffer.\
Access for others will be denied when a producer or a consumer is already using the buffer.\
\
3.Cyclic buffer:\
There are 150 producers and 150 consumers.\
producers will be pushing the items into an array cyclically.consumers will also be consuming the items in the order in which producers are producing\
To acheive this, we are maintaining two pointers within the array.one for the index at which the items are getting consumed and one at which the items are getting produced.\
As the item at a particular index is produced/consumed, their respective pointers are moved to the next index(to the first if initially at last index).\

#sleeping_barber_problem
A barbershop consists of a barber room with b barbers and b barber chairs and a waiting room with n chairs for waiting customers. The rules are;<br>
1)if there are no customers,then barbers sleep in their own chairs.<br>
2)If a customer arrives then he must wake the barber if he is asleep.<br>
3)If a customer arrives while barbers are working then he checks number of freeseats in the waiting room.<br>
4)If all the seats are occupied then he leaves the barbershop and again arrives after some time, otherwise he sits in an empty chair.<br>
5)When a barber finishes a haircut he checks the waiting room to see if there are any waiting customers and falls asleep if there are no waiting customers.<br>
We have written a deadlock free and starve free solution in c++ language.We have taken semaphores;<br>
seat_mutex(to update freeseats),//Number of freeseats in waiting room<br>
 sit_mut(to update sit variable),//Sit variable is the number of the chair where the customer sits<br>
cut_mut(to update cut variable),//Cut variable is the number of the chair which a barber selects<br>
 cus_mutex and cus2_mutex(to maintain mutual exclusion between customers).<br>
bar_mutex and bar2_mutex(to maintain mutual exclusion between barbers).<br>
This is a starve free solution as we have maintained an array to maintain fifo order of customers.<br>
