#include "L_Dijkstra.h"
#include "L_Struttura.h"
#include <stdio.h>
#include <stdlib.h>
#include "L_Heap.h"
#include "L_Utility.h"
#include "L_Error.h"
#include "L_Maze.h"


/*
 *  Descrizione:    inizializzazione prima di eseguire Dijkstra.
 *  Dettagli:       vengono allocate le strutture contenenti le informazioni
 *                  sulle stime e i nodi predecessori. Infine viene allocato
 *                  l'albero heap di gestione del nodo con stima minima.
 *
 *  Parametri in:   P->struttura di gestione principale,
 *                  r,c-> riga e colonna della cella di partenza.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Maze - F_ricerca_percorso.
 *
*/
void F_init_dijkstra(Str P,int r,int c)
{
    /* Allocazione stime, predessori e struttura Heap */
    Dd **d=F_alloca_d(P); // L_Struttura
    Pp **p=F_alloca_p(P); // L_Struttura
    StructHeap Heap=F_alloca_struttura_heap(P,d,p);

    /* Creazione dello heap contenente tutti nodi corridio */
    F_crea_heap(Heap,P,r,c);

    /* Dijkstra */
    F_dijkstra(P,Heap);

    /* Deallocazione delle stime non più necessarie */
    F_dealloca_d(Heap,P); // L_Struttura

   /*
    * Inseriemnto della struttura Heap nella struttura
    * principale per poter usufruire della matrice
    * di predecessori e per la sua successiva deallocazione.
    *
    */
    P->StrHeap=Heap;
}


/*
 *  Descrizione:    algoritmo Dijkstra.
 *  Dettagli:       //
 *
 *  Parametri in:   P->struttura di gestione principale,
 *                  Heap->struttura di gestione heap.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Dijkstra - F_init_dijkstra.
 *
*/
void F_dijkstra(Str P,StructHeap Heap)
{
    /* Si preleva l'albero haep */
    Albero T=Heap->struttura;

    while (T)
    {
        /* Estrazione dell'elemento con stima piu' piccola */
        Albero u=F_estrai_minmax_albero(Heap);

        if(u)
        {
            /*
             * Variabili presenti:
             *
             * i->indice di selezione dell'adiacente,
             *
             * r,c->riga e colonna del nodo estratta dall'albero,
             *
             * v->contiene il nodo adiacente a u.
             */
            int i,r=u->r,c=u->c;
            M *v=NULL;

            for(i=0;i<4;i++)
            {
                /*
                 * Funzione di rilassamento
                 * a tale funzione vengono passate le seguenti variabili:
                 *
                 * Heap->struttura di gestione dello heap usata per la ricerca
                 *       dell'indice dell'adiacente all'interno dell'albero e per
                 *       l'ececuzione della decrase_key.
                 *
                 * u->caso in cui viene effettuato il decrase, u identifica il nodo
                 *    predecessore.
                 *
                 * r,c->riga e colonna del nodo preso dallo heap (u).
                 *
                 * r,c+1/-1 opp r+1/-1,c->riga e colonna del nodo adiacente trovato.
                 *
                 * peso->peso tra i due nodi u->v.
                 */
                switch (i)
                {
                    case 0: // ADJ DX
                        if(c<P->csize-1)
                        {
                            v=F_coord_nodo_dx(P,r,c);
                            if(v->murcorr!=0)
                            {
                                F_relax(Heap,u,r,c,r,c+1,v->pSX->peso);
                            }
                        }
                        break;
                    case 1: // ADJ SX
                        if(c>0)
                        {
                            v=F_coord_nodo_sx(P,r,c);
                            if(v->murcorr!=0)
                            {
                                F_relax(Heap,u,r,c,r,c-1,v->pDX->peso);
                            }
                        }
                        break;
                    case 2: // ADJ UP
                        if(r>0)
                        {
                            v=F_coord_nodo_up(P,r,c);
                            if(v->murcorr!=0)
                            {
                                F_relax(Heap,u,r,c,r-1,c,v->pDW->peso);
                            }
                        }
                        break;
                    case 3: // ADJ DW
                        if(r<P->rsize-1)
                        {
                            v=F_coord_nodo_dw(P,r,c);
                            if(v->murcorr!=0)
                            {
                                F_relax(Heap,u,r,c,r+1,c,v->pUP->peso);
                            }
                        }
                        break;
                    default:
                        F_error(6);
                        break;
                }
            }

            u->stima=NULL;
            u->ptrDx=NULL;
            u->ptrSx=NULL;

           /* Abilitare nel caso in cui si voglia anche
            * l'informazione delle celle visitate da
            * Dijkstra
            *
               M **matr=P->matrice;
               matr[r][c].visitato=2;
            */
        }
        T=Heap->struttura;
    }

}


