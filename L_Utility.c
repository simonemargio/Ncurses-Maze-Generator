#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include "L_Utility.h"
#include "L_Prim.h"
#include "L_Dfs.h"
#include "L_Recursive.h"
#include "L_Divide.h"
#include "L_Maze.h"
#include "L_Astar.h"
#include "L_Error.h"
#include "L_Heap.h"
#include "L_Dijkstra.h"
#include "L_Struttura.h"



/*
 *  Descrizione: scelte dei menu'.
 *  Dettagli:    vettore contenente le scelte che possono
 *               essere effettuate dall'utente.
 *
*/
char *F_menu[]={
        "Genera labirinto",
        "Esci",

        "Prim",
        "DFS",
        "Recursive",
        "Divide",
        "INDIETRO",

        "Modifica labirinto",
        "Cerca percorso minimo",
        "Visualizza celle esplorate",
        "INDIETRO",
};

/*
 *  Descrizione: procedura iniziale.
 *  Dettagli:    procede all'avvio della curses e del controllo del terminale.
 *               Finiti i controlli si procede nel creare la finestra e le strutture
 *               di supporto.
 *
 *  Parametri in:  //
 *  Parametri out: //
 *
 *  Chiamante: main.c - main.
 *
*/
void F_start()
{

    /* Avvio della curses */
    F_avvia_curses();

    /* Verifica delle dimensioni del terminale */
    F_controllo_terminale();

    /* Inizializzazione seme numeri casuali */
    srand((unsigned)time(NULL));

    /* Allocazione della struttura di gestione principale */
    Str P=F_alloca_struttura_principale(); // L_Struttura

    /* Creazione della finestra centrale dei menu' */
    F_crea_finestra_menu(P);

    /* Esegue la stampa del menu' e interagisce con l'utente */
    F_stampa_menu_principale(P);

    /* Deallocazione di tutte le strutture presenti */
    F_dealloca_strutture(P); // L_Struttura

    /* Terminazione della curses */
    F_termina_curses();
}


/*
 *  Descrizione: avvio della modalita' curses.
 *  Dettagli:    esegue le azioni predefinite per una
 *               corretta visualizzazione e interazione
 *               con il programma.
 *
 *  Parametri in:  //
 *  Parametri out: //
 *
 *  Chiamante:  L_Utility - F_start.
 *
*/
void F_avvia_curses()
{
    /* Starta la modalita' curses */
    initscr ();

    /* Elimina l'"echo" di ciò che prende in ingresso */
    noecho ();

    /* Disabilita il line buffering  */
    cbreak ();

    /* Abilita la tastiera (usata per l'interazione con le frecce direzionali) */
    keypad (stdscr, TRUE);

    /* Cursore invisibile */
    curs_set (0);
    refresh ();
}


/*
 *  Descrizione:  terminazione della modalita' curses.
 *  Dettagli:     //
 *
 *  Parametri in:  //
 *  Parametri out: //
 *
 *  Chiamante: L_Utility - F_start, F_controllo_terminale, F_stampa_menu_principale.
 *             L_Error - F_error.
 *
*/
void F_termina_curses()
{
    clrtoeol ();
    refresh ();
    endwin ();
}


/*
 *  Descrizione: controlla la dimensione del terminale.
 *  Dettagli:    il programma accetta come valori minimi un terminale
 *               di dimensioni 80x24. Valori piu' piccoli vengono rifiutati.
 *
 *  Parametri in:  //
 *  Parametri out: //
 *
 *  Chiamante: L_Utility - F_start.
 *
*/
void F_controllo_terminale()
{
    int r=0,c=0;
    /* Preleva le dimensioni correnti del terminale */
    getmaxyx(stdscr,r,c);

    if(r<24 || c<80)
    {
        mvprintw(0,0,"Dimensione del terminale non valida.\n"
                "\nRichiesta minima: 80x24. Il tuo terminale invece ha dimensioni: %dx%d.\nAdatta il tuo terminale e riesegui il programma."
                "\n\nPremi un tasto per continuare.",c,r);
        getch();
        F_termina_curses();
        exit(0);
    }
}


