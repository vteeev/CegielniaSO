#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <signal.h>
#include <sys/ipc.h>
#include <fcntl.h>  // Dla O_WRONLY, O_CREAT, O_APPEND
#include "operacje.h"
//#include "operacje.c"

int shmID, semID, msgID;  //ID semafora, kolejki kom. pamieci dzielonej


int main() {
    init_tasma();

}