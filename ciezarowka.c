#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
//#include "operacje.c"
#include "operacje.h"
#include "dyspozytor.h"
#include <string.h>
int odjazd = 0;

SharedFlags* flaga;

void signal_handler(int signum) {
    if (signum == SIGUSR1) {
        printf("\n\n\nCiê¿arówka %d otrzyma³a sygna³ SIGUSR1. Odje¿d¿a z obecnym ³adunkiem.\n\n\n\n", getpid());
        odjazd = 1; // Flaga odjazdu
    }
    else if (signum == SIGUSR2) {
        printf("\n\n\nCiê¿arówka %d otrzyma³a sygna³ SIGUSR2. Koniec pracy\n\n\n\n", getpid());
        flaga->koniec_pracy = 1; // Flaga odjazdu
    }
}

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
        printf("Ciê¿arówka odje¿d¿a z ³adunkiem: %d\n", ladunek);
        sleep(czas_transportu);
    }
    return NULL;
}


void truck_process(int truck_id, Tasma* shared_buffer, Ciezarowka* ciezarowka, SharedFlags* shared_f, int semID, int msgid)
{
    srand(time(NULL) ^ ((truck_id + 1) * 69 << 16));
    //init_signal_handling();
    flaga = shared_f;
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);


    sem_wait2(semID, 7);
    shared_buffer->kolejnosc[shared_buffer->rear] = truck_id;
    shared_buffer->rear = (shared_buffer->rear + 1) % C_COUNT;
    ciezarowka->trucks[truck_id] = getpid();
    printf("\t\t\t\t\t\t%d\n", truck_id);
    //printf("\tHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH\n");
    sem_post2(semID, 7);
    sleep(2);
    int x;//usleep
    int load;
    int liczba;
    int czas_oczekiwania;
    struct my_msg some_data;
    char msg1[MAX_TEXT] = "\t\t\t\tCIEZAROWKA ODJECHALA Z LADUNKIEM\n";
    char msg2[MAX_TEXT] = "\t\t\t\tCIEZAROWKA PRZYJECHALA PUSTA\n";

    while (1)
    {
        load = 0;
        shared_f->truck_signaled[truck_id] = 0;
        sem_wait2(semID, 4);
        if (shared_f->koniec_pracy_ciezarowki == 1 && shared_buffer->count == 0) {

            printf("KONIEC PRACY CIEZAROWKI[%d]\n", truck_id);
            sem_post2(semID, 4);

            //7)
            exit(2);
        }
        ciezarowka->active_truck2 = truck_id;

        // 6)


        while (load < CIEZAROWKA_LIMIT) {
            //5)                    
            if (shared_f->odjazd == 1) {
                printf("Ciê¿arówka %d odje¿d¿a z ³adunkiem %d (wczeœniejszy sygna³ SIGUSR1).\n", truck_id, load);
                shared_f->odjazd = 0; // Reset flagi

                break;
            }


            //printf("helll-przed\n");
            //4)                           

            sem_wait2(semID, 3);
            //printf("helll-przed2\n");

            sem_wait2(semID, 1);
            if ((load + shared_buffer->buffer[shared_buffer->head]) > CIEZAROWKA_LIMIT) {
                sem_post2(semID, 1);
                //sem_post2(semID, 5);
                sem_post2(semID, 3);
                break;
            }
            liczba = shared_buffer->buffer[shared_buffer->head];
            shared_buffer->head = (shared_buffer->head + 1) % MAX_CAPACITY;

            shared_buffer->count -= 1;
            shared_buffer->suma -= liczba;
            load += liczba;
            //3)                       
            for (int i = 0; i < liczba; i++) {
                sem_post2(semID, 2); // Zmniejsza wartoœæ semafora
            }
            sem_post2(semID, 1);
            sem_post2(semID, 5);

            //tutaj 1)
            //printf("helll-po\n");
            printf("\033[34m\tCiê¿arówka%d odbiera %d, Suma: %d/%d\033[0m\n", truck_id, liczba, load, CIEZAROWKA_LIMIT);
            if (shared_buffer->count == 0) {
                if (shared_f->koniec_pracy_ciezarowki == 1) {

                    printf("JESZCZE MUSZE ODWIEZC CEGLY[%d]\n", truck_id);

                    break;
                }
            }
            else {
                //printf("NIE\n");
            }
            //x=rand() %10000;
            //usleep(x);

//2)

        }
        shared_f->truck_signaled[truck_id] = 1;
        shared_buffer->front = (shared_buffer->front + 1) % C_COUNT;
        printf("\033[31m\t\t\tCiê¿arówka %d odje¿d¿a z ³adunkiem %d/%d\033[0m\n", truck_id, load, CIEZAROWKA_LIMIT);


        sem_post2(semID, 4);

        czas_oczekiwania = rand() % +5 + 1;
        sleep(czas_oczekiwania);
        //semctl(semID, 9, SETVAL, CIEZAROWKA_LIMIT);//jesli ciezarowka odjedzie resetujemy semafor
        sem_wait2(semID, 7);

        shared_buffer->kolejnosc[shared_buffer->rear] = truck_id;
        shared_buffer->rear = (shared_buffer->rear + 1) % C_COUNT;// Przesuwamy wskaŸnik rear
        // Resetujemy status ciê¿arówki po jej zakoñczeniu
        printf("\033[30;41m\t\t\t\t\t\n[[[[[Ciê¿arówka %d wróci³a i jest gotowa do pracy]]]]]\033[0m\n\n", truck_id);
        sem_post2(semID, 7);
    
}


