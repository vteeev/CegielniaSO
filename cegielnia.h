#ifndef CEGIELNIA_H
#define CEGIELNIA_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define K 10          // Maksymalna liczba cegie³ na taœmie
#define M 30          // Maksymalna masa cegie³ na taœmie
#define N 3           // Liczba ciê¿arówek
#define C 20          // £adownoœæ ciê¿arówki

typedef struct {
    int masa;         // Masa ceg³y
} Cegla;

typedef struct {
    Cegla cegly[K];   // Bufor cegie³
    int liczba_cegiel;
    int aktualna_masa;
} Tasma;

// Funkcje dla taœmy
void init_tasma();
void dodaj_cegle(int masa);
int zdejmij_cegle(int ladownosc);

// Globalne zmienne
extern Tasma* tasma;
extern sem_t sem_tasma;

#endif
