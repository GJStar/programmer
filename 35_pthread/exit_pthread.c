#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <error.h>

/*
thread三种退出方式：
1. 线程return返回
2. 线程使用pthread_exit主动退出
3. 其他线程使用pthread_cancel杀死thread
*/
void *thr_fn1(void *test)
{
	printf("thread 1 returning\n");
	return (void *)1;
}

void *thr_fn2(void *arg)
{
	printf("thread 2 exiting\n");
	pthread_exit((void *)2);
}

void *thr_fn3(void *arg)
{
	while(1) {
		printf("thread 3 writing\n");
		sleep(1);
	}
}

int main(void)
{
	pthread_t tid;
	void *tret;

	pthread_create(&tid, NULL, thr_fn1, NULL);
	pthread_join(tid, &tret);
	printf("thread 1 exit code:%ld\n", (long int)tret);

	pthread_create(&tid, NULL, thr_fn2, NULL);
	pthread_join(tid, &tret);
	printf("thread 2 exit code:%ld\n", (long int)tret);

	pthread_create(&tid, NULL, thr_fn3, NULL);
	sleep(3);
	pthread_cancel(tid);
	pthread_join(tid, &tret);
	printf("thread 3 exit code:%ld\n", (long int)tret);

	return 0;
}
