#ifndef OPERACJE_H
#define OPERACJE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <signal.h> // Dla sig_atomic_t

// Deklaracje zmiennych globalnych

// Sta³

#define N 40              //MAX obci¹¿enie taœmy
#define P 3             //Pracownicy
#define MAX_CAPACITY 20  // Pojemnoœæ taœmy
#define C_COUNT 100     //liczba Cie¿arówek
#define CIEZAROWKA_LIMIT 1000  // Limit masy dla ciezarowki

//kolejka komunik
#define MAX_TEXT 50
#define C_ODJAZD 1
#define C_PRZYJAZD 2

// Struktura Taœma (przechowuj¹ca dane i synchronizuj¹ca procesy)
typedef struct {
    int buffer[MAX_CAPACITY];  // Tablica przechowuj¹ca dane
    int head;         // WskaŸnik na pocz¹tek taœmy
    int tail;         // WskaŸnik na koniec taœmy
    int suma;         // Suma ³adunku na taœmie
    int count;        // Liczba elementów w buforze
    int kolejnosc[C_COUNT];       // Tablica kolejnoœci ciê¿arówek
    int front;                    // WskaŸnik na pocz¹tek kolejki (indeks pierwszej ciê¿arówki)
    int rear;  //na koniec
    int last_digit;   // Ostatni element, który zosta³ za³adowany przez ciê¿arówkê
    int active_truck;
} Tasma;

typedef struct {
    pid_t trucks[C_COUNT];  // Tablica przechowuj¹ca dane
    int head;
    int tail;
    int active_truck2;
} Ciezarowka;

typedef struct {
    pid_t tab_prac[P];
}Pracownik;

typedef struct {
    int koniec_pracy;
    int truck_signaled[C_COUNT];

} SharedFlags;

struct my_msg {
    long int msg_type;
    char text[MAX_TEXT];
};

void Tasma_init(Tasma* tasma);

int create_sem(key_t key);
int create_shm(key_t key, size_t size);
int create_msg(key_t key);
void remove_sem(int sem_id);
void remove_shm(int shm_id, void* addr);
void remove_msg(int msg_id);
int join_sem(key_t key);
int join_shm(key_t key);
int join_msg(key_t key);
void sem_wait2(int sem_id, int sem_num);
void sem_post2(int sem_id, int sem_num);


#endif // OPERACJE_H