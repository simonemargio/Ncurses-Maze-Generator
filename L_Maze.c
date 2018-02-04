#include <stdlib.h>
#include "L_Utility.h"
#include "L_Maze.h"
#include "L_Error.h"
#include "L_Astar.h"


/*
 *  Descrizione: selezione di una cella random.
 *  Dettagli:    sceglie in maniera randomica una riga
 *               e colonna per poi ritornare l'indirizzo
 *               della cella scelta.
 *
 *  Parametri in:  P->struttura di gestione principale.
 *  Parametri out: cella della matrice.
 *
 *  Chiamante:     L_Prima - F_prim_maze, L_Dfs - F_dfs_maze,
 *                 L_Recursive - F_init_recursive_maze.
 *
*/
M *F_cella_random(Str P)
{
    M **matr=P->matrice;
    int r=F_genera_int_random(P->rsize);
    int c=F_genera_int_random(P->csize);
    /*
     * Vengono inserite le informazioni della riga
     * e colonna nella struttura della cella.
     * Il parametro 0 è un valore del tutto inutilizzato
     * in questo caso. Maggiori dettagli su tale parametro
     * nella documentazione a pag. 10, 2.2.2.
     */
    matr[r][c].inf=F_alloca_info_nodo(r,c,0);
    matr[r][c].murcorr=1;

    return &matr[r][c];
}


/*
 *  Descrizione: verifica adiacenti unvisited.
 *  Dettagli:    Procede a prelevare tutti gli adiacenti
 *               della cella passata in ingresso per verificare
 *               se qualcuno di questi sia unvisited.
 *
 *  Parametri in:  P->struttura principale, cell->cella per
 *                 controllare gli adiacenti.
 *
 *  Parametri out: Valore numerico indicante l'esistenza di
 *                 un nodo unvisited. Ritorna 1: caso affermativo,
 *                 0 altrimenti.
 *
 *  Chiamante:     L_Prim - F_prim_maze, L_Dfs - F_dfs_maze.
 *
*/
int F_controllo_unvisited(Str P,M *cell)
{
    /* Vengono prelevate le informazioni di riga e colonna */
    int r=cell->inf->r;
    int c=cell->inf->c;
    int i=0,check=0;
    M *adj=NULL;

    for(i=0;i<4;i++)
    {
        /*
         * Per ogni adiacente, se presente, viene verificata
         * la sua condizione di nodo visited o unvisited.
         * F_controllo_nodo_unvisited verifica che l'adiacente
         * abbia tutti i puntatori dei pesi a NULL, caso affermativo
         * ritorna il valore 1, specificando a tale procedura che è
         * presente un nodo unvisited.
         *
         */
        switch (i)
        {
            case 0: // Dx
                if(c+1<P->csize-1)
                {
                    adj=F_coord_nodo_dx(P,r,c+1);
                    if(adj!=NULL) check=F_controllo_nodo_unvisited(adj);
                }
                break;
            case 1: // SX
                if(c-1>0)
                {
                    adj=F_coord_nodo_sx(P,r,c-1);
                    if(adj!=NULL) check=F_controllo_nodo_unvisited(adj);
                }
                break;
            case 2: // UP
                if(r-1>0)
                {
                    adj=F_coord_nodo_up(P,r-1,c);
                    if(adj!=NULL) check=F_controllo_nodo_unvisited(adj);
                }
                break;
            case 3: // DW
                if(r+1<P->rsize-1)
                {
                    adj=F_coord_nodo_dw(P,r+1,c);
                    if(adj!=NULL) check=F_controllo_nodo_unvisited(adj);
                }
                break;
            default:
                F_error(3);
                break;
        }
        /* Se vera è presente un nodo unvisited */
        if(check) break;
    }
    return check;
}


