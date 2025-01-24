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

int main()
{


    printf("KKKKKKKKKKKKKKKKKKKKKK\n");
    key_t key_m1 = ftok(".", 'R');
    if (key_m1 == -1) {
        perror("ftok");
        exit(1);
    }
    int shmid1 = shmget(key_m1, sizeof(Tasma), IPC_CREAT | 0600);
    if (shmid1 == -1) {
        perror("shmget");
        exit(1);
    }

    Tasma* tasma = (Tasma*)shmat(shmid1, NULL, 0);
    if (tasma == (void*)-1) {
        perror("shmat");
        exit(1);
    }
    printf("KKKKKKKKKKKKKKKKKKKKKK\n");
    key_t key_m2 = ftok("/bin/ls", 'C');

    if (key_m2 == -1) {
        perror("ftok");
        exit(1);
    }
    int shmid2 = shmget(key_m2, sizeof(Ciezarowka), IPC_CREAT | 0600);
    if (shmid2 == -1) {
        perror("shmgetttttt");
        exit(1);
    }
    Ciezarowka* ciezarowka = (Ciezarowka*)shmat(shmid2, NULL, 0);
    if (ciezarowka == (void*)-1) {
        perror("shmattttt");
        exit(1);
    }
    printf("KKKKKKKKKKKKKKKKKKKKKK\n");
    key_t key_m3 = ftok("/bin/ls", 'F');

    int shmid3 = shmget(key_m3, sizeof(SharedFlags), IPC_CREAT | 0600);
    if (shmid3 == -1) {
        perror("shmget");
        exit(1);
    }
    SharedFlags* shared_f = (SharedFlags*)shmat(shmid3, NULL, 0);
    if (shared_f == (void*)-1) {
        perror("shmat");
        exit(1);
    }
    printf("KKKKKKKKKKKKKKKKKKKKKK\n");
    int semID;
    key_t key_s = ftok("/bin/ls", 'S');
    semID = semget(key_s, 10, IPC_CREAT | 0666); // 8 semaforów
    if (semID == -1) {
        perror("semget");
        exit(1);
    }



    shmdt(tasma);  // Od³¹czenie pamiêci dzielonej
    shmdt(ciezarowka);
    shmdt(shared_f);


    //shmctl(shmid, IPC_RMID, NULL);//usuniecie
    return 0;