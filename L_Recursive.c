#include "L_Recursive.h"
#include "L_Utility.h"
#include <stdlib.h>
#include "L_Error.h"
#include "L_Maze.h"


/*
 *  Descrizione: inizializzazione del generatore Recursive.
 *  Dettagli:    procede a selezionare una cella random dal
 *               labirinto e successivamente chiama l'algoritmo.
 *
 *  Parametri in:   P->strututra di gestione principale.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Utility - F_stampa_menu_labirinto.
 *
*/
void F_init_recursive_maze(Str P)
{
    /*
     * Seleziona la cella random, non servendosi
     * della cella stessa si preleva solo la sua
     * riga a colonna.
     *
     */
    M *cell=F_cella_random(P);
    int r=cell->inf->r;
    int c=cell->inf->c;
    free(cell->inf);
    cell->inf=NULL;

    F_recursive_maze(P,r,c);
}


/*
 *  Descrizione:    algoritmo di tipo Recursive.
 *  Dettagli:       //
 *
 *  Parametri in:   P->struttura di gestione principale,
 *                  r,c->riga e colonna cella di partenza.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Recursive - F_init_recursive_maze.
 *
*/
void F_recursive_maze(Str P, int r, int c)
{
    /*
     * Viene usato un vettore contenente valori
     * casuali da 0 a 3 (senza occorrenze). Ognuno
     * di questi indica la posizione in cui provare
     * a spostarsi ed eventualmente scavare.
     */
    int i=0;
    int *randDirs=F_direzioni_random();

    for(i=0;i<4;i++)
    {
        switch (randDirs[i])
        {
            case 0: // DX
                if(F_dig_dx(P,r,c)) F_recursive_maze(P,r,c+2);
                break;
            case 1: // SX
                if(F_dig_sx(P,r,c)) F_recursive_maze(P,r,c-2);
                break;
            case 2: // UP
                if(F_dig_up(P,r,c)) F_recursive_maze(P,r-2,c);
                break;
            case 3: // DW
                if(F_dig_dw(P,r,c)) F_recursive_maze(P,r+2,c);
                break;
            default:
                F_error(10);
                break;
        }
    }


}


/*
 *  Descrizione:    collega, se possibile, le celle dall'alto.
 *  Dettagli:       //
 *
 *  Parametri in:   P->struttura di gestione principale,
 *                  r,c->riga e colona della cella presa in esame su
 *                  cui verificare la possibilita' di scavare verso il
 *                  suo adiacente.
 *
 *  Parametri out:  1->le celle sono state collegate, 0->altrimenti.
 *
 *  Chiamante:      L_Recursive - F_recursive_maze.
 *
 */
int F_dig_up(Str P, int r,int c)
{
    /*
     * Si verifica se c'e' la possibilita' di
     * prendere l'adiacente. Caso affermativo si
     * verifica che sia un muro, se lo e' le celle
     * vengono collegate.
     *
     */
    if(r-1>0)
    {
        M **matr=P->matrice;

        if(matr[r][c-2].murcorr!=1)
        {
            Peso pes1=F_alloca_struttura_peso();
            Peso pes2=F_alloca_struttura_peso();

            matr[r-2][c].murcorr=1;
            matr[r-1][c].murcorr=1;

            matr[r][c].pUP=pes1;
            matr[r-1][c].pDW=pes1;
            matr[r-1][c].pUP=pes2;
            matr[r-2][c].pDW=pes2;

            return 1;
        }


    }
     return 0;
}


/*
 *  Descrizione:    collega, se possibile, le celle dal basso.
 *  Dettagli:       //
 *
 *  Parametri in:   P->struttura di gestione principale,
 *                  r,c->riga e colona della cella presa in esame su
 *                  cui verificare la possibilita' di scavare verso il
 *                  suo adiacente.
 *
 *  Parametri out:  1->le celle sono state collegate, 0->altrimenti.
 *
 *  Chiamante:      L_Recursive - F_recursive_maze.
 *
 */
int F_dig_dw(Str P, int r, int c)
{
    if(r+1<P->rsize-1)
    {
        M **matr=P->matrice;

        if(matr[r+2][c].murcorr!=1)
        {
            Peso pes1=F_alloca_struttura_peso();
            Peso pes2=F_alloca_struttura_peso();

            matr[r+2][c].murcorr=1;
            matr[r+1][c].murcorr=1;

            matr[r][c].pDW=pes1;
            matr[r+1][c].pUP=pes1;
            matr[r+1][c].pDW=pes2;
            matr[r+2][c].pUP=pes2;
            return 1;
        }
    }

    return 0;
}


/*
 *  Descrizione:    collega, se possibile, le celle da sinistra.
 *  Dettagli:       //
 *
 *  Parametri in:   P->struttura di gestione principale,
 *                  r,c->riga e colona della cella presa in esame su
 *                  cui verificare la possibilita' di scavare verso il
 *                  suo adiacente.
 *
 *  Parametri out:  1->le celle sono state collegate, 0->altrimenti.
 *
 *  Chiamante:      L_Recursive - F_recursive_maze.
 *
 */
int F_dig_sx(Str P,int r, int c)
{
    if(c-1>0)
    {
        M **matr=P->matrice;

        if(matr[r][c-2].murcorr!=1)
        {
            Peso pes1=F_alloca_struttura_peso();
            Peso pes2=F_alloca_struttura_peso();

            matr[r][c-2].murcorr=1;
            matr[r][c-1].murcorr=1;

            matr[r][c].pSX=pes1;
            matr[r][c-1].pDX=pes1;
            matr[r][c-1].pSX=pes2;
            matr[r][c-2].pDX=pes2;

            return 1;
        }
    }
    return 0;
}


/*
 *  Descrizione:    collega, se possibile, le celle da destra.
 *  Dettagli:       //
 *
 *  Parametri in:   P->struttura di gestione principale,
 *                  r,c->riga e colona della cella presa in esame su
 *                  cui verificare la possibilita' di scavare verso il
 *                  suo adiacente.
 *
 *  Parametri out:  1->le celle sono state collegate, 0->altrimenti.
 *
 *  Chiamante:      L_Recursive - F_recursive_maze.
 *
 */
int F_dig_dx(Str P,int r, int c)
{
    if(c+1<P->csize-1)
    {
        M **matr=P->matrice;
        if(matr[r][c+2].murcorr!=1)
        {
            Peso pes1=F_alloca_struttura_peso();
            Peso pes2=F_alloca_struttura_peso();

            matr[r][c+2].murcorr=1;
            matr[r][c+1].murcorr=1;

            matr[r][c].pDX=pes1;
            matr[r][c+1].pSX=pes1;
            matr[r][c+1].pDX=pes2;
            matr[r][c+2].pSX=pes2;
            return 1;
        }
    }
    return 0;
}


/*
 *  Descrizione:    genera un array con valori casuali.
 *  Dettagli:       crea un array ordinato per poi mischiare
 *                  i valori e ritornarlo alla funzione per
 *                  iniziare lo scavo tra le celle.
 *
 *  Parametri in:   //
 *  Parametri out:  array->array contenente le direzioni random.
 *
 *  Chiamante:       L_Recursive - F_recursive_maze.
 *
*/
int *F_direzioni_random()
{
    int i=0;
    int *array=malloc(sizeof(int)*4);

    for(i=0;i<4;i++) array[i]=i;

    for (i= 0;i<4;i++) {
        int temp = array[i];
        int indice = rand() % 4;

        array[i]=array[indice];
        array[indice]=temp;
    }
    return array;
}