/*
 *  Descrizione: verifica della condizione unvisited.
 *  Dettagli:    //
 *
 *  Parametri in:  cell->cella su cui effettuare la verifica.
 *  Parametri out: 1: la cella e' unvisited, 0: altrimenti.
 *
 *  Chiamante:     L_Maze - F_controllo_unvisited.
 *
*/
int F_controllo_nodo_unvisited(M *cell)
{
    /* Basta che uno dei puntatori sia diverso da NULL per rendere la cella visited */
    if(cell->pUP!=NULL || cell->pSX!=NULL || cell->pDX!=NULL || cell->pDW!=NULL)
        return 0;
    return 1;
}


/*
 *  Descrizione: seleziona un adiacente casuale.
 *  Dettagli:    in base alla cella passata in ingresso,
 *               preleva tutti i suoi adiacenti e ne seleziona
 *               uno casuale.
 *
 *  Parametri in:  P->struttura principale, cell->cella su cui prendere
 *                 gli adiacenti.
 *
 *  Parametri out: cRand->cella adiacente casuale.
 *
 *  Chiamante:     L_Prim - F_prim_maze, L_Dfs - F_dfs_maze.
 *
*/
M *F_adj_random(Str P,M *cell)
{
    M *cDx=NULL, *cSx=NULL, *cUP=NULL, *cDW=NULL;
    int r=cell->inf->r;
    int c=cell->inf->c;

    /* Recupero degli adiacenti */
    if(c+1<P->csize-1) cDx=F_coord_nodo_dx(P,r,c+1);
    if(c-1>0) cSx=F_coord_nodo_sx(P,r,c-1);
    if(r-1>0) cUP=F_coord_nodo_up(P,r-1,c);
    if(r+1<P->rsize-1) cDW=F_coord_nodo_dw(P,r+1,c);

    /* Valore random dell'adiacente che verrà selezionato */
    int rand=F_genera_int_random(4);

    /* Seleziona l'adiacente random */
    M *cRand=F_seleziona_cella_random(P,rand,cDx,cSx,cUP,cDW,cell);

    return cRand;
}


/*
 *  Descrizione:    seleziona l'adiacente random.
 *  Dettagli:       è possibile che l'adiacente selezionato
 *                  come random in base al valore rand generato
 *                  non esista. Si procede quindi a selezionare
 *                  il primo adiacente che si trova.
 *
 *  Parametri in:   P->struttura principale, rand->valore numerico corrispondente
 *                  a un tipo di adicente (SX, DX, UP, DW),
 *                  cXX e altri->adiacenti, cell->cella di riferimento.
 *
 *  Parametri out:  cXX->adiacente trovato.
 *
 *  Chiamante:
 *
*/
M *F_seleziona_cella_random(Str P, int rand, M *cDx, M *cSx, M* cUP, M *cDw, M *cell)
{
    int r=cell->inf->r;
    int c=cell->inf->c;

    /*
     * Se il valore random generato riporta un adiacente
     * che esiste allora si provvede a generare le sue informazioni
     * sulla posizione e a ritornale.
     * Viene usato il terzo parametro nuemerico 'p' che corrisponde a:
     * 0: la cella iniziale si trova sinistra.
     * 1: la cella iniziale si trova a destra.
     * 2: la cella iniziale si trova sotto.
     * 3: la cella iniziale si trova sopra.
     *
     * Ulteriori informazioni sull'uso di tale valore nella documentazione
     * a pag. 10, 2.2.2.
     */
    switch (rand)
    {
        case 0:
            if(cDx!=NULL)
            {
                cDx->inf=F_alloca_info_nodo(r,c+2,0);

                return cDx;
            }
            break;
        case 1:
            if(cSx!=NULL)
            {
                cSx->inf=F_alloca_info_nodo(r,c-2,1);

                return cSx;
            }
            break;
        case 2:
            if(cUP!=NULL)
            {
                cUP->inf=F_alloca_info_nodo(r-2,c,2);

                return cUP;
            }
            break;
        case 3:
            if(cDw!=NULL)
            {
                cDw->inf=F_alloca_info_nodo(r+2,c,3);

                return cDw;
            }
            break;
        default:
            F_error(2);
            break;
    }

    /*
     * Caso in cui al valore random generato non corrisponde
     * ad un adiacente, viene selezionato quello successivo.
     *
     */
    if(rand>=3) F_seleziona_cella_random(P,0,cDx,cSx,cUP,cDw,cell);
    else F_seleziona_cella_random(P,rand+1,cDx,cSx,cUP,cDw,cell);
}


