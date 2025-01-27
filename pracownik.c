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

void start_pracownik(int id) {
    printf("Pracownik %d rozpoczyna pracê (PID: %d)\n", id, getpid());
    while (1) {
        sleep(1); // Symulacja pracy
        printf("Pracownik %d: produkuje ceg³y...\n", id);
    }
}

// Funkcja do tworzenia procesu pracownika
pid_t utworz_pracownika(int id) {
    pid_t pid = fork(); // Tworzenie nowego procesu
    if (pid == 0) {
        // Kod wykonywany przez proces dziecka
        start_pracownik(id);
        exit(0); // Bezpieczne zakoñczenie procesu dziecka po pracy
    }
    else if (pid < 0) {
        // Obs³uga b³êdu, jeœli fork() siê nie powiód³
        perror("Nie uda³o siê utworzyæ procesu pracownika");
    }
    return pid; // Zwracamy PID procesu dziecka do procesu rodzica
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