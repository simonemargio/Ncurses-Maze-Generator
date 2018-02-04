#include "L_Astar.h"
#include "L_Struttura.h"
#include "L_Heap.h"
#include "L_Utility.h"
#include "L_Error.h"
#include "L_Dijkstra.h"
#include <stdlib.h>
#include "L_Maze.h"


/*
 *  Descrizione: inizializzazione prima di eseguire A*.
 *  Dettagli:    vengono create le strutture per la gestione
 *               delle stime e dei predecessori e l'albero
 *               heap per gestire l'estrazione del valore con
 *               stima minima.
 *
 *  Parametri in: P->struttura di gestione principale,
 *                r,c->riga e colonna della cella di partenza,
 *                rA,cA->riga e colonna della cella si arrivo.
 *
 *  Parametri out: //
 *
 *  Chiamante: L_Maze - F_ricerca_percorso.
 *
*/
void F_init_Astar(Str P,int r, int c, int rA,int cA)
{
    /* Allocazione stime, predessori e struttura Heap */
    Albero T=NULL;
    Dd **d=F_alloca_d(P); // L_Struttura
    Pp **p=F_alloca_p(P); // L_Struttura
    StructHeap Heap=F_alloca_struttura_heap(P,d,p);

    /* Calcolo della distanza H di manh. F=G+H->F=0+H per il nodo iniziale */
    d[r][c].stima=F_distanza_manhattan(r,c,rA,cA);

    /* Creazione dell'albero heap contenente la sorgente */
    Albero nuovo_elem=F_crea_nodo_heap(Heap,r,c); // L_Heap
    T=F_inserisci_nodo_albero(T,nuovo_elem,0);    // L_Heap

    /* Inserimento dell'albero nella struttra principale */
    Heap->struttura=T;
    Heap->heapsize=1;

    /* A* */
    F_Astar(P,Heap,r,c,rA,cA);

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
 *  Descrizione:   algoritmo A*.
 *  Dettagli:      //
 *
 *  Parametri in: P->struttura di gestione principale,
 *                Heap->struttura di gestione dello heap,
 *                rP,cP->riga e colonna cella di partenza,
 *                rA,cA->riga e colonna cella di arrivo.
 *
 *  Parametri out: //
 *
 *  Chiamante: L_Astar - F_init_Astar.
 *
*/
void F_Astar(Str P,StructHeap Heap,int rP,int cP,int rA,int cA)
{
    /* Si preleva l'albero haep */
    Albero T=Heap->struttura;

    /*
     * Found: 1->nodo di arrivo non trovato,
     *        2->altrimenti.
     */
    int found=1;

    while(T && found)
    {
        /* Estrazione dell'elemento con stima piu' piccola */
        Albero x=F_estrai_minmax_albero(Heap); // L_Heap

        if(x)
        {
            /* Verifica della cella prelevata con quella di destinazione */
            if(x->r==rA && x->c==cA) found=0;
            else
            {
                /*
                 * Variabili presenti:
                 *
                 * i->indice di selezione dell'adiacente,
                 *
                 * rX,cX->riga e colonna della cella estratta dallo heap,
                 * rY,cY->riga e colonna di una possibile cella adiacente a x,
                 *
                 * indice->indice di controllo per indicare se l'adiacente trovato
                 *         non e' presente (-1) o e' presente (valore!=1) nello heap,
                 *
                 * peso->valore numerico dell'arco presente tra due nodi,
                 * cost->contiene il valore calcolato dalla funzione F=G+H,
                 *
                 * d,p->struttura contenente la matrice delle stime e la
                 *      matrice dei predecessori,
                 * matr->matrice di riferimento rappresentante il labirinto,
                 * y->contiene il nodo adiacente a x.
                 *
                 *
                 */
                int i=0,rX=x->r,cX=x->c,rY=0,cY=0,indice=0;
                float peso=-1,cost=0;
                Dd **d=Heap->d; Pp **p=Heap->p;
                M **matr=P->matrice; M *y=NULL;

                for(i=0;i<4;i++)
                {
                    /*
                     * Per il nodo x preso si controlla l'esistenza
                     * di ogni adiacente nei quattro punti cardinali.
                     * Se l'adiacente e' presente, ne vengono salvate le
                     * informazioni sulla sua riga e colonna e l'arco/peso
                     * in corrispondenza con il nodo x di riferimento.
                     *
                     */
                    switch(i)
                    {
                        case 0: // ADJ DX
                            if(cX<P->csize-1)
                            {
                                y=F_coord_nodo_dx(P,rX,cX);
                                if(y->murcorr!=0)
                                {
                                    rY=rX; cY=(cX+1);
                                    peso=matr[rX][cX].pDX->peso;
                                }
                            }
                            break;

                        case 1: // ADJ SX
                            if(cX>0)
                            {
                                y=F_coord_nodo_sx(P,rX,cX);
                                if(y->murcorr!=0)
                                {
                                    rY=rX; cY=(cX-1);
                                    peso=matr[rX][cX].pSX->peso;
                                }
                            }
                            break;

                        case 2:  // ADJ UP
                            if(rX>0)
                            {
                                y=F_coord_nodo_up(P,rX,cX);
                                if(y->murcorr!=0)
                                {
                                    rY=(rX-1); cY=cX;
                                    peso=matr[rX][cX].pUP->peso;
                                }
                            }
                            break;

                        case 3: // ADJ DW
                            if(rX<P->rsize-1)
                            {
                                y=F_coord_nodo_dw(P,rX,cX);
                                if(y->murcorr!=0)
                                {
                                    rY=(rX+1); cY=cX;
                                    peso=matr[rX][cX].pDW->peso;
                                }
                            }
                            break;

                        default:
                            F_error(6);
                            break;
                    } // Fine switch

                    /* Si verifica l'esistenza dell'adiacente di riferimento in base a 'i' */
                    if(peso!=-1)
                    {
                        /*
                         * Calcolo del costo
                         *
                         * cost = g(y) + h(y,target), dove:
                         *
                         * g(y) = g(x) + w(x,y) -> stima di x + peso
                         *
                         * h(y,target) -> distanza di manhattan da y alla destinazione.
                         */
                        cost=d[rX][cX].stima+peso+F_distanza_manhattan(rY,cY,rA,cA);

                        /*
                         * Ricerca dell'adiacente nello heap.
                         * Se presente viene ritornato l'indice in cui
                         * si trova, altrimenti ritorna -1.
                         */
                        indice=F_cerca_indice_Astar(Heap,rY,cY);

                        /*
                         * Verifica che il nodo adiacente non sia gia' stato inserito nella closed.
                         * Maggiori informazioni nella documentazione a pag.15, 3.2.
                         */
                        if(matr[rY][cY].visitato!=2)
                        {
                            /*
                             * Analisi delle due condizioni nell'if:
                             *
                             * Prima condizione: la stima di tutti i nodi (escluso quello sorgente) e' inizializzata
                             * a vuoto/INFINITO, nel caso in cui l'adj di riferimento e' presente nell'albero heap (valore indice!=1)
                             * e la sua stima e' vuoto/INFINITO allora significa che si e' trovato NECESSARIAMENTE un valore
                             * di costo migliore per raggiungere il nodo, si effettua quindi il decrease.
                             *
                             * Seconda condizione: rappresenta la condizione "predefinita" dell'algoritmo A*. Ovvero se il nodo
                             * dell'adiacente preso in esame appartiene alla open/heap (indice!=1) e il costo calcolato e' migliore
                             * di quello che e' gia' presente, allora si effettua il decrease.
                             *
                             */
                            if((d[rY][cY].stima==INFINITO && indice!=-1) || (indice!=-1 && cost < d[rY][cY].stima))
                            {
                                F_decrease_key_albero(Heap,indice,cost); // L_Heap

                                /* "Salvataggio" informazione del predecessore */
                                p[rY][cY].pred=x;
                            }
                            else /* L'adiacente non e' presente nello heap oppure il costo calcolato e' maggiore della stima gia' presente */
                            {
                                /* Si verifica che l'adiacente sia presente nell'albero */
                                if(indice==-1)
                                {
                                    /* Si inserisci il costo calcolato */
                                    d[rY][cY].stima=cost;

                                    /* Viene creato il nodo contenente le informazioni e inserito nello heap */
                                    Albero nuovo_elem=F_crea_nodo_heap(Heap,rY,cY);
                                    F_inserisci_elemento_albero(Heap,nuovo_elem);
                                    p[rY][cY].pred=x;
                                }
                            }
                        }
                    }
                    /* Si ripristina il costo allo stato iniziale e si inserisce il nodo x nella "closed" */
                    peso=-1;
                } // Fine for
                matr[rX][cX].visitato=2;
            }
        }
         T=Heap->struttura;
    } // Fine while
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
 *  Chiamante:     L_Astar - F_Astar.
 *
*/
int F_cerca_indice_Astar(StructHeap Heap, int r,int c)
{
    Albero T=Heap->struttura;
    int found=1,indice=-1,cont=Heap->heapsize;

    //  printf("\nHEAPSIZE:|%d|\n",cont);

    while (T && cont && found)
    {
        indice++;
        Albero nodo=F_preleva_nodo(Heap,indice); // L_Heap
        if(nodo && nodo->r==r && nodo->c==c) found=0;
        cont--;
    }

    if(found==1) indice=-1;

    return indice;
}


/*
 *  Descrizione:    calcolo della sitanza di Manhattan.
 *  Dettagli:       sulla base del concetto geometrico della
 *                  geometria del taxi e delle proprieta' di
 *                  questa, viene calcolata la distanza.
 *                  Maggiori infomazioni nella documentazione
 *                  a pag.15, 3.1.
 *
 *  Parametri in:   rP,cP->riga e colonna cella di riferimento,
 *                  rA,cA->rica e colonna del target.
 *  Parametri out:  distanza di Manh.
 *
 *  Chiamante:      L_Astar - F_Astar.
 *
*/
float F_distanza_manhattan(int rP,int cP,int rA,int cA)
{
    return (abs(rA-rP)+abs(cA-cP));
}