/*
 *  Descrizione:   scava tra le celle.
 *  Dettagli:      effettua il collegamento delle
 *                 celle passate in ingresso.
 *
 *  Parametri in:  P->struttura principale,
 *                 cell, newcell->celle da collegare.
 *  Parametri out: //
 *
 *  Chiamante:     L_Prim - F_prim_maze, L_Dfs - F_dfs_maze.
 *
*/
void F_dig_to(Str P, M *cell, M *newcell)
{
    /*
     * Vengono allocati due pesi in quanto si procede
     * a collegare tre celle.
     * Maggiori informazioni nella documentazione a pag 20,
     * 4.1.
     */
    Peso pes=F_alloca_struttura_peso();
    Peso pesmezz=F_alloca_struttura_peso();
    M *mezzo=NULL;

    switch (newcell->inf->p)
    {
        case 0: // DX
            mezzo=F_coord_nodo_dx(P,cell->inf->r,cell->inf->c);
            cell->pDX=pesmezz;
            mezzo->pSX=pesmezz;
            mezzo->pDX=pes;
            newcell->pSX=pes;

            break;
        case 1: // SX
            mezzo=F_coord_nodo_sx(P,cell->inf->r,cell->inf->c);
            cell->pSX=pesmezz;
            mezzo->pDX=pesmezz;
            mezzo->pSX=pes;
            newcell->pDX=pes;

            break;
        case 2:  // UP
            mezzo=F_coord_nodo_up(P,cell->inf->r,cell->inf->c);
            cell->pUP=pesmezz;
            mezzo->pDW=pesmezz;
            mezzo->pUP=pes;
            newcell->pDW=pes;

            break;
        case 3: // DW
            mezzo=F_coord_nodo_dw(P,cell->inf->r,cell->inf->c);
            cell->pDW=pesmezz;
            mezzo->pUP=pesmezz;
            mezzo->pDW=pes;
            newcell->pUP=pes;

            break;
        default:
            F_error(1);
            break;
    }
    /* Si settano le celle come corridoi */
    mezzo->murcorr=1;
    newcell->murcorr=1;
}


/*
 *  Descrizione:    eliminazione di vicoli ciechi.
 *  Dettagli:       provvede alla elimazione casuale
 *                  di muri di una cella con 3 muri.
 *
 *  Parametri in:   P->struttura  di gestione principale.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Utility - F_stampa_menu_labirinto.
 *
*/
void F_elimina_vicoli_ciechi(Str P)
{
    M **matr=P->matrice;
    int r=0,c=0,muri=0;

    /*
     * L'analisi viene effettuata ignorando
     * le celle di contorno.
     */
    for(r=1;r<P->rsize-1;r++)
        for(c=1;c<P->csize-1;c++)
        {
            if(matr[r][c].pSX==NULL) muri++;
            if(matr[r][c].pDX==NULL) muri++;
            if(matr[r][c].pDW==NULL) muri++;
            if(matr[r][c].pUP==NULL) muri++;

            if(muri==3)
            {
                /*
                 * nMuri rappresenta il numero di muri che
                 * verrà eliminato.
                 * nRand indica da dove partite per l'elimanzione
                 * del muro (es: 0=muro DX, 1=muro SX ecc...).
                 * Maggiori informazioni documentazione pag. 20, 4.1.1.
                 */
                int nRand=0;
                int nMuri=F_genera_int_random(4);
                if(nMuri!=0) nRand=F_genera_int_random(4);

                switch (nMuri)
                {
                    case 0: // 0 muro
                        // Se si vuole una maggiore eliminazione di vicoli ciechi abilitare la funzione sotto
                        //F_elimina_muri(P,r,c,nMuri+3,nRand);
                        break;
                    case 1: // 1 muri
                        F_elimina_muri(P,r,c,nMuri,nRand);
                        break;
                    case 2: // 2 muri
                        F_elimina_muri(P,r,c,nMuri,nRand);
                        break;
                    case 3: // 3 muri
                        F_elimina_muri(P,r,c,nMuri,nRand);
                        break;
                    default:
                        F_error(9);
                        break;
                }
            }
            muri=0;
        }
}