/*
 *  Descrizione: stampa del menu' iniziale.
 *  Dettagli:    viene visualizzato il menu' iniziale dove si
 *               puo' scegliere se generare un labirinto o uscire.
 *
 *  Parametri in:  P->struttura principale
 *  Parametri out: //
 *
 *  Chiamante: L_Utiliy - F_start.
 *
*/
void F_stampa_menu_principale(Str P)
{
    /*
     * Minimo:       valore minimo di selezione
     * Massimo:      valore massimo di selezione
     * Partenza      valore di partenza dell'effetto illuminazione
     * Selezionato:  valore selezionato iniziale
     * Scelta:       elemento scelto dall'utente
    */
    int minimo=1,massimo=2,partenza=0,selezionato=1,scelta=0;

    /* Stampa delle informazioni iniziali */
    mvprintw(0, 1, "Laboratorio di algoritmi e strutture dati\n\n Studente: Simone Margio N86001098\n "
            "Progetto: generazione e gestione casuale di labirinti");
    mvprintw(23,1,"Usa le frecce direzionali per spostarti e premi INVIO per selezionare!");

    /* Pulizia della schermata di menu' */
    werase(P->wMenu);

    /* Stampa del menu' di selezione iniziale (Genera labirinto - Esci) */
    F_stampa_menu(P->wMenu,selezionato,partenza,massimo,2,2);

    /* Richiesta all'utente di selezione */
    scelta=F_scelta_menu(P->wMenu,selezionato,partenza,minimo,massimo,2,2);

    switch (scelta)
    {
        case 1: // Generazione labirinto
            F_stampa_menu_labirinto(P);
            break;
        case 2: // Esci
            F_dealloca_strutture(P); // L_Struttura
            F_termina_curses();
            exit(0);
        default:
            F_error(4);
            break;
    }
}


/*
 *  Descrizione: stampa del menu' di selezione del tipo di labirinto.
 *  Dettagli:    effettua la stampa del secondo menu' in cui puo' interagire
 *               l'utente. Puo' scegliere tra i quattro tipi di generatori di
 *               labirinti: Prim, DFS, Recursive e Divide.
 *
 *  Parametri in:  P->struttura di gestione principale.
 *  Parametri out: //
 *
 *  Chiamante: L_Utility - F_stampa_menu_principale.
 *
*/
void F_stampa_menu_labirinto(Str P)
{
    /* Descrizione variabili in: F_stampa_menu_principale */
    int minimo=3,massimo=7,partenza=2,selezionato=3,scelta=0;

    /* Pulizia di tutto lo schermo */
    erase();
    mvprintw(0, 1, "Seleziona l'algoritmo di generazione del labirinto!");
    werase(P->wMenu);

    /* Stampa a video la tipologia di generatori */
    F_stampa_descrizione_lab();

    /* Stampa a vidio il secondo menu' e attende l'interazione dell'utente */
    F_stampa_menu(P->wMenu,selezionato,partenza,massimo,2,2);
    scelta=F_scelta_menu(P->wMenu,selezionato,partenza,minimo,massimo,2,2);

    /* La prima volta viene allocata la matrice, le successive solo inizializzata */
    if(P->matrice==NULL) F_alloca_struttura_matrice(P);
    else { F_dealloca_archi_matrice(P); F_inizializza_matrice(P);}

    erase();

    switch (scelta)
    {
        /*
         * Tipi di generatori:
         * F_prim_maze: L_Prim
         * F_dfs_maze:  L_Dfs
         * F_init_recursive_maze: L_Recursive
         * F_init_divide: L_Divide
         * Recursive e divide dispongono di
         * un'inizializzazione iniziale.
         *
         * F_genera_area_labirinto (L_Maze) provvede alla
         * stampa del labirinto a video e alle successive
         * interazioni con l'utente.
         *
         * F_dealloca_info_cell elimina le informazioni usate
         * durante la generazione del labirinto.
         * Maggiori dettagli nella documentazione pag.10, 2.2.2.
         */

        case 3: // PRIM
            F_prim_maze(P);
            F_genera_area_labirinto(P);
            F_dealloca_info_cell(P);
            break;
        case 4: // DFS
            F_dfs_maze(P);
            F_genera_area_labirinto(P);
            F_dealloca_info_cell(P);
            break;
        case 5: // RECURSIVE
            F_init_recursive_maze(P);
            F_elimina_vicoli_ciechi(P);
            F_genera_area_labirinto(P);
            break;
        case 6: // DIVIDE
            F_init_divide(P);
            F_genera_area_labirinto(P);
            break;
        case 7:
            F_stampa_menu_principale(P);
            break;
        default:
            F_error(5);
            break;
    }

    erase();
    F_stampa_menu_principale(P);
}


