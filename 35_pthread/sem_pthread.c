#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM 5
int queue[NUM];
sem_t blank, product;

void *consumer(void *ptr)
{
	int index = 0;
	for (; ;) {
		sem_wait(&product);
		printf("consume %d\n", queue[index]);
		sem_post(&blank);
		index = (index + 1) % NUM;
		sleep(rand() % 5);
	}
}

void *producer(void *ptr)
{
	int index = 0;
	for (; ;) {
		sem_wait(&blank);
		queue[index] = rand() % 100;
		printf("produce %d\n", queue[index]);
		sem_post(&product);
		index = (index + 1) % NUM;
		sleep(rand() % 5);
	}
}

int main()
{
	pthread_t pid, cid;
	sem_init(&blank, 0, NUM);
	sem_init(&product, 0, 0);

	pthread_create(&pid, NULL, producer, NULL);
	pthread_create(&cid, NULL, consumer, NULL);
	pthread_join(pid, NULL);
	pthread_join(cid, NULL);

	sem_destroy(&blank);
	sem_destroy(&product);
	return 0;
}
