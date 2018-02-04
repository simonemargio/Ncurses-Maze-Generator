#include <stdlib.h>
#include "L_Heap.h"
#include "L_Struttura.h"
#include "L_Dijkstra.h"
#include "L_Dijkstra.h"
#include "L_Error.h"

/* Dimensione dell'array contente il valore binario
 * dell'elemento da inserire nello heap.
 *
 * Maggiori informazioni nella documentazione pag.13, 2.3.
 */
#define DIMARRMASK 32


/*
 *  Descrizione: allocazione della struttura per la gestione di heap.
 *  Dettagli:    //
 *
 *  Parametri in:  Heap->struttura gi gestione principale di heap.
 *  Parametri out: Heap->struttura allocata.
 *
 *  Chiamante:     L_Dijkstra - F_init_dijkstra,
 *                 L_Astar - F_init_Astar.
 *
*/
StructHeap F_alloca_heap(StructHeap Heap)
{
    Heap = (struct struttura_gestione_heap *)malloc(sizeof(struct struttura_gestione_heap));
    Heap->struttura=NULL;
    Heap->heapsize=0;
    Heap->p=NULL;
    Heap->p=NULL;

    return Heap;
}


/*
 *  Descrizione: riprisitna la proprieta' di heap.
 *  Dettagli:    //
 *
 *  Parametri in: Heap->struttura principale di heap,
 *                i->indice dell'elemento modificato su cui
 *                verificare ed eventualemente ripristinare
 *                lo stato di heap.
 *
 *  Parametri out: //
 *
 *  Chiamante:     L_Heap - F_estrai_minmax_albero.
 *
*/
void F_heapify(StructHeap Heap, int i)
{
    /*
     * Prende i figli del nodo passat
     * e verifica quali di questi contiene
     * la stima piu' piccola.
     *
     */
    int l=F_HeapSx(i);
    int r = F_HeapDx(i);
    int mas = F_FirstCheck_Albero_Min(Heap,l,i);
    mas = F_SecondCheck_Alebro_Min(Heap,r,mas);

    /* Verifica se il valore minore si trova in un figlio */
    if(mas!=i)
    {
        /*
         * Metto il valore piu' piccolo in testa
         * e mi richiamo su di questo per ripetere
         * il procedimento e verificare di non violare
         * la proprieta' di heap.
         *
         */
        F_scambio_albero(Heap,i,mas);
        F_heapify(Heap,mas);
    }
}


/*
 *  Descrizione:    preleva l'indice del figlio sinistro.
 *  Dettagli:       //
 *
 *  Parametri in:  i->indice dell'elemento in cui prelevare
 *                 il figlio.
 *
 *  Parametri out: i->indice di tale figlio.
 *
 *  Chiamante:     L_Heap - F_heapify.
 *
*/
int F_HeapSx(int i)
{
    i=(2*i)+1;

    return i;
}


/*
 *  Descrizione:    preleva l'indice del figlio destro.
 *  Dettagli:       //
 *
 *  Parametri in:  i->indice dell'elemento in cui prelevare
 *                 il figlio.
 *
 *  Parametri out: i->indice di tale figlio.
 *
 *  Chiamante:     L_Heap - F_heapify.
 *
*/
int F_HeapDx(int i)
{
    i=(2*i)+2;

    return i;
}


/*
 *  Descrizione:    verifica della stima minore in due nodi.
 *  Dettagli:       controlla quali tra il padre 'i' e il figlio
 *                  sinistro 'l' abbia la stima piu' piccola.
 *
 *  Parametri in:   Heap->struttura di gestione heap,
 *                  l->indice figlio sinstro,
 *                  i->indice padre.
 *
 *  Parametri out:  mas->indice del nodo con stima piu' picocla.
 *
 *  Chiamante:      L_Heap - F_heapify.
 *
*/
int F_FirstCheck_Albero_Min(StructHeap Heap,int l,int i)
{
    int mas=i;
    Albero nodo_l=F_preleva_nodo(Heap,l);
    Albero nodo_i=F_preleva_nodo(Heap,i);

    if(l < Heap->heapsize)
    {
        Dd *stima_l=nodo_l->stima;
        Dd *stima_i=nodo_i->stima;

        float priorita_l=stima_l->stima;
        float priorita_i=stima_i->stima;

        /*
         * Maggiori informazioni su tale controllo nella
         * documentazione pag.13, 2.3.
         */
        if ((priorita_i==INFINITO && priorita_l!=INFINITO) || (priorita_l!=INFINITO && priorita_i!=INFINITO && priorita_l < priorita_i)) mas = l;
        else mas = i;
    }

    return mas;
}