/*
 *  Descrizione: stampa delle scelte che possono essere effettuate.
 *  Dettagli:    stampa tutto il menu' e illumina la scelta selezionata
 *               dall'utente.
 *
 *  Parametri in:  win->finestra di riferimento, selezionato->valore iniziale
 *                 da illuminare, partenza->valore iniziale da considerare,
 *                 massimo->massimo valore della scelta, x,y->coordinate di stampa
 *                 delle scelte del menu'.
 *  Parametri out: //
 *
 *  Chiamante: L_Utility - F_stampa_menu_labirinto, F_stampa_menu_principale,
 *             F_scelta_menu, F_stampa_menu_scelta_labirinto
 *
*/
void F_stampa_menu(WINDOW *win,int selezionato,int partenza,int massimo,int x,int y)
{
    /* Abilita la tastiera nella finestra di riferimento */
    keypad(win,TRUE);
    refresh();

    int i;
    /* Disegna la finestra con caratteri di default */
    box(win,0,0);

    for(i=partenza;i<massimo;++i)
    {
        /* Elemento selezionato */
        if(selezionato==i+1)
        {
            /* Effetto illuminazione */
            wattron(win,A_REVERSE);
            mvwprintw(win,y,x,"%s",F_menu[i]);
            wattroff(win,A_REVERSE);
        }
        else
        {
            mvwprintw(win,y,x,"%s",F_menu[i]);
        }
        ++y;

    }
    /* Refresh della finestra */
    wrefresh(win);
}


/*
 *  Descrizione:  stampa a video il secondo menu'.
 *  Dettagli:     stampa il menu' per la seleziona del tipo
 *                di generatore da usare per creare il labirinto
 *                e attende la scelta dall'utente.
 *
 *  Parametri in:  P->struttura principale.
 *  Parametri out: scelta->valore scelto dall'utente.
 *
 *  Chiamante: L_Utility - F_gestione_labirinto.
 *
*/
int F_stampa_menu_scelta_labirinto(Str P)
{
    /* Descrizione variabili in: F_stampa_menu_principale */
    int minimo=8,massimo=11,partenza=7,selezionato=8,scelta=0;

    F_stampa_menu(P->wOplab,selezionato,partenza,massimo,2,9);
    scelta=F_scelta_menu(P->wOplab,selezionato,partenza,minimo,massimo,2,9);
    wrefresh(P->wOplab);

    return scelta;
}


/*
 *  Descrizione:   pulisce e stampa una riga a video.
 *  Dettagli:      permette la stampa di tutte le possibili
 *                 informazioni dinamiche da dare all'utente
 *                 in base alle sue scelte.
 *
 *  Parametri in:  s->stringa da stampare.
 *  Parametri out: //
 *
 *  Chiamante: tutte le funzioni/procedure che richiedono un puts a video.
 *
*/
void F_stampa_pulisci_riga_labirinto(char *s)
{
    move(23,2);
    clrtoeol();
    mvprintw(23,2,"%s",s);
}


/*
 *  Descrizione: ricerca dell'esistenza di un percorso.
 *  Dettagli:
 *
 *  Parametri in: matr->matrice del labirinto, pred->matrice dei predecessori,
 *                rP->riga cella partenza, cP->colonna cella partenza,
 *                rA->riga cella arrivo, cA->colonna cella arrivo.
 *  Parametri out: //
 *
 *  Chiamante: L_Maze.c - F_ricerca_percorso.
 *
*/
void F_stampa_percorso(M **matr, Pp **pred,int rP,int cP,int rA,int cA)
{
    /* Caso base e verifica dell'uguaglianza delle celle passate */
    if(rP==rA && cP==cA)
    {
        /* Si setta la cella come visitata */
        matr[rP][cP].visitato=1;
    }
    else /* Si verifica l'esistenza di un percorso */
    {
        /* Non e' presente un percorso */
        if(pred[rA][cA].pred==NULL)
        {
            F_stampa_pulisci_riga_labirinto("Non e' presente un percorso! Premi un tasto qualsiasi per continuare...");
            getch();

            /* Eliminazione dei caratteri 'P' 'A' nel labirinto */
            F_mostra_elimina_pa(rP+OFFSET_X,cP+OFFSET_Y,0);
            F_mostra_elimina_pa(rA+OFFSET_X,cA+OFFSET_Y,0);
            F_stampa_pulisci_riga_labirinto("Seleziona un'opzione dal menu'.");
        }
        else /* C'è la possibilita' di un percorso */
        {
            /* Prelevo le informazioni di riga e colonna */
            Albero p=pred[rA][cA].pred;
            int r=p->r;
            int c=p->c;
            F_stampa_percorso(matr,pred,rP,cP,r,c);
            matr[rA][cA].visitato=1;
        }
    }
}


