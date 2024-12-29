#ifndef TASMA_H
#define TASMA_H

#include <semaphore.h>
#include "cegielnia.h"

// Funkcje dla taœmy
void init_tasma();
void dodaj_cegle(int masa);
int zdejmij_cegle(int ladownosc);

#endif