/*
 *  Descrizione:    verifica della stima minore in due nodi.
 *  Dettagli:       controlla quali tra il valore piu' piccolo
 *                  preso precedentemente e il figlio
 *                  destro 'r' abbia la stima piu' piccola.
 *
 *  Parametri in:   Heap->struttura di gestione heap,
 *                  mas->indice del nodo con stima minima,
 *                  r->indice figlio destro.
 *
 *  Parametri out:  mas->indice del nodo con stima piu' picocla.
 *
 *  Chiamante:      L_Heap - F_heapify.
 *
*/
int F_SecondCheck_Alebro_Min(StructHeap Heap, int r,int mas)
{
    Albero nodo_r=F_preleva_nodo(Heap,r);
    Albero nodo_mas=F_preleva_nodo(Heap,mas);

    if(r<Heap->heapsize)
    {
        Dd *stima_r=nodo_r->stima;
        Dd *stima_mas=nodo_mas->stima;

        float priorita_r=stima_r->stima;
        float priorita_mas=stima_mas->stima;

        /*
         * Maggiori informazioni su tale controllo nella
         * documentazione pag.13, 2.3.
         */
        if((priorita_r!=INFINITO && priorita_mas==INFINITO) || (priorita_mas!=INFINITO && priorita_r!=INFINITO && priorita_r<priorita_mas)) mas=r;

    }

    return mas;
}



/*
 *  Descrizione:  ignora tutti i bit pari a zero.
 *  Dettagli:     nella conversione dell'indice di inserimento
 *                del nodo vengono ignorati tutti i bit pari a zero
 *                e anche il primo 1 trovato.
 *
 *  Parametri in:  bits->matrice contenente il valore in binario.
 *  Parametri out: j->indice su cui partire per spostarsi a destra
 *                 o sinistra nello heap.
 *
 *  Chiamante:     L_Heap - F_inserisci_nodo_albero.
 *
*/
int F_salta_zero_bit(int *bits)
{
    int j;
    for(j=DIMARRMASK-1;j>=0;j--)
    {
        if(bits[j]==1)
        {
            j--; // Salta anche il primo bit pari a uno
            break;
        }
    }
    return j;
}


/*
 *  Descrizione:    creazie del vettore contenente il valore in binario dell'indice passato.
 *  Dettagli:       //
 *
 *  Parametri in:   n->indice di riferimento.
 *  Parametri out:  bits->vettore col valore n in binario.
 *
 *  Chiamante:      L_Heap - F_inserisci_nodo_albero.
 *
*/
int *F_ottieni_bit(int n)
{
    int *bits = malloc(sizeof(int)* DIMARRMASK);

    int k;
    for(k=0;k<DIMARRMASK;k++)
    {
        int mask = 1 << k;
        int mask_n = n & mask;
        int bit_preso = mask_n >> k;
        bits[k] = bit_preso;
    }
    return bits;
}