/*
 *  Descrizione: creazione della finestra di menu'.
 *  Dettagli:    effettua l'allocazione della finestra di selezione
 *               dei primi due menu' che vengono proposti all'utente.
 *
 *  Parametri in:  P->struttura principale
 *  Parametri out: //
 *
 *  Chiamante: L_Utility - F_start.
 *
*/
void F_crea_finestra_menu(Str P)
{
    int rig = 10;
    int col = 25;
    /*
     * Si possono abilitare se si preferisce che
     * i primi due menu' vengano sempre centrati
     * nello schermo.
     *
     */
    //int starty = ((LINES - rig) / 2);
    //int startx = ((COLS - col) / 2);
    int starty = rig-4;
    int startx = col;
    P->wMenu=F_crea_finesta(rig,col,starty,startx); // L_Struttura
}


/*
 *  Descrizione: stampa a video delle informazioni.
 *  Dettagli:    effettua la stampa dei tipi di generatori
 *               che possono essere utilizzati.
 *
 *  Parametri in:  //
 *  Parametri out: //
 *
 *  Chiamante: L_Utility - F_stampa_menu_labirinto.
 *
*/
void F_stampa_descrizione_lab()
{
    mvprintw(18,1,"Tipologia di generatore:");
    mvprintw(19,1,"PRIM: generatore ispirato a Prim (iterativo)");
    mvprintw(20,1,"DFS: generatore ispirato a Dfs (iterativo)");
    mvprintw(21,1,"RECURSIVE: generatore ispirato a Dfs (ricorsivo)");
    mvprintw(22,1,"DIVIDE: generatore a suddivisione (ricorsivo)");
}


/*
 *  Descrizione: genera un intero random.
 *  Dettagli:    //
 *
 *  Parametri in:  nsize->valore massimo-1 che puo'
 *                 essere generato.
 *
 *  Parametri out: valore generato.
 *
 *  Chiamante: L_Maze - F_cella_random, F_adj_random, F_elimina_vicoli_ciechi.
 *
*/
int F_genera_int_random(int nsize)
{
    return 0 + rand() % (nsize - 0);
}


/*
 *  Descrizione: genera un peso per un arco.
 *  Dettagli:    //
 *
 *  Parametri in:  dimensione massima.
 *  Parametri out: peso generato.
 *
 *  Chiamante: L_Struttura - F_alloca_struttura_peso.
 *
*/
float F_genera_peso(int dim)
{
    float f=(((float)rand()/(float)(RAND_MAX)) * dim);
    return f;
}


/*
 *  Descrizione: gestisce la selezione nel menu'.
 *  Dettagli:    in base alla finestra passata permette all'utente
 *               di usare i tasti direzionali per poter selezionare
 *               l'elemento desiderato.
 *
 *  Parametri in:  dettagli gia' specificati in: F_stampa_menu.
 *  Parametri out: scelta->valore selezionato dall'utente.
 *
 *  Chiamante: L_Utility - F_stampa_menu_principale, F_stampa_menu_labirinto,
 *                         F_stampa_menu_scelta_labirinto.
 *
*/
int F_scelta_menu(WINDOW *win,int selezionato,int partenza,int minimo,int massimo,int x,int y)
{
    int scelta=0;

    while(1)
    {
        int c=wgetch(win);

        switch(c)
        {
            case KEY_UP:
                /* Se si trova al primo elemento, ritorna all'ultimo */
                if(selezionato==minimo)
                    selezionato=massimo;
                else
                    --selezionato;
                break;

            case KEY_DOWN:
                /* Se si trova all'ultimo elemento, ritorna al primo */
                if(selezionato==massimo)
                    selezionato=minimo;
                else
                    ++selezionato;
                break;
            case 10:
                /* Tasto invio premuto, scelta effettuata */
                scelta=selezionato;
                break;
        }
        F_stampa_menu(win,selezionato,partenza,massimo,x,y);
        if(scelta!=0) break;
    }
    refresh();

    return scelta;
}


