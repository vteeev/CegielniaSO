#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include "operacje.h"


int create_sem(key_t key) {
	int sem_id = semget(key, 1, IPC_CREAT|0600);
	if (sem_id == -1) {
		perror("blad w semget()");
		exit(1);
	}

	if (semctl(sem_id, SEM_MUTEX_TABLES_DATA, SETVAL, 1) == -1) {
		perror("blad w semctl() [inicjalizacja semaforow]");
		exit(1);
	}
	
	return sem_id;
}

int create_shm(key_t key, size_t size) {
	int shm_id = shmget(key, size, IPC_CREAT|0600);
	if (shm_id == -1) {
		perror("blad w shmget()");
	}

	return shm_id;
}

int create_msg(key_t key) {
	int msg_id = msgget(key, IPC_CREAT|0600);
	if (msg_id == -1) {
		perror("blad w msgget()");
		exit(1);
	}

	return msg_id;
}

void remove_sem(int sem_id) {
	if (semctl(sem_id, 0, IPC_RMID) == -1) {
		perror("blad w semctl() [usuwanie semaforow]");
	}
}

void remove_shm(int shm_id, void* addr) {
	if (shmdt(addr) == -1) {
		perror("blad w shmdt()");
	}

	if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
		perror("blad w shmctl() [usuwanie pamieci dzielonej]");
	}
}

void remove_msg(int msg_id) {
	if (msgctl(msg_id, IPC_RMID, NULL) == -1)
		perror("blad w msgctl() [usuwanie kolejki komunikatow]");
}

int join_sem(key_t key) {
	int sem_id = semget(key, 0, 0);
	if (sem_id == -1) {
		perror("blad w semget() [dolaczanie do zbioru semaforow]");
		exit(1);
	}

	return sem_id;
}

int join_shm(key_t key) {
	int shm_id = shmget(key, 0, 0);
	if (shm_id == -1) {
		perror("blad w shmget() [dolaczanie do pamieci dzielonej]");
		exit(1);
	}

	return shm_id;
}

int join_msg(key_t key) {
	int msg_id = msgget(key, 0);
	if (msg_id == -1) {
		perror("blad w msgget() [dolaczanie do kolejki komunikatow]");
		exit(1);
	}

	return msg_id;
}

void P(int sem_id, int sem_num) {
	struct sembuf sbuf;
	sbuf.sem_num = sem_num;
	sbuf.sem_op = -1;
	sbuf.sem_flg = 0;

	if (semop(sem_id, &sbuf, 1) == -1) {
		perror("Operacja P nie powiodla sie!");
		exit(1);
	}
}

void V(int sem_id, int sem_num) {
	struct sembuf sbuf;
	sbuf.sem_num = sem_num;
	sbuf.sem_op = 1;
	sbuf.sem_flg = 0;

	if (semop(sem_id, &sbuf, 1) == -1) {
		perror("Operacja V nie powiodla sie!");
		exit(1);
	}
}