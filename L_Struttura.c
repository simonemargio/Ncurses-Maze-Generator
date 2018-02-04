#include "L_Struttura.h"
#include "L_Utility.h"
#include <stdlib.h>



/*
 *  Descrizione:    allocazione della struttura pirncipale.
 *  Dettagli:       struttura di gestione di tutto il programma.
 *
 *  Parametri in:   //
 *  Parametri out:  struttura creata.
 *
 *  Chiamante:      L_Utility - F_start.
 *
*/
Str F_alloca_struttura_principale()
{
    Str P=(struct struttura_principale*)malloc(sizeof(struct struttura_principale));
    P->nsize=0;
    /* RIGA e COLONNA settabili in L_Utility */
    P->rsize=RIGA;
    P->csize=COLONNA;
    P->matrice=NULL;
    P->wMenu=NULL;
    P->wOplab=NULL;
    P->StrHeap=NULL;

    return P;
}


/*
 *  Descrizione: allocazione della matrice.
 *  Dettagli:    effettua l'allocazione della matrice
 *               rappresentativa del labirinto.
 *               La si "collega" alla struttura principale
 *               e la si inizializza.
 *
 *  Parametri in:  P->struttura di gestione principale.
 *  Parametri out: //
 *
 *  Chiamante: L_Utility - F_stampa_menu_labirinto.
 *
*/
void F_alloca_struttura_matrice(Str P)
{
    int i=0;

    M **matrice=(M **)malloc(sizeof(M*)*P->rsize);
    for(i=0;i<P->rsize;i++)
        matrice[i]=(M*)malloc(sizeof(M)*P->csize);

    P->matrice=matrice;

    F_inizializza_matrice(P); // L_Struttura
}


/*
 *  Descrizione:   alloca la struttura di informazione di un nodo.
 *  Dettagli:      documentazione pag.10, 2.2.2.
 *
 *  Parametri in:  r,c->riga e colonna di riferimento.
 *                 p->cella precedente a cui si e' arriva a quella
 *                 in esame.
 *
 *  Parametri out: struttura creata.
 *
 *  Chiamante: L_Maze - F_cella_random, F_seleziona_cella_random,
 *             L_Utility - F_preleva_cella.
 *
*/
InfNodo F_alloca_info_nodo(int r,int c,int p)
{
    InfNodo ind=(struct coordinate_nodo*)malloc(sizeof(struct coordinate_nodo));
    ind->c=c;
    ind->r=r;
    ind->p=p;
    return ind;
}


/*
 *  Descrizione: alloca la struttura per il peso.
 *  Dettagli:    conserva l'informazione del peso tra
 *               due nodi del grafo.
 *
 *  Parametri in: //
 *  Parametri out: struttura generata.
 *
 *  Chiamante:  L_Maze - F_dig_to, F_sistema_archi_modifica_labirinto,
 *              L_Recursive - F_dig_up/dw/sx/dx.
 *
*/
Peso F_alloca_struttura_peso()
{
    Peso strpeso=(struct struttura_peso*)malloc(sizeof(struct struttura_peso));
    float peso=F_genera_peso(1);
    strpeso->peso=peso;

    return strpeso;
}


/*
 *  Descrizione:  inizializza la matrice.
 *  Dettagli:     //
 *
 *  Parametri in:   P->struttura di gestione principale.
 *  Parametri out: //
 *
 *  Chiamante: L_Utility - F_stampa_menu_labirinto.
 *
*/
void F_inizializza_matrice(Str P)
{
    M **matr=P->matrice;

    int i=0,j=0;

    for(i=0;i<P->rsize;i++)
        for(j=0;j<P->csize;j++)
        {
            matr[i][j].murcorr=0;
            matr[i][j].pDW=NULL;
            matr[i][j].pDX=NULL;
            matr[i][j].pSX=NULL;
            matr[i][j].pUP=NULL;
            matr[i][j].visitato=0;
            matr[i][j].inf=NULL;
        }
}


/*
 *  Descrizione:  creazione di una finestra.
 *  Dettagli:     //
 *
 *  Parametri in:  rig,col->dimensioni,
 *                 starty,startx->coordinate di inizio.
 *
 *  Parametri out: finestra creata.
 *
 *  Chiamante: L_Utility - F_crea_finestra_menu, F_genera_menu_labirinto.
 *
*/
WINDOW *F_crea_finesta(int rig, int col, int starty, int startx)
{

    WINDOW *menu_win = newwin(rig, col, starty, startx);

    return menu_win;
}