/*
 *  Descrizione:   procedura principale per la gestione di un labirinto.
 *  Dettagli:      permette la gestione di tutte le funzionalita'
 *                 che possono essere utilizzate su un labirinto
 *                 di riferimento.
 *
 *  Parametri in:  P->struttura di gestione principale.
 *  Parametri out: //
 *
 *  Chiamante: L_Maze - F_genera_area_labirinto.
 *
*/
void F_gestione_labirinto(Str P)
{
    int uscita=1,scelta=0;
    P->nsize=0;

    while (uscita)
    {
        /* Stampa delle funzionalita' che possono essere effettuate */
        scelta=F_stampa_menu_scelta_labirinto(P); // L_Utility

        switch (scelta)
        {

            /*
             * F_pulisci_visite (L_Utility) ripristina
             * lo stato di cella visitata a 0.
             * Maggiori informazioni sullo stato di visita
             * di una cella nella documentazione a pag.10, 2.2.2.
             */
            case 8: // Modifica labirinto
                F_pulisci_visite(P);
                F_modifica_labirinto(P); // L_Maze
                P->nsize=0;
                break;
            case 9: // Ricerca del percorso
                F_pulisci_visite(P);
                F_ricerca_percorso(P); // L_Maze
                break;
            case 10: // Visualizzazione celle esplorate
                /*
                 * P->nsize puo' contenere tre valori:
                 *
                 * 0: non e' ancora stato utilizzato nessun algoritmo
                 *    per la ricerca del percorso. L'utente viene quindi
                 *    informato di dover prima cercare un percorso per poter
                 *    vedere le celle esplorate.
                 * 1: significa che l'algoritmo usato per la ricerca del percorso
                 *    e' Dijkstra. L'utente viene informato di dover usare A* per
                 *    visualizzare la celle esplorate.
                 *    Maggiori dettagli nella documentazione pag.9, 2.2.1.
                 * 2: significa che l'algoritmo usato per la ricerca del percorso
                 *    e' A*. Si procede quidni alla visualizzazione delle celle
                 *    esplorate dall'algoritmo.
                 */
                if(P->nsize==2)
                {
                    F_celle_esplorate(P); // L_Utility
                    P->nsize=0;
                    F_stampa_pulisci_riga_labirinto("Seleziona un'opzione dal menu'.");
                    break;
                } else
                if(P->nsize==0) F_stampa_pulisci_riga_labirinto("Devi eseguire prima 'Cerca percorso minimo'. Premi invio per continuare.");
                else F_stampa_pulisci_riga_labirinto(" La visualizzazione vale solo per A*! Premi invio per continuare.");
                getch();
                F_stampa_pulisci_riga_labirinto("Seleziona un'opzione dal menu'.");
                break;
            case 11: // Indietro
                uscita=0;
                break;
            default:
                F_stampa_pulisci_riga_labirinto("Scelta non valida! Premi invio per continuare.");
                refresh();
                getch();
                // move(23,2);
                // clrtoeol();
                break;
        }

    }

    /* Ritorno al secondo menu' */
    erase();
    F_stampa_menu_labirinto(P);
}


/*
 *  Descrizione:    menu' di interazione per l'uso del labirinto.
 *  Dettagli:       stampa nella finestra specifica la descrizione
 *                  su come usare le funzionalita' a disposizione per
 *                  l'interazione con il labirinto.
 *
 *  Parametri in:   P->struttura di gestione principale.
 *  Parametri out:  //
 *
 *  Chiamante: L_Maze - F_genera_area_labirinto.
 *
*/
void F_genera_menu_labirinto(Str P)
{
    /* Alloca e disegna la finestra di riferimento */
    P->wOplab=F_crea_finesta(P->rsize+1,P->csize-4,2,P->csize+4); // L_Struttura
    box(P->wOplab, 0, 0);

    mvwprintw(P->wOplab,1,2,"%s","MENU' DI GESTIONE LABIRINTO");
    mvwprintw(P->wOplab,3,2,"%s","Seleziona l'opzione che desideri");
    mvwprintw(P->wOplab,4,2,"%s","usando le frecce direzionali e");
    mvwprintw(P->wOplab,5,2,"%s","premendo il tasto invio.");
    F_stampa_pulisci_riga_labirinto("Seleziona un'opzione dal menu'.");
    wrefresh(P->wOplab);
}


