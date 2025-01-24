#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
//#include "operacje.c"
#include "operacje.h"
#include "dyspozytor.h"
#include <string.h>
int odjazd = 0;

SharedFlags* flaga;

void signal_handler(int signum) {
    if (signum == SIGUSR1) {
        printf("\n\n\nCi�ar�wka %d otrzyma�a sygna� SIGUSR1. Odje�d�a z obecnym �adunkiem.\n\n\n\n", getpid());
        odjazd = 1; // Flaga odjazdu
    }
    else if (signum == SIGUSR2) {
        printf("\n\n\nCi�ar�wka %d otrzyma�a sygna� SIGUSR2. Koniec pracy\n\n\n\n", getpid());
        flaga->koniec_pracy = 1; // Flaga odjazdu
    }
}

void dequeue(Tasma* tasma) {

    tasma->front = (tasma->front + 1) % C_COUNT;
}

void enqueue(Tasma* tasma, int truck_id) {

    tasma->kolejnosc[tasma->rear] = truck_id;
    tasma->rear = (tasma->rear + 1) % C_COUNT;
}
void* ciezarowka(void* arg) {
    int czas_transportu = *(int*)arg;
    while (1) {
        int ladunek = zdejmij_cegle(C);
        printf("Ci�ar�wka odje�d�a z �adunkiem: %d\n", ladunek);
        sleep(czas_transportu);
    }
    return NULL;
}



pid_t utworz_ciezarowke(int id) {
    pid_t pid = fork();
    if (pid == 0) {
        start_ciezarowka(id);
        exit(0); // Proces dziecka ko�czy prac� po zako�czeniu funkcji
    }
    else if (pid < 0) {
        perror("Nie uda�o si� utworzy� procesu ci�ar�wki");
    }
    return pid;
}