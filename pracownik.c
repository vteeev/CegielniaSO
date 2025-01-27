#include "cegielnia.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *pracownik(void *arg) {
    int masa_cegly = *(int *)arg;
    while (1) {
        dodaj_cegle(masa_cegly);
        sleep(rand() % 3);
    }
    return NULL;
}

void pracownik(Tasma* tasma, SharedFlags* shared_f, Pracownik* prac, int i, int semID, pid_t prac_pid) {

    srand(time(NULL) ^ getpid());
    flaga = shared_f;
    signal(SIGUSR2, signal_handler);

    sem_wait2(semID, 0);
    prac->tab_prac[i] = getpid();
    //printf("hahaha %d\n", prac->tab_prac[i]);
    sem_post2(semID, 0);
    int x;
    int c;


    sleep(3);
    while (1) {
        c = rand() % 3 + 1; // Losowanie liczby 1, 2 lub 3

        sem_wait2(semID, 5); // liczni count Tasmy
        for (int j = 0; j < c; j++) {
            sem_wait2(semID, 2); // licznik masy Tasmy

        }

        sem_wait2(semID, 1);
        tasma->buffer[tasma->tail] = c;
        tasma->tail = (tasma->tail + 1) % MAX_CAPACITY;
        tasma->count += 1;
        tasma->suma += c;
        printf("\033[32mP[%d] Dodal: %d  count_t: %d masa_t: %d/%d\033[0m\n", i, c, tasma->count, tasma->suma, N);
        sem_post2(semID, 1);
        sem_post2(semID, 3);


        // Losowe opóŸnienie miêdzy 0 a 100 ms

        sem_wait2(semID, 0);
        if (shared_f->koniec_pracy_pracownikow == 1) {

            printf("\tPRACOWNIK[%d] KONCZY PRACE\n", i);
            sem_post2(semID, 0);


            exit(0);
        }
        sem_post2(semID, 0);//1 //2
    }
}

int main() {

    key_t key_m = ftok("/bin/ls", 'R');
    int shmid1 = shmget(key_m, sizeof(Tasma), IPC_CREAT | 0600);
    if (shmid1 == -1) {
        perror("shmget");
        exit(1);
    }
    Tasma* tasma = (Tasma*)shmat(shmid1, NULL, 0);
    if (tasma == (void*)-1) {
        perror("shmat");
        exit(1);
    }

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

    key_t key_m4 = ftok("/bin/ls", 'P');
    int shmid4 = shmget(key_m4, sizeof(Pracownik), IPC_CREAT | 0600);
    if (shmid4 == -1) {
        perror("shmget");
        exit(1);
    }
    Pracownik* prac = (Pracownik*)shmat(shmid4, NULL, 0);
    if (shared_f == (void*)-1) {
        perror("shmat");
        exit(1);
    }

    key_t key_s = ftok("/bin/ls", 'S');
    int semID = semget(key_s, 10, IPC_CREAT | 0666); // 8 semaforów
    if (semID == -1) {
        perror("semget");
        exit(1);
    }
    Tasma_init(tasma);

    pid_t prac_pid[P];
    for (int i = 0; i < P; i++) {
        pid_t pid = fork();
        prac_pid[i] = pid;
        if (pid == 0) {
            printf("%d\n", getpid());
            // Proces "pracownik"
            //monitoruj_klawisze();
            pracownik(tasma, shared_f, prac, i, semID, prac_pid[i]);
            exit(0);
        }
    }
    //monitoruj_klawisze();
    //kontroluj_sygnal2(prac_pid);

    for (int i = 0; i < P; i++) {
        wait(NULL);
    }



    shmdt(prac);
    shmdt(shared_f);
    shmdt(tasma);

    return 0;
}