/*
 *  Descrizione: visualizza le celle esplorate.
 *  Dettagli:    mostra a video le celle esplorate effettivamente dall'algoritmo A*.
 *               Viene effettuato un controllo per verificare che il terminale
 *               supporti l'uso dei colori.
 *               Caso negativo si effettua la stampa con i colori predefiniti.
 *               Caso positivo si verifica se il terminale puo' riprisitnare i colori
 *               di default dopo un cambio di colore.
 *
 *  Parametri in:  P->struttura di gestione principale.
 *  Parametri out: //
 *
 *  Chiamante:     L_Utility - F_gestione_labirinto.
 *
*/
void F_celle_esplorate(Str P)
{
    /* Verifica del supporto colore */
    int check=F_check_color();

    if(check) // Il terminale supporta l'uso dei colori
    {
        /*
         * Avvio dell'uso dei colori.
         * Uso del colore di sfondo rosso con foreground bianco.
         * Verifica del ripristino dei colori predefiniti.
         * Maggiori infomazioni sulla gestione dei colori nella
         * documentazione a pag.28, 5.1.4.
         */
        start_color();
        init_pair(1,COLOR_WHITE,COLOR_RED);
        int x=use_default_colors();
        if(x!=0)
        {
            F_stampa_pulisci_riga_labirinto("Il terminale non supporta colori: ISO 6429. Premi invio per continuare.");
            getch();
            F_stampa_pulisci_riga_labirinto("Dopo potresti visualizzare colori divesi. Premi invio per continuare.");
            getch();
        }
        attron(COLOR_PAIR(1));
        F_visualizza_celle_esplorate(P); // L_Utility
        attroff(COLOR_PAIR(1));
    }
    else // Nessun supporto per i colori
    {
        F_visualizza_celle_esplorate(P); // L_Utility
    }
    F_stampa_pulisci_riga_labirinto("Visualizzazione eseguita. Premi invio per tornare al menu'. ");
    getch();
    F_pulisci_visite(P); // L_Utility
}


/*
 *  Descrizione: mostra le celle esplorate.
 *  Dettagli:    visualizza le celle che sono state
 *               esplorate dall'algoritmo di ricerca A*.
 *
 *  Parametri in:  P->struttura di gestione principale.
 *  Parametri out: //
 *
 *  Chiamante: L_Utility - F_celle_esplorate.
 *
*/
void F_visualizza_celle_esplorate(Str P)
{
    M **matr=P->matrice;
    int r=0,c=0;

    for(r=0;r<P->rsize;r++)
        for(c=0;c<P->csize;c++)
        {
            /*
             * Maggiori informazioni sullo stato
             * visitato di una cella nella documentazione
             * a pag.10, 2.2.2.
             */
            if(matr[r][c].visitato==2)
            {
                /* SPostamento del cursore nel punto indicato */
                move(r+OFFSET_X,c+OFFSET_Y);
                addch (ACS_BULLET);
                refresh();
            }
        }
}


/*
 *  Descrizione:  verifica del supporto colori del terminale.
 *  Dettagli:     //
 *
 *  Parametri in:  //
 *  Parametri out: 0->il terminale non supporta i colori, 1->altrimenti.
 *
 *  Chiamante: L_Utility - F_celle_esplorate.
 *
*/
int F_check_color()
{
    if(has_colors()==FALSE) return 0;
    else return 1;
}


/*
 *  Descrizione:   ripristina lo stato di visita.
 *  Dettagli:      riporta lo stato di visita di ogni cella
 *                 esplorata (2) o facente parte di un percorso
 *                 minimo (1) allo stato di non visitato.
 *
 *  Parametri in:  P->struttura di gestione principale
 *  Parametri out: //
 *
 *  Chiamante: L_Utility - F_celle_esplorate, F_gestione_labirinto.
 *
*/
void F_pulisci_visite(Str P)
{
    M **matr=P->matrice;
    int i=0,j=0;

    for(i=0;i<P->rsize;i++)
        for(j=0;j<P->csize;j++)
        {
            if(matr[i][j].visitato==1 || matr[i][j].visitato==2)
            {
                matr[i][j].visitato=0;
                move(i+OFFSET_X,j+OFFSET_Y);
                addch (' ');
                refresh();
            }

        }
}



/*
 *  Descrizione: inserisci o elimina (P)artenza e/o (A)rrivo.
 *  Dettagli:    Le celle di partenza e arrivo vengono visualizzate
 *               mediante il pcarattere P e A.
 *               Nel caso in cui si sceglie un cella sbagliata oppure
 *               si decice di non voler piu' effettuare l'operazione
 *               di visualizzazione del percorso minimo, P e/o A vengono
 *               eliminate visivamente dal labirinto.
 *
 *  Parametri in:   r,c-> riga e colonna della cella selezionata.
 *                  el-> carattere da disegnare.
 *  Parametri out:  //
 *
 *  Chiamante: L_Utility - F_stampa_percorso, L_Maze - F_ricerca_percorso.
 *
*/
void F_mostra_elimina_pa(int r,int c, int el)
{
    move(r,c);

    switch (el)
    {
        case 0:
            addch(' ');
            break;
        case 1:
            addch('P');
            break;
        case 2:
            addch('A');
            break;

        default:
            F_error(8);
            break;
    }

}


