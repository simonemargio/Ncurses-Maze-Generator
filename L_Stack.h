#ifndef LASD_FINAL_2_L_STACK_H
#define LASD_FINAL_2_L_STACK_H


/*
 *  Descrizione:   struttura di un elemento dello stack.
 *  Dettagli:      //
 *
 *  Contenuto:     elemPtr->punta all'elementosalvato,
 *                 nextPtr->elemento successivo della
 *                 struttura.
 *
*/
struct struttura_elemento_pila
{
    void *elemPtr;
    struct struttura_elemento_pila *nextPtr;
};


typedef struct struttura_elemento_pila *Stack;

/* inserisce in testa un nuovo elemento */
Stack F_push(Stack P,void  *T);

/* elimina l'elemento in cima */
Stack F_pop(Stack P);

/* ritorna l'elemento in cima */
void *F_top(Stack P);


#endif //LASD_FINAL_2_L_STACK_H