/*
 *  Descrizione:    creazione dell'albero heap.
 *  Dettagli:       //
 *
 *  Parametri in:   Heap->struttura di gestione principale dello heap,
 *                  P->struttura di gestione principale,
 *                  r,c->riga e colonna della cella da inserire nelle
 *                  informazioni del nodo heap da creare.
 *
 *  Parametri out:  //
 *
 *  Chiamante:      L_Dijkstra - F_init_dijkstra.
 *
*/
void F_crea_heap(StructHeap Heap, Str P,int r,int c)
{
    int i=0,j=0,realsize=1,cont=1;

    Albero T=NULL;
    M **matrice=P->matrice;

    Dd **d=Heap->d;
    d[r][c].stima=0;

    /*
     * La prima cella scelta randomicamente dall'algoritmo
     * di ricerca viene inserito in testa all'albero heap.
     *
     */
    Albero nuovo_elem=F_crea_nodo_heap(Heap,r,c);
    T=F_inserisci_nodo_albero(T,nuovo_elem,0);

    /* Vengono inseriti tutti gli altri nodi che sono corridoi */
    while (cont<Heap->heapsize)
    {
        if(matrice[i][j].murcorr!=0)
        {
            /* La cella iniziale viene saltata essendo già in radice */
            if(i==r && j==c) { /* puts("Cella iniziale, la salto perche' gia messa all'inizio");*/ cont--;}
            else
            {
                /* Si crea un nuovo nodo contenente la cella trovata */
                nuovo_elem=F_crea_nodo_heap(Heap,i,j);
                T=F_inserisci_nodo_albero(T,nuovo_elem,realsize);
                realsize++;
            }

        }

        /* Si leggono tutte le righe e colonne della matrice */
        if(j>=P->csize-1)
        {
            i++;
            j=0;
        }
        else
            j++;

        cont++;
    }

    /*
     * Si setta la dimensione reale dello heap
     * e lo si aggangia alla struttura di gestione
     * heap.
     */
    Heap->heapsize=realsize;
    Heap->struttura = T;
}

/* Funzione per la creazione di un albero completo */
/*
 *  Descrizione:    inserisce un nodo all'interno dell'albero heap.
 *  Dettagli:       l'inserimento viene effettuato da sinistra a destra
 *                  livello per livello.
 *
 *  Parametri in:   T->albero heap,
 *                  nuovo_elem->nodo da inserire,
 *                  indice->posizione di inserimento.
 *  Parametri out:  T->albero heap.
 *
 *  Chiamante:      L_Heap - F_crea_heap,
 *                  L_Astar - F_init_Astar.
 *
 *
*/
Albero F_inserisci_nodo_albero(Albero T, Albero nuovo_elem, int indice)
{
    /* Posizione di inserimento trovata */
    if(indice == 0)
    {
        T=nuovo_elem;
    }
    else
    {
        /*
         * Si ottiene il numero di posizione del nodo in biniario,
         * si saltano tutti i vit pari a zero e il primo bit pari a uno (radice).
         * I restanti bit permettono di trovare la posizone del nodo
         */
        Albero Tmp = T;
        int *bits = F_ottieni_bit(indice+1);

        int j=F_salta_zero_bit(bits);

        for(;j>=0;j--)
        {
            if(bits[j]==1) // Mi sposto a destra
            {
                   if(Tmp->ptrDx == NULL)
                        Tmp->ptrDx=F_inserisci_nodo_albero(Tmp,nuovo_elem,0);
                   else
                          Tmp=Tmp->ptrDx;
            }
            else // Mi sposto a sinistra
            {
                   if(Tmp->ptrSx == NULL)
                        Tmp->ptrSx=F_inserisci_nodo_albero(Tmp,nuovo_elem,0);
                   else
                        Tmp=Tmp->ptrSx;
            }
        }
        free(bits);
    }
    return T;
}


/*
 *  Descrizione:    allocazione di un nodo dell'albero heap.
 *  Dettagli:       //
 *
 *  Parametri in:   Heap->struttura principale di gestione heap,
 *                  r,c->riga e colonna della cella di riferimento.
 *  Parametri out:  elem->nodo creato.
 *
 *  Chiamante:      L_Heap - F_crea_heap,
 *                  L_Astar - F_init_Astar.
 *
*/
Albero F_crea_nodo_heap(StructHeap Heap,int r,int c)
{
    Dd **d=Heap->d;

    Albero elem=(struct struttura_nodo_albero*)malloc(sizeof(struct struttura_nodo_albero));
    /*
     * Viene collegato l'indirizzo della cella
     * scelta in relazione alla matrice delle stime.
     * Maggiori informazioni nella documentazione
     * pag.14, 2.3.2.
     */
    elem->stima=&d[r][c];
    elem->r=r;
    elem->c=c;
    elem->ptrDx=NULL;
    elem->ptrSx=NULL;

    return elem;
}