/*
 *  Descrizione: verifica l'esistenza di un percorso.
 *  Dettagli:    controlla se e' presente un percorso o una parte di esso.
 *               In caso affermativo si procede successivamente a richiedere
 *               all'utente se vuole effettuare un'animazione di un pergonaggio
 *               lungo il percorso.
 *               Caso opposto la richiesta di animazione non viene richiesta.
 *
 *  Parametri in:  P->struttura di gestione principale.
 *  Parametri out: found-> 1: e' presente un percorso, 0: altrimenti.
 *
 *  Chiamante: L_Maze - F_ricerca_percorso.
 *
*/
int F_check_percorso(Str P)
{
    M **matr=P->matrice;
    int i=0,j=0,found=0;

    for(i=0;i<P->rsize;i++)
        for(j=0;j<P->csize;j++) {
            if (matr[i][j].visitato == 1) {
                    found=1;
                    break;
            }
        }
    return found;
}


/*
 *  Descrizione: richiesta di animazione.
 *  Dettagli:    chiede all'utente se vuole animare un personaggio
 *               che segue il percorso trovato.
 *
 *  Parametri in: P->struttura principale, rP,cP-> riga e colonna della
 *                cella di partenza, rA,cA-> riga e colonna della cella
 *                di arrivo.
 *
 *  Parametri out: //
 *
 *  Chiamante: L_Maze - F_ricerca_percorso.
 *
*/
void F_richiesta_animazione(Str P, int rP, int cP, int rA, int cA)
{
    int c=-1;
    F_stampa_pulisci_riga_labirinto("Vuoi animare un personaggio che segue il percorso? (S): Si (INVIO): Annulla");

    c=getch();

    switch (c)
    {
        case 115: // s
        case 83:  // S
            F_stampa_pulisci_riga_labirinto("Animazione in corso...");
            F_animazione_percorso(P,rP,cP,rA,cA);
            break;
        default:
            //mvprintw(23,2,"%d",c);
            break;
    }

    F_stampa_pulisci_riga_labirinto("Seleziona un'opzione dal menu'.");
}



/*
 *  Descrizione: effettua l'animazione del personaggio.
 *  Dettagli:    dalla cella di partenza cerca l'adiacente con
 *               visitato pari a 1 e avanza verso di questo.
 *
 *  Parametri in:   P->struttura principale, r,c->riga e colonna cella
 *                  di partenza, rA,cA->riga e colonna cella di arrivo.
 *
 *  Parametri out: //
 *
 *  Chiamante: L_Utility - F_richiesta_animazione.
 *
*/
void F_animazione_percorso(Str P,int r, int c, int rA, int cA)
{
    M **matr=P->matrice;
    M *cellDx=NULL,*cellSx=NULL,*cellUp=NULL,*cellDW=NULL;
    int endp=1;
    matr[r][c].visitato=0;

    while (endp) {

        /* Spostamento nella cella selezionata */
        move(r+OFFSET_X,c+OFFSET_Y);

        /* Personaggio */
        addch('O');
        refresh();

        /* "Effetto" spostamento */
        usleep(DELAY);

        /* Preleva gli adiacenti */
        if(c<P->csize-1) cellDx=F_coord_nodo_dx(P,r,c); // DX
        if(c>0) cellSx=F_coord_nodo_sx(P,r,c);    // SX
        if(r>0) cellUp=F_coord_nodo_up(P,r,c); // UP
        if(r<P->rsize-1) cellDW=F_coord_nodo_dw(P,r,c); // DW

        /* "Crea" l'animazione */
        move(r+OFFSET_X,c+OFFSET_Y); addch(' ');

        /* Ricerca dell'adiacente con stato visitato pari a 1 */
        if(cellDx!=NULL && cellDx->visitato==1){  c=c+1; cellDx->visitato=0; }
        else
        {
            if(cellSx!=NULL && cellSx->visitato==1) {c=c-1; cellSx->visitato=0; }
            else
            {
                if(cellUp!=NULL && cellUp->visitato==1) {r=r-1; cellUp->visitato=0;}
                else
                {
                    if(cellDW!=NULL && cellDW->visitato==1) {r=r+1; cellDW->visitato=0;}
                    else endp=0; /* Si e' arrivati all'ultima cella che non ha adiacenti visitati */
                }
            }
        }
    }
    mvprintw(23,2," ");
}