/*
 *  Descrizione: allocazione matrice delle stime.
 *  Dettagli:    //
 *
 *  Parametri in:  P->struttura di gestione principale.
 *  Parametri out: m->matrice delle stime.
 *
 *  Chiamante: L_Dijkstra - F_init_dijkstra,
 *             L_Astar - F_init_Astar.
 *
*/
Dd **F_alloca_d(Str P)
{
    /*
     * Uso del valore INFINITO:
     * viene usato da Dijkstra durante la crezione
     * dello heap di tutti i nodi che sono corridoi.
     * Lo stesso valore viene usato anche da A* come
     * inizializzazione di tutte le stime non ancora
     * calcolate dalla funzione F.
     */
    int i=0,j=0;

    Dd  **m=(Dd **)malloc(sizeof(Dd*)*P->rsize);
    for(i=0;i<P->rsize;i++)
        m[i]=(Dd*)malloc(sizeof(Dd)*P->csize);

    for(i=0;i<P->rsize;i++)
        for(j=0;j<P->csize;j++)
            m[i][j].stima=INFINITO;

    return m;
}


/*
 *  Descrizione:    allocazione matrice dei predecessori.
 *  Dettagli:       //
 *
 *  Parametri in:   P->struttura di gestione principale.
 *  Parametri out:  m->matrice delle predecessori.
 *
 *  Chiamante:      L_Dijkstra - F_init_dijkstra,
 *                  L_Astar - F_init_Astar.
 *
*/
Pp **F_alloca_p(Str P)
{
    int i=0,j=0;

    Pp  **p=(Pp **)malloc(sizeof(Pp*)*P->rsize);
    for(i=0;i<P->rsize;i++)
        p[i]=(Pp*)malloc(sizeof(Pp)*P->csize);

    for(i=0;i<P->rsize;i++)
        for(j=0;j<P->csize;j++)
            p[i][j].pred=NULL;

    return p;
}



/*
 *  Descrizione: allocazione struttura Heap.
 *  Dettagli:    alloca la struttura principale di gestione di uno Heap.
 *
 *  Parametri in:  P->struttura di gestione principale,
 *                 d->matrice delle stime.
 *                 p->matrice dei predecessori.
 *
 *  Parametri out: Heap->struttura.
 *
 *  Chiamante:     L_Astar - F_Astar, L_Dijkstra - F_Dijkstra.
 *
*/
StructHeap F_alloca_struttura_heap(Str P,Dd **d, Pp **p)
{
    StructHeap Heap=NULL;
    Heap=F_alloca_heap(Heap);
    Heap->p=p;
    Heap->d=d;
    Heap->heapsize=P->csize*P->rsize;

    return Heap;
}


/*
 *  Descrizione:    deallocazione della matrice delle stime.
 *  Dettagli:       //
 *
 *  Parametri in:   Heap->struttura di gestione heap,
 *                  P->struttura di gestione principale.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Astar - F_Astar, L_Dijkstra - F_Dijkstra.
 *
*/
void F_dealloca_d(StructHeap Heap, Str P)
{
    Dd **stime=Heap->d;
    int i=0;

    for(i=0;i<P->rsize;i++)
    {
        Dd *stima=stime[i];
        free(stima);
    }
    Heap->d=NULL;
}


/*
 *  Descrizione:    deallocazione della matrice dei predecessori.
 *  Dettagli:       //
 *
 *  Parametri in:   Heap->struttura di gestione heap,
 *                  P->struttura di gestione principale.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Maze - F_ricerca_percorso.
 *
*/
void F_dealloca_p(StructHeap Heap, Str P)
{
    Pp **pred=Heap->p;
    int i=0;

    for(i=0;i<P->rsize;i++)
    {
        Pp *pre=pred[i];
        free(pre);
    }
    Heap->p=NULL;
}


