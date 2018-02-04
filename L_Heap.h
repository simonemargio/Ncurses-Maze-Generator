#ifndef LASD_FINAL_2_L_HEAP_H
#define LASD_FINAL_2_L_HEAP_H

#include "L_Struttura.h"
#include "L_Heap.h"
#include "L_Coda.h"



/*
 *  Descrizione: struttura di un nodo dell'albero heap.
 *  Dettagli:    descrive un singolo nodo presente all'iterno
 *               dell'heap costrutio dai due algoritmi di ricerca
 *               del percorso minimo.
 *
 *  Contenuto:   r,c->riga e colonna della cella di riferimento.
 *               stima->punta all'indirizzo della riga e colonna,
 *               della cella di riferimento, della matrice delle stime,
 *               ptrDx,Sx->puntatore ai figli.
 *
*/
struct struttura_nodo_albero
{
    int r;
    int c;
    void *stima;
    struct struttura_nodo_albero *ptrDx;
    struct struttura_nodo_albero *ptrSx;
};


/*
 *  Descrizione: struttura principale per la gestione di un heap.
 *  Dettagli:    mantiene le informazioni che servono a gestire un
 *               albero heap e le informazioni sulle strutture dati
 *               usate dagli algoritmi di ricerca.
 *
 *  Contenuto:   struttura->punta alla radice dello heap,
 *               heapsize->numero di elementi che compongono l'albero,
 *               d->puntatore alla matrice delle stime,
 *               p->puntatore alla matrice dei predecessori.
 *
*/
struct struttura_gestione_heap
{
    void *struttura;
    int heapsize;

    struct struttura_d **d;
    struct struttura_p **p;
};


typedef struct struttura_gestione_heap *StructHeap;
typedef struct struttura_nodo_albero *Albero;
typedef struct struttura_principale *Str;
typedef struct struttura_nodo M;
typedef struct struttura_elemento_coda *Coda;


/* riprisitna la proprieta' di heap */
void F_heapify(StructHeap Heap, int i);

/* allocazione della struttura per la gestione di heap */
StructHeap F_alloca_heap(StructHeap Heap);

/* creazione dell'albero heap */
void F_crea_heap(StructHeap Heap, Str P,int r,int c);

/* allocazione di un nodo dell'albero heap */
Albero F_crea_nodo_heap(StructHeap Heap,int r,int c);

/*  creazie del vettore contenente il valore in binario dell'indice passato */
int *F_ottieni_bit(int n);

/* ignora tutti i bit pari a zero */
int F_salta_zero_bit(int *bits);

/* inserisce un nodo all'interno dell'albero heap */
Albero F_inserisci_nodo_albero(Albero T,Albero nuovo_elem,int indice);

/* estrazione della radice dallo heap */
Albero F_estrai_minmax_albero(StructHeap Heap);

/*  preleva l'indice del figlio sinistro */
int F_HeapSx(int i);

/* preleva l'indice del figlio destro */
int F_HeapDx(int i);

/* verifica della stima minore in due nodi */
int F_FirstCheck_Albero_Min(StructHeap Heap,int l,int i);

/* verifica della stima minore in due nodi */
int F_SecondCheck_Alebro_Min(StructHeap Heap, int r,int mas);

/* scambia le informazioni di due nodi */
void F_scambio_albero(StructHeap Heap,int i,int mas);

/* preleva il nodo di indice passato */
Albero F_preleva_nodo(StructHeap Heap,int indice);

/* eliminazione della foglia */
void F_elimina_foglia(StructHeap Heap);

/* copia le informazioni di due nodi */
Albero F_copia_valori_albero(Albero minT, Albero newT);

/*  esegue il decrease nel nodo scelto */
void F_decrease_key_albero(StructHeap Heap,int i, float val);

/* inserisce un nuovo nodo nello heap */
void F_inserisci_elemento_albero(StructHeap Heap,Albero nuovo_elemento);

#endif //LASD_FINAL_2_L_HEAP_H
