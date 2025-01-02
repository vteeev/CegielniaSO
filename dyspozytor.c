#include "cegielnia.h"

void dyspozytor() {
    while (1) {
        int polecenie;
        scanf("%d", &polecenie);
        if (polecenie == 1) {
            printf("Polecenie: odjazd ciê¿arówki z niepe³nym ³adunkiem.\n");
        }
        else if (polecenie == 2) {
            printf("Polecenie: zakoñczenie pracy.\n");
            exit(0);
        }
    }
}