/*
 *  Descrizione:    elinazione di un muro.
 *  Dettagli:       //
 *
 *  Parametri in:   P->struttura di gestione principale,
 *                  r,c->riga e colonna della che e' rappresenta
 *                  il vicolo cieco,
 *                  nMuri->numero di muri da eliminare,
 *                  nRand->valore di partenza dove eliminare il muro.
 *
 *  Parametri out:  //
 *
 *  Chiamante:      L_Maze - F_elimina_vicoli_ciechi.
 *
*/
void F_elimina_muri(Str P,int r, int c,int nMuri,int nRand)
{
    /*
     * Se esiste un adiacente si provvede a renderlo corridoio
     * e successivamente si collegano le celle.
     *
     */
    switch (nRand)
    {
        case 0: // DX
            if(nMuri!=0 && !P->matrice[r][c].pDX)
            {
                P->matrice[r][c+1].murcorr=1;
                F_sistema_archi_modifica_labirinto(P,r,c+1);
                nMuri--;
            }
            break;

        case 1: // DW
            if(nMuri!=0 && !P->matrice[r][c].pDW)
            {
                P->matrice[r+1][c].murcorr=1;
                F_sistema_archi_modifica_labirinto(P,r+1,c);
                nMuri--;
            }
            break;

        case 2: // SX
            if(nMuri!=0 && !P->matrice[r][c].pSX)
            {
                P->matrice[r-1][c].murcorr=1;
                F_sistema_archi_modifica_labirinto(P,r-1,c);
                nMuri--;
            }
            break;

        case 3: // UP
            if(nMuri!=0 && !P->matrice[r][c].pUP)
            {
                P->matrice[r][c-1].murcorr=1;
                F_sistema_archi_modifica_labirinto(P,r,c-1);
                nMuri--;
            }
            break;
        default:
            F_error(12);
            break;
    }

   /*
    * Caso in cui al valore random generato non corrisponde
    * ad un adiacente, viene selezionato quello successivo.
    *
    */
    if(nMuri!=0 && nRand>=3) F_elimina_muri(P,r,c,nMuri,0);
    else if(nMuri!=0 && nRand<3) F_elimina_muri(P,r,c,nMuri,nRand+1);
}


/*
 *  Descrizione:    stampa a video il labirinto.
 *  Dettagli:       oltre alla stampa del laibirnto provvede a:
 *                  generare l'area di contorno attorno al labirinto,
 *                  stampare il menu' per la gestione del labirinto e
 *                  attende l'input dall'utente.
 *
 *  Parametri in:   P->struttura di gestione principale.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Utility - F_stampa_menu_labirinto.
 *
*/
void F_genera_area_labirinto(Str P)
{
    erase();
    refresh();

    M **matr=P->matrice;
    int i=0,j=0;

    for(i=0;i<P->rsize;i++)
        for(j=0;j<P->csize;j++)
        {
            move (i + OFFSET_X, j + OFFSET_Y);
            if(matr[i][j].murcorr==0)
                addch (' ' | A_REVERSE);
            else
                addch (' ');
        }

    F_genera_area_contorno(P);
    F_genera_menu_labirinto(P);
    F_gestione_labirinto(P);
}


