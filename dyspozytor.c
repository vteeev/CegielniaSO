#include "cegielnia.h"

void dyspozytor() {
    while (1) {
        int polecenie;
        scanf("%d", &polecenie);
        if (polecenie == 1) {
            printf("Polecenie: odjazd ci�ar�wki z niepe�nym �adunkiem.\n");
        }
        else if (polecenie == 2) {
            printf("Polecenie: zako�czenie pracy.\n");
            exit(0);
        }
    }
}