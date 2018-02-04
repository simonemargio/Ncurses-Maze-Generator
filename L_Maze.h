#ifndef LASD_FINAL_2_L_MAZE_H
#define LASD_FINAL_2_L_MAZE_H

#include "L_Struttura.h"

/* selezione di una cella random */
M *F_cella_random(Str P);

/* verifica adiacenti unvisited */
int F_controllo_unvisited(Str P,M *cell);

/* verifica della condizione unvisited */
int F_controllo_nodo_unvisited(M *cell);

/* seleziona un adiacente casuale */
M *F_adj_random(Str P,M *cell);

/* seleziona l'adiacente random */
M *F_seleziona_cella_random(Str P, int rand, M *cDx, M *cSx, M* xUP, M *cDw, M *cell);

/* scava tra le celle */
void F_dig_to(Str P, M *cell, M *newcell);

/* eliminazione di vicoli ciechi */
void F_elimina_vicoli_ciechi(Str P);

/*  elinazione di un muro */
void F_elimina_muri(Str P,int r, int c,int nMuri,int nRand);

/* stampa a video il labirinto */
void F_genera_area_labirinto(Str P);

/* disegna attorno al labirinto */
void F_genera_area_contorno(Str P);

/*  modifica del labirinto */
void F_modifica_labirinto(Str P);

/* ricerca del percorso scelto */
void F_ricerca_percorso(Str P);

/* modifica lo stato degli archi di una cella */
void F_sistema_archi_modifica_labirinto(Str P,int r,int c);

/* ritornano la cella adiacente */
M *F_coord_nodo_dx(Str P,int r,int c);
M *F_coord_nodo_sx(Str P,int r,int c);
M *F_coord_nodo_dw(Str P,int r,int c);
M *F_coord_nodo_up(Str P,int r,int c);


#endif //LASD_FINAL_2_L_MAZE_H
