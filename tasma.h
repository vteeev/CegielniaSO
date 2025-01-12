#ifndef TASMA_H
#define TASMA_H

#include <pthread.h>  // Do��czamy bibliotek� dla pthread

// Struktura reprezentuj�ca ta�m�
typedef struct {
    int maks_liczba_cegiel;
    int maks_masa;
    int aktualna_masa;
    int* tasma;       // Tablica na przechowywanie cegie�
    int front;        // Indeks pierwszej ceg�y w kolejce
    int rear;         // Indeks ostatniej ceg�y w kolejce
    int size;         // Liczba cegie� w kolejce
    pthread_mutex_t mtx;  // Mutex do synchronizacji dost�pu
    pthread_cond_t cv;    // Zmienna warunkowa do synchronizacji
} Tasma;

// Funkcje operuj�ce na ta�mie
void Tasma_init(Tasma* tasma, int maks_liczba_cegiel, int maks_masa);
int Tasma_dodaj_cegle(Tasma* tasma, int masa_cegly);
int Tasma_pobierz_cegle(Tasma* tasma);
int Tasma_sprawdz_cegle(Tasma* tasma);

#endif // TASMA_H
