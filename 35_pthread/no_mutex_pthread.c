#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define LOOP 1000
int g_counter = 0;

void *cnt(void *vptr)
{
	int val;
	for (int i = 0; i < LOOP; i++) {
		val = g_counter;
		usleep(1);
		printf("%s: %llx: %d\n", (char *)vptr, (unsigned long long)pthread_self(), val + 1);
		g_counter = val + 1;
	}
}

int main()
{
	pthread_t tid1, tid2;

	pthread_create(&tid1, NULL, &cnt, "tid1");
	pthread_create(&tid2, NULL, &cnt, "tid2");

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	printf("counter is :%d\n", g_counter);

	return 0;
}