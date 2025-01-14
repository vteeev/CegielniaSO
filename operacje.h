#ifndef OPERACJE_H
#define OPERACJE_H

#include <sys/types.h>
#include <sys/ipc.h>

int create_sem(key_t key);
int create_shm(key_t key, size_t size);
int create_msg(key_t key);
void remove_sem(int sem_id);
void remove_shm(int shm_id, void* addr);
void remove_msg(int msg_id);
int join_sem(key_t key);
int join_shm(key_t key);
int join_msg(key_t key);
void P(int sem_id, int sem_num);
void V(int sem_id, int sem_num);
void print_single_order(int id);

#endif // OPERACJE_H