#include "L_Divide.h"
#include "L_Utility.h"
#include <stdlib.h>
#include "L_Maze.h"

/*
 *  Descrizione:    inizializzazione prima della procedura Divide.
 *  Dettagli:       siccome il Divide e' l'unico algoritmo che lavora
 *                  su una matrice completamente formata da nodi corridoio,
 *                  vengono eliminati tutti i muri dalla matrice passata.
 *                  Successivamente viene chiamata la procedura di crezione
 *                  del labirinto.
 *  Parametri in:   P->struttura di gestione principale.
 *  Parametri out:  //
 *
 *  Chiamante:     L_Utility - F_stampa_menu_labirinto.
 *
*/
void F_init_divide(Str P)
{
    F_elimina_tutti_muri(P);
    F_divide(P,0,0,P->csize,P->rsize);
}


/*
 *  Descrizione:    generatore a suddivisione ricorsiva.
 *  Dettagli:       //
 *
 *  Parametri in:   P->struttura di gestione principale,
 *                  r,c->riga e colonna della cella da cui partire,
 *                  w,h->altezza e larghezza della stanza su cui
 *                  effettuare la divisione.
 *  Parametri out:  //
 *
 *  Chiamante:      F_Divide - F_init_divide.
 *
*/
void F_divide(Str P,int r, int c,int w, int h)
{
    if(w>0 && h>0)
    {
        if(h>w) F_divide_horizontally(P,r,c,w,h);
        else F_divide_vertically(P,r,c,w,h);
    }
}


/*
 *  Descrizione:    divisione della stanza in verticale.
 *  Dettagli:       //
 *
 *  Parametri in:   P->struttura di gestione principale,
 *                  r,c->riga e colonna della cella di partenza,
 *                  width,height-larghezza e altezza della stanza.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Divide - F_divide.
 *
*/
void F_divide_vertically(Str P, int r, int c, int width,int height)
{
    /* Generazione random della posizione del muro e della porta */
    int w_wall=F_random_even(width);
    int door=F_random_odd(height);

    int i=0;
    for(i=0;i<height;i++)
    {
        /* Si costruiscono muri saltando la porta */
        if (i != door)
        {
            M **matr = P->matrice;
            matr[r+i][c+w_wall].murcorr = 0;

            /* Si scollegano gli archi con la cella diventata muro */
            F_sistema_archi_modifica_labirinto(P,r+i,c+w_wall); // L_Maze
        }
    }

    /* Si richiama sull sottostanza sinistra e poi destra */
    F_divide(P,r,c,w_wall-1,height);
    F_divide(P,r,c+w_wall+2,width-w_wall-2,height);
}


/*
 *  Descrizione:    divisione della stanza in orizzontale.
 *  Dettagli:       //
 *
 *  Parametri in:   P->struttura di gestione principale,
 *                  r,c->riga e colonna della cella di partenza,
 *                  width,height-larghezza e altezza della stanza.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Divide - F_divide.
 *
*/
void F_divide_horizontally(Str P,int r,int c,int width, int height)
{
    /* Generazione random della posizione del muro e della porta */
    int h_wall=F_random_even(height);
    int door=F_random_odd(width);

    int i=0;
    for(i=0;i<width;i++)
    {
        /* Si costruiscono muri saltando la porta */
        if(i!=door)
        {
            M **matr=P->matrice;
            matr[r+h_wall][c+i].murcorr=0;

            /* Si scollegano gli archi con la cella diventata muro */
            F_sistema_archi_modifica_labirinto(P,r+h_wall,c+i);
        }
    }

    /* Si richiama sull sottostanza sinistra e poi destra */
    F_divide(P,r,c,width,h_wall-1);
    F_divide(P,r+h_wall+2,c,width,height-h_wall-2);
}


/*
 *  Descrizione:   scelta del valore di generazione della porta.
 *  Dettagli:      //
 *
 *  Parametri in:  bound->altezza o larghezza della stanza.
 *  Parametri out: val->indice dove non costruire il muro ma lasciare la porta.
 *
 *  Chiamante:     L_Divide - F_divide_vertically, F_divide_horizontally.
 *
*/
int F_random_odd(int bound)
{
    int val=rand()%bound;
    if(val%2==0)
    {
        if(val>0) val=val-1;
        else val=val+1;
    }

    return val;
}


/*
 *  Descrizione:   scelta del valore di generazione del muro.
 *  Dettagli:      //
 *
 *  Parametri in:  bound->altezza o larghezza della stanza.
 *  Parametri out: val->indice dove eseguire la costruzione del muro.
 *
 *  Chiamante:     L_Divide - F_divide_vertically, F_divide_horizontally.
 *
*/
int F_random_even(int bound)
{
    int val=rand()%bound;
    if(val%2==1)
    {
        if(val>0) val=val-1;
        else val=val+1;


    }


    return val;
}



/*
 *  Descrizione:    eliminazione di tutti i muri.
 *  Dettagli:       ogni muro della matrice viene rimosso
 *                  e le celle collegate tra loro.
 *
 *  Parametri in:   P->struttura di gestione principale.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Divide - F_init_divide.
 *
*/
void F_elimina_tutti_muri(Str P)
{
    M **matr=P->matrice;
    int r=0,c=0;

    for(r=0;r<P->rsize;r++)
        for(c=0;c<P->csize;c++)
        {
            /* Se è un muro viene reso corridio */
            if(!matr[r][c].murcorr)
            {
                matr[r][c].murcorr=1;
                F_sistema_archi_modifica_labirinto(P,r,c);
            }
        }
}