#ifndef PRACOWNIK_H
#define PRACOWNIK_H

#include "cegielnia.h"
#include <sys/types.h>



// Funkcja dla pracownik�w
void* pracownik(void* arg);

void start_pracownik(int id);
pid_t utworz_pracownika(int id);

#endif