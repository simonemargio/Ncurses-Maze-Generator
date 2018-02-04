#include "L_Prim.h"
#include "L_Struttura.h"
#include "L_Utility.h"
#include "L_Coda.h"
#include "L_Maze.h"


/*
 *  Descrizione:    generatore di tipo Prim.
 *  Dettagli:       //
 *
 *  Parametri in:   P->struttura di gestione principale.
 *  Parametri out:  //
 *
 *  Chiamante:       L_Utility - F_stampa_menu_labirinto.
 *
*/
void F_prim_maze(Str P)
{
    /*
     * Creazione della struttuca coda e selezione
     * di una cella random dal labirinto.
     * Questa viene messa in coda e successivamente
     * elaborata.
     */
    Coda Q=NULL;
    M *initcell=F_cella_random(P);
    Q=F_enqueue(Q,initcell);
    P->nsize=0;
    /*
     * Viene mantenuto il numero totale
     * degli elementi presenti nella coda.
     * Il valore 1 indica inserimento di una unita',
     * viceversa il valore 0 indica il decremento di una
     * unità. L'uso di tale valore e' dovuto alla sua randomizzazione
     * per selezionare una cella casuale dalla coda
     */
    F_aggiorna_nelem_coda(P,1);

    while (Q)
    {
        /*
         * Si preleva un elemento casuale dalla coda.
         * Si verifica se ha adiacenti unvisited. Caso
         * positivo si procede nello scavare le celle, aggiungendo
         * l'adicente alla coda. Caso negativo di elimina la cella
         * dalla coda.
         */
        M *cell=F_random_coda(P,Q);

        if(F_controllo_unvisited(P,cell))
        {
            M *newcell=F_adj_random(P,cell);
            F_dig_to(P,cell,newcell);
            Q=F_enqueue(Q,newcell);
            F_aggiorna_nelem_coda(P,1);
        }
        else
        {
            Q=F_dequeue(Q,cell);
            F_aggiorna_nelem_coda(P,0);
        }
    }

}


/*
 *  Descrizione:    seleziona un elemento random dalla coda.
 *  Dettagli:       //
 *
 *  Parametri in:   P->strututra gestione principale,
 *                  Q->coda.
 *  Parametri out:  cell->cella selezionata.
 *
 *  Chiamante:      L_Prim - F_prim_maze.
 *
*/
M *F_random_coda(Str P, Coda Q)
{
    int rand=F_genera_int_random(P->nsize);
    M *cell=F_preleva_random_coda(Q,rand);

    return cell;
}
