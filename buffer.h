/*
	Name: Jaquelin Fraire
	Course: CS3502
	Assignment 2
*/
#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#define BUFFER_SIZE 10
#define SHM_KEY 0x1234
#define SEM_MUTEX "/sem_mutex"
#define SEM_EMPTY "/sem_empty"
#define SEM_FULL "/sem_full"

typedef struct {
	int value;
	int producer_id;
} item_t;

typedef struct {
	item_t items[BUFFER_SIZE];
	int in;
	int out;
} shared_buffer_t;

#endif
