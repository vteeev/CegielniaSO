#ifndef CIEZAROWKA_H
#define CIEZAROWKA_H

#include "cegielnia.h"

// Funkcja dla ciężarówek
void* ciezarowka(void* arg);

void start_ciezarowka(int id);
pid_t utworz_ciezarowke(int id);

#endif