/*
 *  Descrizione:    disegna attorno al labirinto.
 *  Dettagli:       rende in maniera più visiva i margini
 *                  del labirinto.
 *
 *  Parametri in:   P->struttura di gestione principale.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Maze - F_genera_area_labirinto.
 *
*/
void F_genera_area_contorno(Str P)
{
    int i=0;
    for(i=1;i<=P->csize+OFFSET_X-1;i++){ // UP
        move(1, i);
        if(i==1) addch(ACS_DIAMOND);
        else addch(ACS_S9);
    }
    addch(ACS_DIAMOND);

    for(i=1;i<=P->csize+OFFSET_X-1;i++){ // DW
        move(P->rsize+OFFSET_X, i);
        if(i==1) addch(ACS_DIAMOND);
        else addch(ACS_S3);
    }
    addch(ACS_DIAMOND);

    for(i=2;i<=P->rsize+OFFSET_X-1;i++){ // SX
        move(i, 1);
        addch(ACS_VLINE);
    }

    for(i=2;i<=P->rsize+OFFSET_X-1;i++){ // DX
        move(i,P->csize+OFFSET_X);
        addch(ACS_VLINE);
    }

}


/*
 *  Descrizione:    modifica del labirinto.
 *  Dettagli:       prende la cella selezionata dell'utente
 *                  per modificare il suo stato, muro->corridio
 *                  e viceversa.
 *
 *  Parametri in:   P->struttura di gestione principale.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Utility - F_gestione_labirinto.
 *
*/
void F_modifica_labirinto(Str P)
{
    int c=0; MEVENT event;
    F_stampa_pulisci_riga_labirinto("Seleziona col mouse (click sinistro) la posizione da modificare.");

    /* Viene prelevato solo il click del pulsante sx del mouse */
    mousemask(BUTTON1_CLICKED, NULL);

    c=getch();

    if(c==KEY_MOUSE)
    {
        if(getmouse(&event)==OK)
        {
            if(event.bstate & BUTTON1_CLICKED)
            {
                int riga=event.y,colonna=event.x;

                /* Verifica che sia stata selezionata una cella corretta */
                if((riga>=OFFSET_X && riga<P->rsize+OFFSET_X) && (colonna>=OFFSET_Y && colonna<P->csize+OFFSET_Y))
                {
                    M **matr=P->matrice;

                    /* Se la cella e' un corridio diventa muro; else, viceversa */
                    if(matr[riga-OFFSET_Y][colonna-OFFSET_X].murcorr) matr[riga-OFFSET_Y][colonna-OFFSET_X].murcorr=0;
                    else matr[riga-OFFSET_Y][colonna-OFFSET_X].murcorr=1;

                    /* Mostra a video la modifica */
                    move (riga,colonna);
                    if(matr[riga-OFFSET_Y][colonna-OFFSET_X].murcorr==0)
                        addch (' ' | A_REVERSE);
                    else
                        addch (' ');
                    refresh();

                    /* A seconda della modifica collega o scollega gli archi con le celle adiacenti */
                    F_sistema_archi_modifica_labirinto(P,riga-OFFSET_Y,colonna-OFFSET_X);

                }else F_stampa_pulisci_riga_labirinto("Cella selezionata non valida, annullo. Seleziona un'opzione dal menu'.");

                refresh();

            }else F_stampa_pulisci_riga_labirinto("Usa la pressione del tasto sinistro! Comando annullato.");

        } else F_stampa_pulisci_riga_labirinto("Selezione non valida, annullo. Seleziona un'opzione dal menu'.");
    }
    else F_stampa_pulisci_riga_labirinto("Modifica del labirinto annullata. Seleziona un'opzione dal menu'.");
}