/*
 *  Descrizione:    estrazione della radice dallo heap.
 *  Dettagli:       viene estratta la radice, i suoi valori
 *                  sostituiti con l'ultima foglia e poi eventualmente
 *                  ripristinata la proprieta' di heap.
 *
 *  Parametri in:   Heap->struttura di gestione principale di heap.
 *  Parametri out:  newT->nodo contenente la radice estratta.
 *
 *  Chiamante:      L_Dijkstra - F_dijkstra,
 *                  L_Astar - F_Astar.
 *
*/
Albero F_estrai_minmax_albero(StructHeap Heap)
{
    Albero minT=Heap->struttura;
    Albero newT=NULL;

    if(minT)
    {
        newT=(struct struttura_nodo_albero*)malloc(sizeof(struct struttura_nodo_albero));
        newT=F_copia_valori_albero(minT,newT);
        F_scambio_albero(Heap,0,Heap->heapsize-1);
        F_elimina_foglia(Heap);
        F_heapify(Heap,0);
    }
    if(Heap->heapsize<0) { newT=NULL; Heap->struttura=NULL;}

    return newT;
}

/*
 *  Descrizione:   copia le informazioni di due nodi.
 *  Dettagli:      effettua la copia dei valori della radice dello
 *                 heap con il nodo che verra' ritornato per effettuare
 *                 le varie operazioni.
 *
 *  Parametri in:  minT->radice dello heap,
 *                 newT->nodo di ritorno.
 *  Parametri out: newT->nodo contentente le informazioni copiate dalla radice.
 *
 *  Chiamante:     L_Heap - F_estrai_minmax_albero.
 *
*/
Albero F_copia_valori_albero(Albero minT, Albero newT)
{
    int r = minT->r;
    int c = minT->c;
    newT->c=c;
    newT->r=r;
    Dd *stima=minT->stima;
    minT->stima=NULL;
    newT->stima=stima;

    return newT;
}

/*
 *  Descrizione:    eliminazione della foglia.
 *  Dettagli:       elimina l'ultima foglia dello heap contenente
 *                  le informazioni sulla radice precedentemente estratta.
 *
 *  Parametri in:   Heap->struttura di gestione heap.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Heap - F_estrai_minmax_albero.
 *
*/
void F_elimina_foglia(StructHeap Heap)
{
    /* Si preleva la foglia da eliminare e il padre di questa */
    Albero ultima_foglia = F_preleva_nodo(Heap,Heap->heapsize-1);
    Albero padre_foglia = F_preleva_nodo(Heap,((Heap->heapsize-2)/2));

    if(padre_foglia)
    {
        /* Si elimina il legame con il figlio */
        if(padre_foglia->ptrDx != NULL)
        {
            padre_foglia->ptrDx = NULL;
        }
        else{
            if(padre_foglia->ptrSx != NULL)
            {
                padre_foglia->ptrSx = NULL;
            }
        }
    }

    /* Si aggiusta il valore di heapsize e si elimina la foglia */
    Heap->heapsize=((Heap->heapsize)-1);
    if(Heap->heapsize>=0){ultima_foglia->stima=NULL; ultima_foglia->ptrDx=NULL; ultima_foglia->ptrSx=NULL;}
}



/*
 *  Descrizione:    scambia le informazioni di due nodi.
 *  Dettagli:       usato per il ripritino dello stato di heap.
 *
 *  Parametri in:   Heap->struttura di gestione heap,
 *                  i->indice del primo nodo,
 *                  mas->indice del secondo nodo.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Heap - F_Heapify.
 *
*/
void F_scambio_albero(StructHeap Heap,int i,int mas)
{

    Albero nodo_i=F_preleva_nodo(Heap,i);
    Albero nodo_mas=F_preleva_nodo(Heap,mas);

    int c_i = nodo_i->c;
    int r_i = nodo_i->r;
    Dd *stima_i = nodo_i->stima;

    nodo_i->c=nodo_mas->c;
    nodo_i->r=nodo_mas->r;
    nodo_i->stima=nodo_mas->stima;

    nodo_mas->r=r_i;
    nodo_mas->c=c_i;
    nodo_mas->stima=stima_i;
}


