#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct msg_t {
	struct msg_t *next;
	int num;
} msg;

msg *g_head;
pthread_cond_t g_has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
pthread_cond_wait
1. 释放Mutex
2. 阻塞等待
3. 当被唤醒时，重新获得Mutex并返回
*/
void *consumer(void *p)
{
	msg *mp;
	for (;;) {
		pthread_mutex_lock(&g_mutex);
		while (g_head == NULL)
			pthread_cond_wait(&g_has_product, &g_mutex);
		mp = g_head;
		g_head = g_head->next;
		pthread_mutex_unlock(&g_mutex);
		printf("consume %d\n", mp->num);
		free(mp);
		sleep(rand() % 5);
	}
}

void *producer(void *p)
{
	msg *mp;
	for (;;) {
		mp = malloc(sizeof(msg));
		mp->num = rand() % 1000 + 1;
		printf("produce %d\n", mp->num);
		pthread_mutex_lock(&g_mutex);
		mp->next = g_head;
		g_head = mp;
		pthread_mutex_unlock(&g_mutex);
		pthread_cond_signal(&g_has_product);
		sleep(rand() % 5);
	}
}

int main()
{
	pthread_t pid, cid;
	srand(time(NULL));
	pthread_create(&pid, NULL, producer, NULL);
	pthread_create(&cid, NULL, consumer, NULL);
	
	pthread_join(pid, NULL);
	pthread_join(cid, NULL);
	return 0;
}
