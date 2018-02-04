#include <stdlib.h>
#include <stdio.h>
#include "L_Stack.h"
#include "L_Error.h"



/*
 *  Descrizione: inserisce in testa un nuovo elemento.
 *  Dettagli:    //
 *
 *  Parametri in:   P->struttura di gestione principale,
 *                  T->elemento da inserire.
 *  Parametri out:  P->"nuovo" stack.
 *
 *  Chiamante:      L_Dfs - F_dfs_maze.
 *
*/
Stack F_push(Stack P,void *T)
{
    Stack nuovo_elemento=malloc(sizeof(struct struttura_elemento_pila));

    if(nuovo_elemento!=NULL)
    {
        nuovo_elemento->elemPtr=T;
        nuovo_elemento->nextPtr=P;
        P=nuovo_elemento;
    }
    else
    {
       F_error(13);
    }
    return P;
}


/*
 *  Descrizione: elimina l'elemento in cima.
 *  Dettagli:    //
 *
 *  Parametri in:   P->stack di riferimento.
 *  Parametri out:  P->"nuvo" stack.
 *
 *  Chiamante:   L_Dfs - F_dfs_maze.
 *
*/
Stack F_pop(Stack P)
{
    Stack Tmp=NULL;

    if(P!=NULL)
    {
        Tmp=P->nextPtr;
        free(P);
        P=Tmp;
    }
    return P;
}


/*
 *  Descrizione:   ritorna l'elemento in cima.
 *  Dettagli:      //
 *
 *  Parametri in:  P->stack di riferimento.
 *  Parametri out: elemPtr->elemento in cima allo stack.
 *
 *  Chiamante:   L_Dfs - F_dfs_maze.
 *
*/
void *F_top(Stack P)
{
    if(P!=NULL)
        return(P->elemPtr);
    else
        return NULL;
}