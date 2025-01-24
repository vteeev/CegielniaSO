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
        printf("\n\n\nCiê¿arówka %d otrzyma³a sygna³ SIGUSR1. Odje¿d¿a z obecnym ³adunkiem.\n\n\n\n", getpid());
        odjazd = 1; // Flaga odjazdu
    }
    else if (signum == SIGUSR2) {
        printf("\n\n\nCiê¿arówka %d otrzyma³a sygna³ SIGUSR2. Koniec pracy\n\n\n\n", getpid());
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
        printf("Ciê¿arówka odje¿d¿a z ³adunkiem: %d\n", ladunek);
        sleep(czas_transportu);
    }
    return NULL;
}



pid_t utworz_ciezarowke(int id) {
    pid_t pid = fork();
    if (pid == 0) {
        start_ciezarowka(id);
        exit(0); // Proces dziecka koñczy pracê po zakoñczeniu funkcji
    }
    else if (pid < 0) {
        perror("Nie uda³o siê utworzyæ procesu ciê¿arówki");
    }
    return pid;
}