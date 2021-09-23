#ifndef __POOL__
#define __POOL__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>
#include <semaphore.h>
typedef void* (*function_t)(void* arg);
#define gettid() syscall(__NR_gettid)

typedef struct Jobnode{
	function_t pf;
	void* arg;
}Jobnode;

typedef struct threadjob{
	Jobnode data;
	struct threadjob* next;
}threadjob;

typedef struct threadpool{
	threadjob* poolhead;
	int Maxthread;		//the Max number of thread we can create
	int jobnum;			//the working number 
	pthread_t * threads;
	int flag;
	sem_t sem;
}threadpool;

//operation of threadpool
threadpool* Pool_init(int Maxthread);		//init the threadpool

int Delete_pool(threadpool* pool);					//Delete the threadpool

int Add_job(threadpool* pool , function_t pf , void* arg); //add job to the threadpool

void Job_running(threadpool* pool);
//

// operation of job queue
Jobnode Pop(threadpool* pool);

int Push(threadpool* pool , Jobnode data );

int Delete(threadpool* pool);
//

#endif