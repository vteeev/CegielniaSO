#include "cegielnia.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* ciezarowka(void* arg) {
    int czas_transportu = *(int*)arg;
    while (1) {
        int ladunek = zdejmij_cegle(C);
        printf("Ciê¿arówka odje¿d¿a z ³adunkiem: %d\n", ladunek);
        sleep(czas_transportu);
    }
    return NULL;
}

void start_ciezarowka(int id) {
    printf("Ciê¿arówka %d rozpoczyna pracê (PID: %d)\n", id, getpid());
    while (1) {
        sleep(2); // Symulacja transportu
        printf("Ciê¿arówka %d: przewozi ceg³y...\n", id);
    }
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