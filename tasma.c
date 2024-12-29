#include "tasma.h"

Tasma* tasma = NULL;
sem_t sem_tasma;

void init_tasma() {
    int shm_id = shmget(IPC_PRIVATE, sizeof(Tasma), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget failed");
        exit(1);
    }
    tasma = (Tasma*)shmat(shm_id, NULL, 0);
    if (tasma == (void*)-1) {
        perror("shmat failed");
        exit(1);
    }
    tasma->liczba_cegiel = 0;
    tasma->aktualna_masa = 0;
    sem_init(&sem_tasma, 1, 1);
}

void dodaj_cegle(int masa) {
    sem_wait(&sem_tasma);
    if (tasma->liczba_cegiel < K && (tasma->aktualna_masa + masa) <= M) {
        tasma->cegly[tasma->liczba_cegiel++].masa = masa;
        tasma->aktualna_masa += masa;
    }
    sem_post(&sem_tasma);
}

int zdejmij_cegle(int ladownosc) {
    int ladunek = 0;
    sem_wait(&sem_tasma);
    while (ladunek < ladownosc && tasma->liczba_cegiel > 0) {
        ladunek += tasma->cegly[--tasma->liczba_cegiel].masa;
        tasma->aktualna_masa -= tasma->cegly[tasma->liczba_cegiel].masa;
    }
    sem_post(&sem_tasma);
    return ladunek;
}
