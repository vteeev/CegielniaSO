#include "cegielnia.h"

void dyspozytor() {
    while (1) {
        int polecenie;
        scanf("%d", &polecenie);
        if (polecenie == 1) {
            printf("Polecenie: odjazd ciężarówki z niepełnym ładunkiem.\n");
        }
        else if (polecenie == 2) {
            printf("Polecenie: zakończenie pracy.\n");
            exit(0);
        }
    }
}