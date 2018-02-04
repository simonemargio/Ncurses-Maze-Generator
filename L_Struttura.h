#ifndef LASD_FINAL_2_L_STRUTTURA_H
#define LASD_FINAL_2_L_STRUTTURA_H

#include <curses.h>
#include "L_Struttura.h"
#include "L_Heap.h"
#include "L_Coda.h"



/*
 *  Descrizione: struttura principale.
 *  Dettagli:    rappresenta la struttura main per la gestione di tutto il programma.
 *
 *  Contenuto:   matrice->contiene la matrice strutturata in termini di nodi
 *               di un grafo.
 *
 *               rsize->righe della matrice.
 *
 *               csize->colonne della matrice
 *
 *               nsize->valore numerico usato da:
 *                      L_Utility: mantiene l'informazione sul tipo di algoritmo
 *                                 di ricerca che stato usato (0: nessuno, 1: Dijkstra,
 *                                 2: A*).
 *                      L_Prim: mantiene l'informazione sul numero di elementi
 *                              presenti nella coda.
 *
 *              wMenu->puntatore alla finestra generata per i due menu' principali.
 *
 *              wOplab->puntatore alla finestra generata per operare sul labirinto.
 *
 *              StrHeap->puntatore alla struttura di gestione Heap.
*/
struct struttura_principale{
    struct struttura_nodo **matrice;
    int rsize;
    int csize;
    int nsize;
    WINDOW *wMenu;
    WINDOW *wOplab;
    struct struttura_gestione_heap *StrHeap;
};


/*
 *  Descrizione: struttura di un nodo/cella del grafo/labirinto.
 *  Dettagli:    contiene tutte le informazioni relative alla gestione
 *               di una singola cella presente nel labirinto di riferimento.
 *
 *  Contenuto:   murcorr->valore numerico che indica lo stato
 *               di una cella (0: muro, 1:corridio).
 *
 *               visitato->indica lo stato di visita della cella (0: cella
 *               non visitata, 1: cella visitata, 2: cella visitata ma non
 *               facente parte dell'insieme di celle che compongono il
 *               percorso minimo).
 *
 *               pUP,DW,SX,DX->puntatori all'arco/peso del nodo/cella successiva
 *               in base ai quattro punti cardinali (UP: sopra, DW: sotto,
 *               SX: sinistra, DX: destra).
 *
 *               inf->punta alla struttura contenente informazioni sul nodo di
 *               riferimento (r: riga, c: colonna, p: nodo precedente).
 *               Maggiori informazioni su tale struttura nella documentazione
 *               a pag.10, 2.2.2.
 *
*/
struct struttura_nodo{
    int murcorr;
    int visitato;
    struct struttura_peso *pUP;
    struct struttura_peso *pDW;
    struct struttura_peso *pSX;
    struct struttura_peso *pDX;
    struct coordinate_nodo *inf;
};


/*
 *  Descrizione: mantiene l'informazione di un nodo.
 *  Dettagli:    contiene la riga, colonna e il nodo precedente
 *               da cui si è arrivato al nodo corrente.
 *               Maggiori informazioni su tale struttura nella documentazione
 *               a pag.10, 2.2.2.
 *
 *  Contenuto:   valori numerici.
 *
*/
struct coordinate_nodo{
    int r;
    int c;
    int p;
};


/*
 *  Descrizione:  mantiene l'informazione del peso tra due nodi/celle.
 *  Dettagli:     //
 *
 *  Contenuto:    peso->valore indicante il peso.
 *
*/
struct struttura_peso{
    float peso;
};


/*
 *  Descrizione:  allocata come matrice mantiene l'informazione della stima.
 *  Dettagli:     usata da Dijkstra e A*.
 *
 *  Contenuto:    stima->valore di stima stabilito.
 *
*/
struct struttura_d
{
    float stima;
};


/*
 *  Descrizione:  allocata come matrice mantiene l'informazione del nodo predecessore.
 *  Dettagli:     usata da Dijkstra e A*.
 *
 *  Contenuto:    pred->puntatore al predecessore.
 *
*/
struct struttura_p
{
    void *pred;
};


typedef struct struttura_principale *Str;
typedef struct struttura_peso *Peso;
typedef struct struttura_nodo M;
typedef struct coordinate_nodo *InfNodo;
typedef struct struttura_d Dd;
typedef struct struttura_p Pp;
typedef struct struttura_gestione_heap *StructHeap;
typedef struct struttura_nodo_albero *Albero;

/* allocazione della struttura pirncipale */
Str F_alloca_struttura_principale();

/* allocazione della matrice */
void F_alloca_struttura_matrice(Str P);

/* alloca la struttura di informazione di un nodo */
InfNodo F_alloca_info_nodo(int r,int c,int p);

/* alloca la struttura per il peso */
Peso F_alloca_struttura_peso();

/* inizializza la matrice */
void F_inizializza_matrice(Str P);

/* creazione di una finestra */
WINDOW *F_crea_finesta(int rig, int col, int starty, int startx);

/* allocazione matrice delle stime */
Dd **F_alloca_d(Str P);

/* allocazione matrice dei predecessori */
Pp **F_alloca_p(Str P);

/* allocazione struttura di gestione Heap */
StructHeap F_alloca_struttura_heap(Str P,Dd **d, Pp **p);

/* deallocazione della matrice delle stime */
void F_dealloca_d(StructHeap Heap,Str P);

/*  deallocazione della matrice delle stime */
void F_dealloca_p(StructHeap Heap, Str P);

/* dealloca tutti gli archi della matrice */
void F_dealloca_archi_matrice(Str P);

/* dealloca la struttura di informazioni sulle celle */
void F_dealloca_info_cell(Str P);

/* eallocazione della struttura principale */
void F_dealloca_strutture(Str P);

/* dealloca la matrice di riferimento */
void F_delloca_matrice(Str P);

/* deallocazione della struttura Heap */
void F_dealloca_heap(Str P);

/* dealloca l'intero albero heap */
void F_dealloca_albero_heap(Albero T);

#endif //LASD_FINAL_2_L_STRUTTURA_H
