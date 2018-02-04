#ifndef LASD_FINAL_2_L_CODA_H
#define LASD_FINAL_2_L_CODA_H

#include "L_Struttura.h"
#include "L_Coda.h"


typedef struct struttura_principale *Str;
typedef struct struttura_nodo M;
typedef struct struttura_elemento_coda *Coda;

/*
 *  Descrizione:  mantiene l'informazione su un elemento della coda.
 *  Dettagli:     //
 *
 *  Contenuto:    elemPtr->puntatore all'elemento,
 *                nextPtr->puntatore alla struttura che contiene
 *                         l'elemento successivo, se presente.
 *
*/
struct struttura_elemento_coda
{
    void *elemPtr;
    struct struttura_elemento_coda *nextPtr;
};


/* inserimento in coda di un nuovo elemento */
Coda F_enqueue(Coda C,void *elem);

/* eleminazione di un elemento dalla coda */
Coda F_dequeue(Coda C,void *elem);

/* aggiorna il numero di elementi nella coda */
void F_aggiorna_nelem_coda(Str P, int incdecr);

/* selezione di un elemento random dalla coda */
M *F_preleva_random_coda(Coda Q,int rand);

#endif //LASD_FINAL_2_L_CODA_H
