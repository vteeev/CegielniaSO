#include "tasma.h"
#include <stdio.h>
#include <stdlib.h>

void Tasma_init(Tasma* tasma, int maks_liczba_cegiel, int maks_masa) {
    tasma->maks_liczba_cegiel = maks_liczba_cegiel;
    tasma->maks_masa = maks_masa;
    tasma->aktualna_masa = 0;
    tasma->tasma = (int*)malloc(sizeof(int) * maks_liczba_cegiel); // Alokacja pamiêci na ceg³y
    tasma->front = 0;
    tasma->rear = 0;
    tasma->size = 0;

    // Inicjalizacja mutexu i zmiennej warunkowej
    pthread_mutex_init(&tasma->mtx, NULL);
    pthread_cond_init(&tasma->cv, NULL);
}

int Tasma_dodaj_cegle(Tasma* tasma, int masa_cegly) {
    pthread_mutex_lock(&tasma->mtx);  // Zablokowanie mutexu

    if (tasma->size < tasma->maks_liczba_cegiel && (tasma->aktualna_masa + masa_cegly) <= tasma->maks_masa) {
        tasma->tasma[tasma->rear] = masa_cegly;
        tasma->rear = (tasma->rear + 1) % tasma->maks_liczba_cegiel;
        tasma->aktualna_masa += masa_cegly;
        tasma->size++;

        printf("Dodano ceg³ê o masie %d. Aktualna masa: %d.\n", masa_cegly, tasma->aktualna_masa);

        pthread_cond_signal(&tasma->cv);  // Sygnalizowanie, ¿e ceg³a jest dostêpna
    }

    pthread_mutex_unlock(&tasma->mtx);  // Odblokowanie mutexu
    return (tasma->size < tasma->maks_liczba_cegiel) ? 1 : 0;  // Jeœli dodano ceg³ê, zwróæ 1
}

int Tasma_pobierz_cegle(Tasma* tasma) {
    pthread_mutex_lock(&tasma->mtx);  // Zablokowanie mutexu

    while (tasma->size == 0) {  // Czekanie, jeœli taœma jest pusta
        pthread_cond_wait(&tasma->cv, &tasma->mtx);  // Czekanie na dostêpnoœæ ceg³y
    }

    int masa_cegly = tasma->tasma[tasma->front];
    tasma->front = (tasma->front + 1) % tasma->maks_liczba_cegiel;
    tasma->aktualna_masa -= masa_cegly;
    tasma->size--;

    printf("Pobrano ceg³ê o masie %d. Pozosta³a masa: %d.\n", masa_cegly, tasma->aktualna_masa);

    pthread_mutex_unlock(&tasma->mtx);  // Odblokowanie mutexu
    return masa_cegly;
}

int Tasma_sprawdz_cegle(Tasma* tasma) {
    pthread_mutex_lock(&tasma->mtx);  // Zablokowanie mutexu

    int masa_cegly = 0;
    if (tasma->size > 0) {
        masa_cegly = tasma->tasma[tasma->front];
    }

    pthread_mutex_unlock(&tasma->mtx);  // Odblokowanie mutexu
    return masa_cegly;
}
