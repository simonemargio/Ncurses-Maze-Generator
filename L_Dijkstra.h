#ifndef LASD_FINAL_2_L_DIJKSTRA_H
#define LASD_FINAL_2_L_DIJKSTRA_H

/*
 * Maggiori dettagli sull'uso progettuale
 * del valore INFINITO pari a -1 nella
 * documentazione a pag.8.
 */
#define INFINITO (-1)

#include "L_Struttura.h"
#include "L_Heap.h"

/* inizializzazione prima di eseguire Dijkstra */
void F_init_dijkstra(Str P,int r,int c);

/* algoritmo Dijkstra */
void F_dijkstra(Str P,StructHeap Heap);

/* procedura di rilassamento degli archi */
void F_relax(StructHeap Heap, Albero u, int rU, int cU, int rV, int cV, float arco);

/* ricerca di un elemento nell'albero */
int F_cerca_indice_decrease(StructHeap Heap, int r, int c);

#endif //LASD_FINAL_2_L_DIJKSTRA_H
