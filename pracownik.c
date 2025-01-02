#include "cegielnia.h"

void *pracownik(void *arg) {
    int masa_cegly = *(int *)arg;
    while (1) {
        dodaj_cegle(masa_cegly);
        sleep(rand() % 3);
    }
    return NULL;
}