int main()
{


    printf("KKKKKKKKKKKKKKKKKKKKKK\n");
    key_t key_m1 = ftok(".", 'R');
    if (key_m1 == -1) {
        perror("ftok");
        exit(1);
    }
    int shmid1 = shmget(key_m1, sizeof(Tasma), IPC_CREAT | 0600);
    if (shmid1 == -1) {
        perror("shmget");
        exit(1);
    }

    Tasma* tasma = (Tasma*)shmat(shmid1, NULL, 0);
    if (tasma == (void*)-1) {
        perror("shmat");
        exit(1);
    }
    printf("KKKKKKKKKKKKKKKKKKKKKK\n");
    key_t key_m2 = ftok("/bin/ls", 'C');

    if (key_m2 == -1) {
        perror("ftok");
        exit(1);
    }
    int shmid2 = shmget(key_m2, sizeof(Ciezarowka), IPC_CREAT | 0600);
    if (shmid2 == -1) {
        perror("shmgetttttt");
        exit(1);
    }
    Ciezarowka* ciezarowka = (Ciezarowka*)shmat(shmid2, NULL, 0);
    if (ciezarowka == (void*)-1) {
        perror("shmattttt");
        exit(1);
    }
    printf("KKKKKKKKKKKKKKKKKKKKKK\n");
    key_t key_m3 = ftok("/bin/ls", 'F');

    int shmid3 = shmget(key_m3, sizeof(SharedFlags), IPC_CREAT | 0600);
    if (shmid3 == -1) {
        perror("shmget");
        exit(1);
    }
    SharedFlags* shared_f = (SharedFlags*)shmat(shmid3, NULL, 0);
    if (shared_f == (void*)-1) {
        perror("shmat");
        exit(1);
    }
    printf("KKKKKKKKKKKKKKKKKKKKKK\n");
    int semID;
    key_t key_s = ftok("/bin/ls", 'S');
    semID = semget(key_s, 10, IPC_CREAT | 0666); // 8 semaforów
    if (semID == -1) {
        perror("semget");
        exit(1);
    }
    key_t key_msg = ftok("/bin/ls", 'M');
    int msgid = msgget(key_msg, IPC_CREAT | 0600);
    if (msgid == -1) {
        fprintf(stderr, "message failed with error:%d\n", errno);
        exit(EXIT_FAILURE);
    }

    //ustaw_handler();
    tasma->front = 0;
    tasma->rear = 0;
    for (int i = 0; i < C_COUNT; i++) {
        tasma->kolejnosc[i] = -1;  // Pocz¹tkowo kolejnoœæ jest pusta
    }
    for (int i = 0; i < C_COUNT; i++) {
        shared_f->truck_signaled[i] = 0;  // Ustawienie pocz¹tkowe na 0
    }
    shared_f->koniec_pracy_ciezarowki = 0;
    shared_f->koniec_pracy_pracownikow = 0;
    shared_f->odjazd = 0;


    pid_t truck_pids[C_COUNT];
    for (int i = 0; i < C_COUNT; i++) {
        truck_pids[i] = fork();
        if (truck_pids[i] == 0) {

            truck_process(i, tasma, ciezarowka, shared_f, semID, msgid);

            exit(0);
        }
        else if (truck_pids[i] > 0) {
        }

    }

    for (int i = 0; i < (C_COUNT); i++) {
        wait(NULL);
    }

    shmdt(tasma);  // Od³¹czenie pamiêci dzielonej
    shmdt(ciezarowka);
    shmdt(shared_f);


    //shmctl(shmid, IPC_RMID, NULL);//usuniecie
    return 0;
}