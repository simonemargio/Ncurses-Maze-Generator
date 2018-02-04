#ifndef LASD_FINAL_2_L_RECURSIVE_H
#define LASD_FINAL_2_L_RECURSIVE_H

#include "L_Struttura.h"


/* algoritmo di tipo Recursive */
void F_recursive_maze(Str P, int r, int c);

/* inizializzazione del generatore Recursive */
void F_init_recursive_maze(Str P);

/*  genera un array con valori casuali */
int *F_direzioni_random();

/*  collega, se possibile, le celle prese  */
int F_dig_up(Str P, int r,int c);
int F_dig_dw(Str P, int r, int c);
int F_dig_sx(Str P,int r, int c);
int F_dig_dx(Str P,int r, int c);

#endif //LASD_FINAL_2_L_RECURSIVE_H
