#include <stdlib.h>
#include "L_Coda.h"
#include "L_Struttura.h"
#include "L_Error.h"



/*
 *  Descrizione:   inserimento in coda.
 *  Dettagli:      inserisce in coda alla struttura un nuovo elemento.
 *
 *  Parametri in:  C->coda, elem->puntatore all'emento da inserire.
 *  Parametri out: C->coda.
 *
 *  Chiamante:     L_Prim - F_prim_maze.
 *
*/
Coda F_enqueue(Coda C,void *elem)
{
    Coda nuovo_elemento=(struct struttura_elemento_coda *)malloc(sizeof(struct struttura_elemento_coda));

    if(nuovo_elemento!=NULL)
    {
        nuovo_elemento->elemPtr=elem;
        nuovo_elemento->nextPtr=NULL;

        if(C==NULL) C=nuovo_elemento;
        else
        {
            Coda Tmp=C;
            while(Tmp->nextPtr!=NULL)
                Tmp=Tmp->nextPtr;
            Tmp->nextPtr=nuovo_elemento;
        }
    }else
        F_error(11);

    return C;
}


/*
 *  Descrizione:    aggiorna il numero di elementi nella coda.
 *  Dettagli:       mantiene l'informazione del numero totale
 *                  si elementi presenti nella coda, in modo
 *                  da agevolare la randomizzazione dell'elemento
 *                  della coda scelto dalla generazione del labirinto
 *                  di tipo Prim.
 *
 *  Parametri in:   P->struttura di gestione principale,
 *                  incdecr->valore numerico che specifica se incrementare
 *                  o decrementare il numero di elementi della coda (1->incremento,
 *                  2->decremento).
 *  Parametri out:  //
 *
 *  Chiamante:      L_Prim - F_prim_maze.
 *
*/
void F_aggiorna_nelem_coda(Str P, int incdecr)
{
    int nelem=P->nsize;
    if(incdecr) {nelem++; P->nsize=nelem;}
    else {nelem--; P->nsize=nelem;}
}


/*
 *  Descrizione:    selezione di un elemento random dalla coda.
 *  Dettagli:       in base al valore numerico generato, viene preso
 *                  l'elemento di tale valore dalla coda.
 *
 *  Parametri in:   C->coda, rand->intero randomizzato.
 *  Parametri out:  elem->elemento della coda.
 *
 *  Chiamante:      L_Prim - F_random_coda.
 *
*/
M *F_preleva_random_coda(Coda C,int rand)
{
    int i=0; Coda Curr=C;
    void *elem=NULL;

    for(i=0;i<rand && Curr!=NULL;i++) Curr=Curr->nextPtr;

    if(Curr!=NULL) elem=Curr->elemPtr;

    return elem;
}


/*
 *  Descrizione:   eleminazione di un elemento dalla coda.
 *  Dettagli:      essendo la scelta di elementi randomizzata,
 *                 l'eliminazione di un elemento rispecchia le
 *                 procedure di eliminazione di un elemento da
 *                 una lista.
 *
 *  Parametri in:  C->coda, elem->elemento da eliminare.
 *  Parametri out: C->eventuale nuova coda.
 *
 *  Chiamante:     L_Prim - F_prim_maze.
 *
*/
Coda F_dequeue(Coda C,void *elem) {
    Coda Prec = C, Curr = C;

    while (Curr != NULL && Curr->elemPtr != elem) {
        Prec = Curr;
        Curr = Curr->nextPtr;
    }

    if (Curr != NULL) {
        if (C == Curr) C = Curr->nextPtr;
        else Prec->nextPtr = Curr->nextPtr;
    }

    return C;
}