/*
 *  Descrizione: dealloca tutti gli archi della matrice.
 *  Dettagli:    elimina ogni arco presente ad ogni nodo
 *               del labirinto.
 *
 *  Parametri in:  P->struttura di gestione principale.
 *  Parametri out: //
 *
 *  Chiamante:  L_Utility - F_stampa_menu_labirinto.
 *
*/
void F_dealloca_archi_matrice(Str P)
{
    int r=0,c=0;
    M **matr=P->matrice;

    for(r=0;r<P->rsize;r++)
        for(c=0;c<P->csize;c++)
        {
            /*
             * Per ogni nodo si verifica l'esistenza
             * della struttura peso nei quattro punti
             * cardinali.
             * Per quelli trovati si mette a null il
             * puntatore della cella successiva, si
             * dealloca la struttura peso e si mette a
             * null anche il puntatore della cella di
             * riferimento.
             */
            if(matr[r][c].pDX!=NULL)
            {
                matr[r][c+1].pSX=NULL;
                free(matr[r][c].pDX);
                matr[r][c].pDX=NULL;
            }

            if(matr[r][c].pSX!=NULL)
            {
                matr[r][c-1].pDX=NULL;
                free(matr[r][c].pSX);
                matr[r][c].pSX=NULL;
            }

            if(matr[r][c].pUP!=NULL)
            {
                matr[r-1][c].pDW=NULL;
                free(matr[r][c].pUP);
                matr[r][c].pUP=NULL;
            }

            if(matr[r][c].pDW!=NULL)
            {
                matr[r+1][c].pUP=NULL;
                free(matr[r][c].pDW);
                matr[r][c].pDW=NULL;
            }
        }
}


/*
 *  Descrizione:    dealloca la struttura di informazioni sulle celle.
 *  Dettagli:       Maggiori informazioni su l'utilizzo della struttura
 *                  nel manuale a pag.10, 2.2.2.
 *
 *  Parametri in:   P->struttura di gestione principale.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Utility - F_stampa_menu_labirinto.
 *
*/
void F_dealloca_info_cell(Str P)
{
    M **matr=P->matrice;
    int r=0,c=0;

    for(r=0;r<P->rsize;r++)
        for(c=0;c<P->csize;c++)
        {
            if(matr[r][c].inf!=NULL)
            {
                free(matr[r][c].inf);
                matr[r][c].inf=NULL;
            }
        }
}


/*
 *  Descrizione: deallocazione della struttura principale.
 *  Dettagli:    in uscita dal programma vengono deallocate
 *               tutte le strutture che sono state usate.
 *
 *  Parametri in:  P->struttura di gestione principale.
 *  Parametri out: //
 *
 *  Chiamante: L_Utility - F_start, F_stampa_menu_principale.
 *
*/
void F_dealloca_strutture(Str P)
{
    if(P)
    {
        if(P->matrice) {F_delloca_matrice(P); free(P->matrice); P->matrice=NULL;}
        if(P->StrHeap) {F_dealloca_heap(P); free(P->StrHeap); P->StrHeap=NULL; }
    }

    P=NULL;
    free(P);
}


/*
 *  Descrizione: dealloca la matrice di riferimento.
 *  Dettagli:    //
 *
 *  Parametri in:  P->struttura di gestione principale.
 *  Parametri out: //
 *
 *  Chiamante: L_Struttura - F_dealloca_strutture.
 *
*/
void F_delloca_matrice(Str P)
{
    M **matr=P->matrice;
    int r=0;
    F_dealloca_archi_matrice(P);
    for(r=0;r<P->rsize;r++) free(matr[r]);
}


/*
 *  Descrizione: deallocazione della struttura Heap.
 *  Dettagli:    effettua la deallocazione delle due matrici
 *               (matrice dei predecessori e matrice delle stime),
 *               successivamente effettua l'eliminazione dell'albero
 *               heap.
 *
 *  Parametri in:  P->struttura di gestione principale.
 *  Parametri out: //
 *
 *  Chiamante: L_Struttura - F_dealloca_strutture.
 *
*/
void F_dealloca_heap(Str P)
{
    StructHeap Heap=P->StrHeap;
    if(Heap->p) F_dealloca_p(Heap,P);
    if(Heap->d) F_dealloca_d(Heap,P);
    if(Heap->struttura) {F_dealloca_albero_heap(Heap->struttura); free(Heap->struttura); Heap->struttura=NULL;}
}


/*
 *  Descrizione: dealloca l'intero albero heap.
 *  Dettagli:    //
 *
 *  Parametri in:  T->albero heap di riferimento.
 *  Parametri out: //
 *
 *  Chiamante: L_Maze - F_ricerca_percorso,
 *             L_Struttura - F_dealloca_strutture.
 *
*/
void F_dealloca_albero_heap(Albero T)
{
    if(T)
    {
        F_dealloca_albero_heap(T->ptrSx);
        F_dealloca_albero_heap(T->ptrDx);
        /*
         * T->stima
         * Non viene deallocato ma solo messa
         * a null in quanto essa e' un puntatore
         * alla matrice di stime generata.
         * Questa verra' poi deallocato mediante
         * un'altra funzione.
         */
        T->stima=NULL;
        free(T);
    }
}


