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