/*
 *  Descrizione:    preleva il nodo di indice passato.
 *  Dettagli:       in base al valore di 'indice' preleva il nodo selezionato
 *                  dallo heap.
 *
 *  Parametri in:   Heap->struttura di gestione heap,
 *                  indice->indice del nodo che si vuole ritornare.
 *  Parametri out:  nodo->nodo scelto.
 *
 *  Chiamante:      F_Heap - F_decrease_key_albero, F_elimina_foglia, F_scambio_albero.
 *
*/
Albero F_preleva_nodo(StructHeap Heap,int indice)
{
    Albero nodo = Heap->struttura;


        int *bits = F_ottieni_bit(indice+1); // Ottengo la posizione in binario del nodo

        int j=F_salta_zero_bit(bits); // Salto gli zero e il primo uno in bit

        for(;j>=0;j--)  // Prelevo il nodo spostandomi con i bit restanti
        {
            if(bits[j]==1)
            {
                nodo=nodo->ptrDx;
            }
            else
            {
                nodo=nodo->ptrSx;
            }
        }
        free(bits);

    return nodo;
}


/*
 *  Descrizione:    esegue il decrease nel nodo scelto.
 *  Dettagli:       cambia il valore di stima del nodo con un valore
 *                  piu' piccolo ed esegue il ripristino dello stato
 *                  di heap.
 *
 *  Parametri in:   Heap->struttura di gestione heap,
 *                  i->indice del nodo su cui operare,
 *                  val->nuova stima.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Dijstra - F_relax,
 *                  L_Astar - F_Astar.
 *
*/
void F_decrease_key_albero(StructHeap Heap,int i, float val)
{
    Albero nodo = F_preleva_nodo(Heap,i);
    Dd *stima_n=nodo->stima;

    /*
     * Si esegue il decrease quando il nodo contiene INFINITO,
     * essendo necessariamente val un valore piu' piccolo e quando
     * il valore passato e' minore di quello gia' presente.
     *
     */
    if(stima_n->stima==INFINITO || val < stima_n->stima)
    {
        /* Inserisco la nuova stima */
        stima_n->stima=val;

        /* Provvedo al ripristino della proprieta' heap */
        Albero nodo_padre = F_preleva_nodo(Heap,((i+1)/2)-1);
        Dd *stima_p=nodo_padre->stima;

        while((i > 0 && stima_p->stima==INFINITO) || (i > 0 &&  stima_p->stima > stima_n->stima))
        {
            F_scambio_albero(Heap,i,((i+1)/2)-1);

            i = ((i+1)/2)-1;
            nodo_padre = F_preleva_nodo(Heap,((i+1)/2)-1);
            nodo = F_preleva_nodo(Heap,i);

            stima_p=nodo_padre->stima;
            stima_n=nodo->stima;
        }
    } else F_error(7);
}


/*
 *  Descrizione:  inserisce un nuovo nodo nello heap.
 *  Dettagli:     //
 *
 *  Parametri in:  Heap->struttura di gestione heap,
 *                 nuovo_elemento->elemento da inserire.
 *
 *  Parametri out: //
 *
 *  Chiamante:     L_Astar - F_Astar.
 *
*/
void F_inserisci_elemento_albero(StructHeap Heap,Albero nuovo_elemento) {
    Albero T = Heap->struttura;

    T = F_inserisci_nodo_albero(T,nuovo_elemento,Heap->heapsize);

    Heap->heapsize = Heap->heapsize + 1;
    Heap->struttura = T;

    int i = Heap->heapsize - 1;
    Albero nodo_padre = F_preleva_nodo(Heap, ((i + 1) / 2) - 1);
    Albero nodo = F_preleva_nodo(Heap, i);

    Dd *stima_n = nodo->stima;
    Dd *stima_p = nodo_padre->stima;

    while ((i > 0 && stima_p->stima == INFINITO) || (i > 0 && stima_p->stima > stima_n->stima)) {
        F_scambio_albero(Heap, i, ((i + 1) / 2) - 1);
        i = ((i + 1) / 2) - 1;
        nodo_padre = F_preleva_nodo(Heap, ((i + 1) / 2) - 1);
        nodo = F_preleva_nodo(Heap, i);

        stima_p = nodo_padre->stima;
        stima_n = nodo->stima;
    }
}