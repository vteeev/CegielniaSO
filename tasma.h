#ifndef TASMA_H
#define TASMA_H

#include <pthread.h>  // Do³¹czamy bibliotekê dla pthread

// Struktura reprezentuj¹ca taœmê
typedef struct {
    int maks_liczba_cegiel;
    int maks_masa;
    int aktualna_masa;
    int* tasma;       // Tablica na przechowywanie cegie³
    int front;        // Indeks pierwszej ceg³y w kolejce
    int rear;         // Indeks ostatniej ceg³y w kolejce
    int size;         // Liczba cegie³ w kolejce
    pthread_mutex_t mtx;  // Mutex do synchronizacji dostêpu
    pthread_cond_t cv;    // Zmienna warunkowa do synchronizacji
} Tasma;

// Funkcje operuj¹ce na taœmie
void Tasma_init(Tasma* tasma, int maks_liczba_cegiel, int maks_masa);
int Tasma_dodaj_cegle(Tasma* tasma, int masa_cegly);
int Tasma_pobierz_cegle(Tasma* tasma);
int Tasma_sprawdz_cegle(Tasma* tasma);

#endif // TASMA_H
