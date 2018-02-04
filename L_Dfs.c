#include "L_Dfs.h"
#include "L_Stack.h"
#include "L_Utility.h"
#include "L_Maze.h"


/*
 *  Descrizione:    generatore ispirato a DFS.
 *  Dettagli:       //
 *
 *  Parametri in:   P->struttura di gestione principale.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Utility - F_stampa_menu_labirinto.
 *
*/
void F_dfs_maze(Str P)
{
    /*
     * Creazione della struttura stack, selezione di
     * una cella random e inserimento di questa nello
     * stack.
     *
     */
    Stack S=NULL;
    M *initcell=F_cella_random(P);
    S=F_push(S,initcell);

    while (S)
    {
        M *cell=F_top(S);
        /*
         * Si preleva la cella in cima allo stack
         * e si verifica se la cella contiene adiacenti non visitati
         * (circondati da quattro muri). Caso positivo
         * si procede nello scavare le due celle (diventano
         * corridi), altimenti si elimina la cella dallo stack.
         *
         */
        if(F_controllo_unvisited(P,cell)) // L_Maze
        {
            M *newcell=F_adj_random(P,cell); // L_Maze
            F_dig_to(P,cell,newcell);  // L_Maze
            S=F_push(S,newcell);
        } else
            S=F_pop(S);
    }
}


