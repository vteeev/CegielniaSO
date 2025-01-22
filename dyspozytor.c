#include <signal.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include "operacje.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>


/*void monitoruj_klawisze() {
    char c;
    //printf("Naciœnij 'q' (SIGUSR1) lub 'w' (SIGUSR2), aby wys³aæ sygna³.\n");
    while (1) {
        c = getchar();
        if (c == 'q') {
            // Wyœlij sygna³ SIGUSR1 do samego siebie
            kill(getpid(), SIGUSR1);
        }// else if (c == 'w') {
            // Wyœlij sygna³ SIGUSR2 do samego siebie
           // kill(getpid(), SIGUSR2);
        //}
    }
}*/

// Funkcja wczytuje znak bez czekania na Enter
char get_char_no_wait() {
    struct termios oldt, newt;
    char ch;
    int oldf;

    // Pobierz bie¿¹ce ustawienia terminala
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Wy³¹cz kanoniczny tryb i echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar(); // Pobierz znak z wejœcia

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Przywróæ ustawienia terminala
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        return ch; // Zwróæ znak, jeœli zosta³ wprowadzony
    }

    return EOF; // Zwróæ EOF, jeœli brak znaku
}
void flush_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);  // Czyœci bufor do koñca linii
}

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>

// Funkcja do przechwycenia pojedynczego znaku z klawiatury
char getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int main() {

    key_t key_m1 = ftok(".", 'R');
    int shmid1 = shmget(key_m1, sizeof(Tasma), IPC_CREAT | 06);
    Tasma* tasma = (Tasma*)shmat(shmid1, NULL, 0);
    if (tasma == (void*)-1) {
        perror("shmat");
        exit(1);
    }

    key_t key_m2 = ftok("/bin/ls", 'C');

    int shmid2 = shmget(key_m2, sizeof(Ciezarowka), IPC_CREAT | 0600);
    if (shmid2 == -1) {
        perror("shmget");
        exit(1);
    }
    Ciezarowka* ciezarowka = (Ciezarowka*)shmat(shmid2, NULL, 0);
    if (ciezarowka == (void*)-1) {
        perror("shmat");
        exit(1);
    }

    key_t key_m3 = ftok("/bin/ls", 'F');
    if (key_m3 == -1) {
        perror("ftok");
        exit(1);
    }
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
    key_t key_m4 = ftok("/bin/ls", 'P');
    int shmid4 = shmget(key_m4, sizeof(Pracownik), IPC_CREAT | 0600);
    if (shmid4 == -1) {
        perror("shmget");
        exit(1);
    }
    Pracownik* prac = (Pracownik*)shmat(shmid4, NULL, 0);
    if (shared_f == (void*)-1) {
        perror("shmat");
        exit(1);
    }
    /*printf("hejjjjj\n");
    int semID;
    key_t key_s = ftok(".", 'S');
        semID = semget(key_s, 9, IPC_CREAT | 0666); // 8 semaforów
    if (semID == -1) {
        perror("semget");
        exit(1);
    }*/


    //sem_wait2(semID, 8);
    pid_t ciez_pid, prac_pid;
    int rear, front;
    printf("Naciœnij 's', aby wys³aæ sygna³ do aktywnej ciê¿arówki, lub 'q', aby zakoñczyæ.\n");
    for (int i = 0; i < P; i++) {
        printf("Prac[%d]: %d\n", i, prac->tab_prac[i]);
    }
    for (int i = 0; i < C_COUNT; i++) {
        printf("PID ciê¿arówki[%d]: %d\n", i, ciezarowka->trucks[i]);
    }

    while (1) {
        char c = getch(); // Pobranie pojedynczego znaku z klawiatury

        if (c == 'r') {
            //Wysy³amy sygna³ tylko do aktywnej ciê¿arówki

            ciez_pid = ciezarowka->trucks[ciezarowka->active_truck2];
            rear = tasma->rear;
            front = tasma->front;
            //if(tasma->rear!=tasma->front){
            printf("oooooooooooo\n");

            if (ciez_pid > 0 && shared_f->truck_signaled[ciezarowka->active_truck2] == 0) {  // Jeœli sygna³ nie zosta³ jeszcze wys³any
                //main_pid = ciezarowka->trucks[active_truck_id];
                printf("Wys³ano sygna³ SIGUSR1 do procesu %d (ciê¿arówka aktywna)\n", ciez_pid);
                kill(ciez_pid, SIGUSR1); // Wysy³anie sygna³u
                shared_f->truck_signaled[ciezarowka->active_truck2] = 1;  // Oznacz, ¿e sygna³ zosta³ wys³any
            }
            else {
                printf("Sygna³ ju¿ zosta³ wys³any do tej ciê¿arówki.\n");
            }
            //}

        }
        else if (c == 'y') {
            prac_pid = prac->tab_prac[0];
            if (prac_pid > 0) {

                for (int i = 0; i < C_COUNT; i++) {
                    ciez_pid = ciezarowka->trucks[i];
                    kill(ciez_pid, SIGUSR2); // Wysy³anie sygna³u
                    printf("Wys³ano sygna³ SIGUSR2 do procesu %d (ciezarowki[%d])\n", ciez_pid, i);
                }

                for (int i = 0; i < P; i++) {
                    prac_pid = prac->tab_prac[i];
                    kill(prac_pid, SIGUSR2); // Wysy³anie sygna³u
                    printf("Wys³ano sygna³ SIGUSR2 do procesu %d (pracownika[%d])\n", prac_pid, i);
                }

            }
            else {
                printf("Sygna³ ju¿ zosta³ wys³any do tej ciê¿arówki.\n");
            }
        }

    }
    shmctl(shmid3, IPC_RMID, NULL);  // Usuwa segment pamiêci wspó³dzielonej

    shmctl(shmid2, IPC_RMID, NULL);  // Usuwa segment pamiêci wspó³dzielonej

    shmctl(shmid1, IPC_RMID, NULL);  // Usuwa segment pamiêci wspó³dzielonej

    return 0;
}
