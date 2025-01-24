#ifndef CIEZAROWKA_H
#define CIEZAROWKA_H


// Funkcja dla ciê¿arówek
void dequeue(Tasma* tasma);
void enqueue(Tasma* tasma, int truck_id);




void* ciezarowka(void* arg);
void start_ciezarowka(int id);
pid_t utworz_ciezarowke(int id);

#endif