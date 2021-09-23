#include "threadpool.h"
void task1(void)
{
	int sum=0,i;
	for(i=0; i<100000000; i++){
		sum += i;
	}
	printf("this is the task1 running in %ld\n",gettid());
}

void task2(void)
{
	int sum=0,i;
	for(i=0; i<100000000; i++){
		sum = i + sum;
	}
	printf("this is the task2 running in%ld\n",gettid());
}
int main()
{
	threadpool* pool = Pool_init(10);
	int i=0;
	for(; i<100; i++){
		Add_job(pool ,(void*)task1 , NULL );
		Add_job(pool , (void*)task2 , NULL);
	}
	sleep(10);
	Delete_pool(pool);
	return 0;
}