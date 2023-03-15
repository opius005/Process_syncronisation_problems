#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "sem.h"
using namespace std;

struct Semaphore mut;
struct Semaphore rd;
struct Semaphore wrt;

int readerscount = 0;
int common_variable = 0;

void *reader(void *num)
{
	int index = *(int *)num;
	P(&mut, index);
	P(&rd, index);
	readerscount++;
	if (readerscount == 1)
	{
		P(&wrt, index);
	}
	V(&rd);
	V(&mut);

	// Critical Section

	printf("Reader-%d is reading , modifications = %d\n", index, common_variable);
	free(num);

	P(&rd, index);
	readerscount--;
	if (readerscount == 0)
	{
		V(&wrt);
	}
	V(&rd);

	return 0;
}

void *writer(void *num)
{
	int number = *(int*)num;
	P(&mut, number);
	P(&wrt, number);

	// Critical Section
	int index = *(int *)num;
	common_variable++;
	printf("writer-%d is writing , modifications = %d \n", index, common_variable);
	V(&wrt);
	V(&mut);

	return 0;
}

int main()
{
	string s;
	cin >> s;
	Semaphore_init(&rd, 1);
	Semaphore_init(&wrt, 1);
	Semaphore_init(&mut, 1);

	int nr = 0;
	int nw = 0;
	for (int i = 0; i < s.size(); i++)
	{

		if (s[i] == 'r')
			nr++;
		if (s[i] == 'w')
			nw++;
	}
	pthread_t r[nr], w[nw];
	int rc = 1;
	int wc = 1;
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == 'r')
		{
			int *a = (int *)malloc(sizeof(int));
			*a = rc;
			pthread_create(&r[rc], NULL, &reader, a);
			get_thread_id(&rd, i);
			get_thread_id(&wrt, i);
			get_thread_id(&mut, i);
			rc++;
		}
		if (s[i] == 'w')
		{
			int *b = (int *)malloc(sizeof(int));
			*b = wc;
			pthread_create(&w[wc], NULL, &writer, b);
			get_thread_id(&rd, i);
			get_thread_id(&wrt, i);
			get_thread_id(&mut, i);
			wc++;
		}
	}
	rc = wc = 1;
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == 'r')
		{
			pthread_join(r[rc], NULL);
			rc++;
		}
		if (s[i] == 'w')
		{
			pthread_join(w[wc], NULL);
			wc++;
		}
	}
}