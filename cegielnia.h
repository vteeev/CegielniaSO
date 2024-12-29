#ifndef CEGIELNIA_H
#define CEGIELNIA_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define K 10          // Maksymalna liczba cegie� na ta�mie
#define M 30          // Maksymalna masa cegie� na ta�mie
#define N 3           // Liczba ci�ar�wek
#define C 20          // �adowno�� ci�ar�wki

typedef struct {
    int masa;         // Masa ceg�y
} Cegla;

typedef struct {
    Cegla cegly[K];   // Bufor cegie�
    int liczba_cegiel;
    int aktualna_masa;
} Tasma;

// Funkcje dla ta�my
void init_tasma();
void dodaj_cegle(int masa);
int zdejmij_cegle(int ladownosc);

// Globalne zmienne
extern Tasma* tasma;
extern sem_t sem_tasma;

#endif
