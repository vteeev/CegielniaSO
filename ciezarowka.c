#include "cegielnia.h"

void* ciezarowka(void* arg) {
    int czas_transportu = *(int*)arg;
    while (1) {
        int ladunek = zdejmij_cegle(C);
        printf("Ci�ar�wka odje�d�a z �adunkiem: %d\n", ladunek);
        sleep(czas_transportu);
    }
    return NULL;
}
