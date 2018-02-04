#ifndef LASD_FINAL_2_L_ASTAR_H
#define LASD_FINAL_2_L_ASTAR_H

#include "L_Struttura.h"

/* inizializzazione prima di eseguire A* */
void F_init_Astar(Str P,int r, int c, int rA,int cA);

/* calcolo della sitanza di Manhattan */
float F_distanza_manhattan(int rP,int cP,int rA,int cA);

/* algoritmo A* */
void F_Astar(Str P,StructHeap Heap,int rP,int cP,int rA,int cA);

/* ricerca di un elemento nell'albero */
int F_cerca_indice_Astar(StructHeap Heap, int r,int c);

#endif //LASD_FINAL_2_L_ASTAR_H
