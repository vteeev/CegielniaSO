#include "cegielnia.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* ciezarowka(void* arg) {
    int czas_transportu = *(int*)arg;
    while (1) {
        int ladunek = zdejmij_cegle(C);
        printf("Ci�ar�wka odje�d�a z �adunkiem: %d\n", ladunek);
        sleep(czas_transportu);
    }
    return NULL;
}

void start_ciezarowka(int id) {
    printf("Ci�ar�wka %d rozpoczyna prac� (PID: %d)\n", id, getpid());
    while (1) {
        sleep(2); // Symulacja transportu
        printf("Ci�ar�wka %d: przewozi ceg�y...\n", id);
    }
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