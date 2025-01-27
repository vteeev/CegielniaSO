#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <signal.h>
#include <sys/ipc.h>
#include <fcntl.h>  // Dla O_WRONLY, O_CREAT, O_APPEND
#include "operacje.h"
//#include "operacje.c"

int shmID, semID, msgID;  //ID semafora, kolejki kom. pamieci dzielonej


int main() {
    
    key_t key_m = ftok("/bin/ls", 'R');
    int shmid1 = shmget(key_m, sizeof(Tasma), IPC_CREAT | 0600);
    if (shmid1 == -1) {
        perror("shmget1");
        exit(1);
    }
    Tasma* tasma = (Tasma*)shmat(shmid1, NULL, 0);
    if (tasma == (void*)-1) {
        perror("shmat1");
        exit(1);
    }
    //polacz pamdziel 
    printf("KKKKKKKKKKKKKKKKKKKKKK");

    key_t key_m2 = ftok("/bin/ls", 'C');
    int shmid2 = shmget(key_m2, sizeof(Ciezarowka), IPC_CREAT | 0600);
    if (shmid2 == -1) {
        perror("shmget2");
        exit(1);
    }
    Ciezarowka* ciezarowka = (Ciezarowka*)shmat(shmid2, NULL, 0);
    if (ciezarowka == (void*)-1) {
        perror("shmat");
        exit(1);
    }
    printf("KKKKKKKKKKKKKKKKKKKKKK");

    key_t key_m3 = ftok("/bin/ls", 'F');
    int shmid3 = shmget(key_m3, sizeof(SharedFlags), IPC_CREAT | 0600);
    if (shmid3 == -1) {
        perror("shmget3");
        exit(1);
    }
    SharedFlags* shared_f = (SharedFlags*)shmat(shmid3, NULL, 0);
    if (shared_f == (void*)-1) {
        perror("shmat");
        exit(1);
    }

    key_t key_m4 = ftok("/bin/ls", 'P');
    int shmid4 = shmget(key_m4, sizeof(Pracownik), IPC_CREAT | 0600);
    if (shmid4 == -1) {
        perror("shmget4");
        exit(1);
    }
    Pracownik* prac = (Pracownik*)shmat(shmid4, NULL, 0);
    if (shared_f == (void*)-1) {
        perror("shmat4");
        exit(1);
    }


    //init sem
    key_t key_s = ftok("/bin/ls", 'S');
    semID = semget(key_s, 10, IPC_CREAT | 0666); // 8 semaforów
    if (semID == -1) {
        perror("semgetsem");
        exit(1);
    }
    semctl(semID, 0, SETVAL, 1);
    semctl(semID, 1, SETVAL, 1);
    semctl(semID, 2, SETVAL, N);
    semctl(semID, 3, SETVAL, 0);
    semctl(semID, 4, SETVAL, 1);
    semctl(semID, 5, SETVAL, MAX_CAPACITY);
    semctl(semID, 6, SETVAL, 2);//koniec_ciezarowka
    semctl(semID, 7, SETVAL, 1);
    semctl(semID, 8, SETVAL, 1);
    semctl(semID, 9, SETVAL, 0);

    switch (fork()) {
    case -1:
        printf("Blad fork (mainprog)\n");
        exit(2);
    case 0:
        //printf("hallo\n");
        if (execl("./pracownik", "pracownik", NULL) == -1) {  // Check execl error
            perror("execl failed");
            exit(1);
        }
        break;  // Just to be safe in case of non-termination in child
    }

    // Drugi fork
    switch (fork()) {
    case -1:
        printf("Blad fork (mainprog)\n");
        exit(2);
    case 0:
        //printf("hallo\n");
        if (execl("./ciezarowka", "ciezarowka", NULL) == -1) {  // Check execl error
            perror("execl failed");
            exit(1);
        }
        break;  // Just to be safe in case of non-termination in child
    }
    switch (fork()) {
    case -1:
        printf("Blad fork (mainprog)\n");
        exit(2);
    case 0:
        //printf("hallo\n");
        if (execl("./dyspozytor", "dyspozytor", NULL) == -1) {  // Check execl error
            perror("execl failed");
            exit(1);
        }
        break;  // Just to be safe in case of non-termination in child
    }

    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }




    semctl(semID, 0, IPC_RMID, NULL);
    shmctl(shmid1, IPC_RMID, NULL);
    shmctl(shmid2, IPC_RMID, NULL);
    shmctl(shmid3, IPC_RMID, NULL);
    shmctl(shmid4, IPC_RMID, NULL);
    //shmctl(shmid3, IPC_RMID, NULL);  // Usuwa segment pamiêci wspó³dzielonej
    //shmctl(shmid2, IPC_RMID, NULL);  // Usuwa segment pamiêci wspó³dzielonej



    printf("MAIN: Koniec.\n");

}