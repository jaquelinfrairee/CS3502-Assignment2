/*
	Name: Jaquelin Fraire
	Course: CS3502
	Assignment 2
*/
#include "buffer.h"
#include <sys/shm.h>
#include <fcntl.h> 

shared_buffer_t* buffer = NULL;
sem_t* mutex = NULL;
sem_t* empty = NULL;
sem_t* full = NULL;
int shm_id = -1;

void cleanup() {
	if (buffer != NULL) {
		shmdt(buffer);
	}

	if (mutex != SEM_FAILED) sem_close(mutex);
	if (empty != SEM_FAILED) sem_close(empty);
	if (full != SEM_FAILED) sem_close(full);
}

void signal_handler(int sig) {
	printf("\nProducer: Caught signal %d, cleaning up...\n", sig);
	cleanup();
	exit(0);
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
	fprintf(stderr, "Usage: %s <producer_id> <num_items>\n", argv[0]);
	exit(1);
	}

	int producer_id = atoi(argv[1]);
	int num_items = atoi(argv[2]);

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	srand(time(NULL) + producer_id);

	shm_id = shmget(SHM_KEY, sizeof(shared_buffer_t), 0666 | IPC_CREAT);
if (shm_id == -1) {
	perror("Producer shmget failed");
	exit(1);
}

buffer = (shared_buffer_t*) shmat(shm_id, NULL, 0);

if (buffer == (void*) -1) {
	perror("Producer shmat failed");
	exit(1);
}

	mutex = sem_open(SEM_MUTEX, O_CREAT, 0666, 1);
empty = sem_open(SEM_EMPTY, O_CREAT, 0666, BUFFER_SIZE);
full = sem_open(SEM_FULL, O_CREAT, 0666, 0);

if (mutex == SEM_FAILED || empty == SEM_FAILED || full == SEM_FAILED) {
	perror("Producer sem_open failed");
	exit(1);
}

	printf("Producer %d: Starting to produce %d items\n", producer_id, num_items);

	for (int i = 0; i < num_items; i++) {
		item_t item;
		item.value = producer_id * 1000 + i;
		item.producer_id = producer_id;

	sem_wait(empty);

	sem_wait(mutex);

	buffer->items[buffer->in] = item;
buffer->in = (buffer->in + 1) % BUFFER_SIZE;

	printf("Producer %d: Produced value %d\n", producer_id, item.value);

	sem_post(mutex);

	sem_post(full);

	usleep(rand() % 100000);
	}

	printf("Producer %d: Finished producing %d items\n", producer_id, num_items);
	cleanup();
	return 0;
}
