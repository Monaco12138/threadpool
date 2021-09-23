#include "threadpool.h"

threadpool* Pool_init(int Maxthread)
{
	threadpool* pool;
	pool = (threadpool*)malloc(sizeof(threadpool));
	pool->flag = 1;
	sem_init(&(pool->sem) , 0 , 1);
	//
	pool->poolhead = (threadjob*)malloc( sizeof(threadjob) );
	pool->poolhead->next = NULL;
	//
	pool->Maxthread = Maxthread;
	pool->jobnum = 0;
	pool->threads = (pthread_t*)malloc( sizeof(pthread_t) * Maxthread ); //create max num of threads;

	int i=0;
	for(; i < Maxthread ; i++){
		pthread_create(&pool->threads[i] , NULL , (void*)Job_running , (void*)pool);
	}
	return pool;
}

void Job_running(threadpool* pool)
{
	while(pool->flag){
		if(pool->jobnum > 0){
			sem_wait(&(pool->sem));
			Jobnode job;
			job = Pop(pool);
			sem_post(&(pool->sem));
			function_t pf = job.pf;
			void* arg  = job.arg;
			pf(arg);		//exce the job
		}
	}
	pthread_exit(0);
}

Jobnode Pop(threadpool* pool)
{
	threadjob* phead = pool->poolhead;
	threadjob* p1 = phead;
	threadjob* q2 = phead->next;
	p1->next = q2->next;
	Jobnode data = q2->data;
	free(q2);
	(pool->jobnum)--;
	return data;
}

int Add_job(threadpool* pool , function_t pf , void* arg)
{
	Jobnode data;
	data.pf = pf;
	data.arg = arg;
	sem_wait(&(pool->sem));
	Push(pool , data);
	sem_post(&(pool->sem));
	return 1;
}

int Push(threadpool* pool , Jobnode data )
{
	threadjob* phead = pool->poolhead;
	threadjob* p1 = phead;
	for(; p1->next;){
		p1 = p1->next;
	}
	threadjob* temp = (threadjob*)malloc( sizeof(threadjob) );
	temp->data = data;
	p1->next = temp;
	temp->next = NULL;
	(pool->jobnum)++;
	return 1;
}

int Delete_pool(threadpool* pool)
{
	//sem_wait(&(pool->sem));
	pool->flag = 0;
	//sem_post(&(pool->sem));
	int i=0; 
	for(; i< pool->Maxthread; i++) {
		pthread_join(pool->threads[i] , NULL);		//wait for all the threads
	}
//	pool->flag = 0;
	Delete(pool);
	pool->jobnum = 0;
	free(pool->threads);
	free(pool);
	return 1;
}

int Delete(threadpool* pool)
{
	threadjob* p1 = pool->poolhead;
	threadjob* q2;
	for(; p1 ;p1=q2){
		q2 = p1->next;
		free(p1);
	}
	return 1;
}