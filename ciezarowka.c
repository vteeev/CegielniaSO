#include "cegielnia.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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