/*
 *  Descrizione:   visualizza il percorso trovato.
 *  Dettagli:      dopo la ricerca del percorso minimo, se presente
 *                 viene visualizzato a video.
 *
 *  Parametri in:  informazioni presenti all'interno della procedura.
 *  Parametri out: //
 *
 *  Chiamante:     L_Maze - F_ricerca_percorso.
 *
*/
void F_mostra_percorso(Str P,int r,int c,int rA,int cA)
{
    /*
     * Parametri in ingresso:
     * r,c,rA,cA sono stati lasciati nonostante non utilizzati.
     * La scelta e' dovuta nel caso in cui si voglia implementare una
     * variante di ricerca del percorso simile a quella mostrata nella
     * procedura F_animazione_percorso, con stampa delle sole celle visitate
     * e ricerca dei suoi adiacenti senza coinvolgere tutte le celle della matrice.
     */

    int i=0,j=0;

    M **matr=P->matrice;

    for(i=0;i<P->rsize;i++)
        for(j=0;j<P->csize;j++)
        {
            if(matr[i][j].visitato==1)
            {
                move(i+OFFSET_X,j+OFFSET_Y);
                addch ('*');
                refresh();
            }
        }
}


/*
 *  Descrizione:   permette la selezione di una cella del labirinto.
 *  Dettagli:      chiede l'interazione dell'utente tramite l'uso del mouse
 *                 (click sinistro) per selezionare la cella che desidera.
 *
 *  Parametri in:  P->struttura principale.
 *  Parametri out: struttura contentente riga e colonna della cella selezionata.
 *
 *  Chiamante: L_Maze - F_ricerca_percorso.
 *
*/
InfNodo F_preleva_cella(Str P)
{
    int c=0; MEVENT event;

    /* Abilita l'uso del mouse solo per iil click sinistro */
    mousemask(BUTTON1_CLICKED, NULL);

    c=getch();

    if(c==KEY_MOUSE) {
        if (getmouse(&event) == OK) {
            if (event.bstate & BUTTON1_CLICKED)
            {
               /* Verifica che la cella selezionata sia corretta */
               int check=F_check_cella_in_labirinto(P,event);
               if(check) return NULL;
            }
        }else return NULL; 
    }else return NULL;

    /*
     * Si e' deciso di sfruttare la struttura usata per mantenere informazioni
     * delle celle durante l'uso di Prim e Dfs.
     * In questo caso si salvano le informazioni sulla cella selezionata.
     * Il terzo parametro e' del tutto indifferente.
     */
    InfNodo inf=F_alloca_info_nodo(event.y-OFFSET_X,event.x-OFFSET_Y,0);

    return inf;
}



/*
 *  Descrizione:    verifica che la cella scelta sia corretta.
 *  Dettagli:       viene controllato che il click effettuato dall'utente
 *                  appartenga ad una cella presente nella matrice.
 *
 *  Parametri in:   P->struttura principale, event->struttura curses contenente
 *                  riga e colonna selezionate.
 *  Parametri out:  1->cella non valida, 0->altrimenti.
 *
 *  Chiamante:
 *
*/
int F_check_cella_in_labirinto(Str P,MEVENT event)
{
    int r=event.y,c=event.x;

    /* La cella deve trovarsi all'interno della rappresentazione del labirinto */
    if((r<P->rsize+OFFSET_X && r>=OFFSET_X) && (c<P->csize+OFFSET_Y && c>=OFFSET_Y))
        return 0;
    else
        return 1;
}


/* Procedura di test. Puo' essere utilizzata per la stampa
 * della matrice senza l'uso della curses.
 * Di default non e' chiamata da nessuna funzione/procedura.
 */
void F_stampa_matrice(Str P)
{
    M **matr=P->matrice;
    int i,j;
    puts("\nLABIRINTO");
    for(i=0;i<P->rsize;i++)
    {
        puts("");
        for(j=0;j<P->csize;j++)
        {
            if(matr[i][j].murcorr==0) // Muro
                printf("|M|");
            else
            {
                /* Abilitare una delle due in base all'uso che ne serve */

                /* Stampa del percorso minimo e dei nodi visitati */
                //   if(matr[i][j].visitato==1) printf("|.|");
                //   else if(matr[i][j].visitato==2) printf("|0|");
                //   else printf("| |");

                /* Stampa degli archi presenti */
                //    printf("|");
                //    if(matr[i][j].pSX!=NULL) printf("sx.");
                //    if(matr[i][j].pDX!=NULL) printf("dx.");
                //    if(matr[i][j].pUP!=NULL) printf("up.");
                //    if(matr[i][j].pDW!=NULL) printf("dw.");
                //    printf("|");
            }
        }
    }
}