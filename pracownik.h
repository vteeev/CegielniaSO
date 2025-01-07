#ifndef PRACOWNIK_H
#define PRACOWNIK_H

#include "cegielnia.h"
#include <sys/types.h>



// Funkcja dla pracowników
void* pracownik(void* arg);

void start_pracownik(int id);
pid_t utworz_pracownika(int id);

#endif