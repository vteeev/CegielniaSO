#ifndef DYSPOZYTOR_H
#define DYSPOZYTOR_H

#include <semaphore.h>
#include <signal.h> // Dla sig_atomic_t
#include "operacje.h"



char get_char_no_wait();
char getch();
void flush_input_buffer();
//void kontroluj_sygnal1(struct Tasma* tasma, pid_t truck_pids[]);
void kontroluj_sygnal2(pid_t prac_pids[]);

#endif  //DYSPOZYTOR_H