/*
 *  Descrizione:    modifica lo stato degli archi di una cella.
 *  Dettagli:       in base allo stato della cella elimina gli archi uscenti
 *                  oppure li crea e collega con gli adiacenti.
 *
 *  Parametri in:   P->struttura di gestione principale,
 *                  r,c->riga e colonna della cella di riferimento.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Maze - F_modifica_labirinto.
 *
*/
void F_sistema_archi_modifica_labirinto(Str P,int r,int c)
{
    M **matr=P->matrice,*cDX=NULL,*cSX=NULL,*cUP=NULL,*cDW=NULL;

    /* Prelevo gli adiacenti */
    if(c<P->csize-1) cDX=F_coord_nodo_dx(P,r,c); // DX
    if(c>0) cSX=F_coord_nodo_sx(P,r,c);    // SX
    if(r>0) cUP=F_coord_nodo_up(P,r,c); // UP
    if(r<P->rsize-1) cDW=F_coord_nodo_dw(P,r,c); // DW

    /* Significa che la cella e' diventato un corridio */
    if(matr[r][c].murcorr)
    {
        F_stampa_pulisci_riga_labirinto("La cella scelta e' diventata un corridio! Seleziona un'opzione dal menu'.");

        /*
         * Si provvede a collegare gli eventuali
         * adiacenti.
         *
         */
        if(cDX!=NULL)
        {
            Peso pes=F_alloca_struttura_peso();
            cDX->pSX=pes;
            matr[r][c].pDX=pes;
        }

        if(cSX!=NULL)
        {
            Peso pes=F_alloca_struttura_peso();
            cSX->pDX=pes;
            matr[r][c].pSX=pes;
        }


        if(cUP!=NULL)
        {
            Peso pes=F_alloca_struttura_peso();
            cUP->pDW=pes;
            matr[r][c].pUP=pes;
        }

        if(cDW!=NULL)
        {
            Peso pes=F_alloca_struttura_peso();
            cDW->pUP=pes;
            matr[r][c].pDW=pes;
        }

    } else{ /* La cella e' diventata un muro */

        F_stampa_pulisci_riga_labirinto("La cella scelta e' diventata un muro! Seleziona un'opzione dal menu'.");

        /*
         * Si provvede all'eliminazione
         * degli archi.
         *
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
 *  Descrizione:  ritornano la cella adiacente a quella presa
 *                in ingresso.
 *  Dettagli:     //
 *
 *  Parametri in:  P->struttura di gestione principale,
 *                 r,c->riga e colonna della cella a cui
 *                 si vuole ottenere l'adiacente scelto.
 *
 *  Parametri out: indirizzo della cella adiacente.
 *
*/
M *F_coord_nodo_dx(Str P,int r,int c)
{
    M **matr=P->matrice;
    return &matr[r][c+1];
}

M *F_coord_nodo_sx(Str P,int r,int c)
{
    M **matr=P->matrice;
    return &matr[r][c-1];
}

M *F_coord_nodo_up(Str P,int r,int c)
{
    M **matr=P->matrice;
    return &matr[r-1][c];
}

M *F_coord_nodo_dw(Str P,int r,int c)
{
    M **matr=P->matrice;
    return &matr[r+1][c];
}


/*
 *  Descrizione:    ricerca del percorso scelto.
 *  Dettagli:       preleva le celle di partenza e arrivo scelte
 *                  dall'utente e esegue la ricerca del percorso minimo
 *                  tramite uno dei due algoritmi.
 *
 *  Parametri in:   P->struttura di gestione principale.
 *  Parametri out:  //
 *
 *  Chiamante:      L_Utility - F_gestione_labirinto.
 *
*/
void F_ricerca_percorso(Str P)
{
    /*
     * Si procede a prelevare le incormazioni
     * di riga e colonna della cella di partenza selezionata
     * dall'utente.
     */
    M **matr=P->matrice; M *p=NULL, *a=NULL;
    F_stampa_pulisci_riga_labirinto("Seleziona col mouse (click sinistro) la posizione da cui partire.");
    InfNodo infp=F_preleva_cella(P);
    if(infp) p=&matr[infp->r][infp->c];

    /* Si verifica che la cella sia corretta e che non sia un muro */
    if(p && p->murcorr!=0)
    {
        /* Viene mostrato a video il simbolo 'P' per indicare la cella di partenza scelta */
        F_mostra_elimina_pa(infp->r+OFFSET_X,infp->c+OFFSET_Y,1);

        /* Si preleva la cella di arrivo */
        F_stampa_pulisci_riga_labirinto("Seleziona col mouse (click sinistro) la posizione a cui arrivare.");
        InfNodo infa=F_preleva_cella(P);
        if(infa) a=&matr[infa->r][infa->c];

        /* Si controlla la correttezza della cella di arrivo */
        if(a && a->murcorr!=0)
        {
            /* Si mostra il simbolo 'A' indicante la cella di arrivo scelta */
            F_mostra_elimina_pa(infa->r+OFFSET_X,infa->c+OFFSET_Y,2);
            F_stampa_pulisci_riga_labirinto("Premi: (D): Dijkstra, (A): A*, (INVIO): Annulla");

            /* Attesa della scelta dell'utente sull'algoritmo da usare */
            int c=getch(),checkp=0;
            F_stampa_pulisci_riga_labirinto(" ");

            switch (c)
            {
                /*
                 * Entrambi gli algoritmi chiamano la funzione
                 * di inizializzazione e poi eseguono la ricerca del
                 * percorso. Successivamente  con F_stampa_percorso si verifica
                 * dell'effettiva esistenza del percorso minimo e con F_stampa_percorso
                 * si procede alla visaualizzazione a video.
                 *
                 * In caso dell'esistenza del percorso minimo (controllata da F_check_percorso),
                 * viene richiesto all'utente di animare un personaggio lungo tale percorso.
                 *
                 * Terminato si procede alla deallocazione di tutte le strutture usate.
                 *
                 */

                /* Dijkstra */
                case 100: // d
                case 68:  // D
                    F_init_dijkstra(P,infp->r,infp->c);
                    F_stampa_percorso(matr,P->StrHeap->p,infp->r,infp->c,infa->r,infa->c);
                    F_mostra_percorso(P,infp->r,infp->c,infa->r,infa->c);

                    checkp=F_check_percorso(P);
                    if(checkp) F_richiesta_animazione(P,infp->r,infp->c,infa->r,infa->c);

                    free(infp); free(infa);
                    F_dealloca_p(P->StrHeap,P);
                    StructHeap A=P->StrHeap; free(A); P->StrHeap=NULL;
                    P->nsize=1;
                    break;
                /* A* */
                case 97: // a
                case 65: // A
                    F_init_Astar(P,infp->r,infp->c,infa->r,infa->c);
                    F_stampa_percorso(matr,P->StrHeap->p,infp->r,infp->c,infa->r,infa->c);
                    F_mostra_percorso(P,infp->r,infp->c,infa->r,infa->c);

                    checkp=F_check_percorso(P);
                    if(checkp) F_richiesta_animazione(P,infp->r,infp->c,infa->r,infa->c);

                    free(infp); free(infa);
                    F_dealloca_p(P->StrHeap,P);
                    if(P->StrHeap->struttura) F_dealloca_albero_heap(P->StrHeap->struttura);
                    StructHeap B=P->StrHeap; free(B); P->StrHeap=NULL;
                    P->nsize=2;
                    break;
                default:
                    F_stampa_pulisci_riga_labirinto("Azione annullata. Seleziona un'opzione dal menu'.");
                    F_mostra_elimina_pa(infp->r+OFFSET_X,infp->c+OFFSET_Y,0);
                    F_mostra_elimina_pa(infa->r+OFFSET_X,infa->c+OFFSET_Y,0);
                    break;
            }

        } else {F_stampa_pulisci_riga_labirinto("Cella selezionata non valida, annullo. Seleziona un'opzione dal menu'."); F_mostra_elimina_pa(infp->r+OFFSET_X,infp->c+OFFSET_Y,0); }

    } else F_stampa_pulisci_riga_labirinto("Cella selezionata non valida, annullo. Seleziona un'opzione dal menu'.");

}