/*
 *  Descrizione: procedura di rilassamento degli archi.
 *  Dettagli:    //
 *
 *  Parametri in:   Heap->struttura di gestione heap,
 *                  u->nodo estratto dallo heap,
 *                  rU,cU->riga e colonna del nodo u,
 *                  rV,cV->riga e colonna del nodo adiacente,
 *                  arco->valore numerico dell'arco associato u->v.
 *  Parametri out: //
 *
 *  Chiamante: L_Dijkstra - F_dijkstra.
 *
*/
void F_relax(StructHeap Heap, Albero u, int rU, int cU, int rV, int cV, float arco)
{
    /* Prelevo la matrice delle stime, e la stima di (u) e (v) */
    Dd **Mstime=Heap->d;
    float dV=Mstime[rV][cV].stima;
    float dU=Mstime[rU][cU].stima;

    /*
     * Utilizzo del valore INFINITO
     *
     * Il primo controllo verifica che il nodo preso
     * in esame (u) non abbia stima INFINITO. Questo puo'
     * accadere nel caso in cui ci siano nodi che non sono
     * raggiunti dalla sorgente.
     *
     * Il secondo controllo, invece; siccome nell'inizializzazione
     * tutte le stime di ogni nodo, escluso il nodo sorgente,
     * sono poste a INFINITO, l rilassamento di un arco avviene
     * in uno dei seguenti casi:
     *      - la stima del nodo adiacente (v) e' infinito.
     *      - la stima del nodo adiacente (v) e' maggiore
     *        della stima del nodo di riferimento (u) + l'arco
     *        tra i due, w(u,v).
     */
    if(dU!=INFINITO)
    {
        if(dV==INFINITO || dV > dU + arco)
        {
           /*
            * Ricerca dell'adiacente nello heap.
            * Se presente viene ritornato l'indice in cui
            * si trova, altrimenti ritorna -1.
            */
            int i=F_cerca_indice_decrease(Heap,rV,cV);

            /* Esecuzione della decrase e salvataggio del predecessore */
            F_decrease_key_albero(Heap,i,dU + arco);
            Pp **Mpred=Heap->p;
            Mpred[rV][cV].pred=u;
        }
    }
}


/*
 *  Descrizione:   ricerca di un elemento nell'albero.
 *  Dettagli:      cerca l'esistenza dell'adiacente preso in esame
 *                 all'interno dello heap. Il confronto avviene mediante
 *                 l'uso della riga e colonna in ingresso con quelle
 *                 contenute nel nodo in esame.
 *                 Nel caso in cui l'adiacente sia presente nell'albero si ritorna
 *                 il suo indice, altrimenti il valore -1.
 *
 *  Parametri in:  Heap->struttura di gestione heap,
 *                 r,c->riga e colonna dell'adiacente da trovare nell'albero.
 *  Parametri out: indice dell'elemento se trovato, -1 altrimenti.
 *
 *  Chiamante:     L_Dijkstra - F_dijkstra.
 *
*/
int F_cerca_indice_decrease(StructHeap Heap, int r, int c)
{
    Albero T=Heap->struttura;
    int found=1,indice=-1;


    while (T && found)
    {
       indice++;
       Albero nodo=F_preleva_nodo(Heap,indice); // L_Heap
       if(nodo && nodo->r==r && nodo->c==c) found=0;
    }

    return indice;
}










