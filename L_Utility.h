#ifndef LASD_FINAL_2_L_UTILITY_H
#define LASD_FINAL_2_L_UTILITY_H

#include <ncurses.h>
#include "L_Dijkstra.h"
#include "L_Struttura.h"

/* Riga e colonna della matrice da generare */
#define RIGA 20     /* DEFAULT: 20 */
#define COLONNA 40  /* DEFAULT: 40 */

/* Riga e colonna da cui partitre per mostrare la matrice in curses */
#define OFFSET_X 2  /* DEFAULT: 2 */
#define OFFSET_Y 2  /* DEFAULT: 2 */

/* Tempo di animazione del personaggio */
#define DELAY 130000     /* DAFAULT: 130000 */

/* funzione iniziale del programma */
void F_start();

/* avvio della modalita' curses */
void F_avvia_curses();

/* terminazione della modalita' curses */
void F_termina_curses();

/* controlla la dimensione del terminale */
void F_controllo_terminale();

/* stampa del menu' iniziale */
void F_stampa_menu_principale(Str P);

/* stampa del menu' di selezione del tipo di labirinto */
void F_stampa_menu_labirinto(Str P);

/* stampa delle scelte che possono essere effettuate */
void F_stampa_menu(WINDOW *win,int selezionato,int partenza,int massimo,int x,int y);

/* stampa a video il secondo menu' */
int F_stampa_menu_scelta_labirinto(Str P);

/* pulisce e stampa una riga a video */
void F_stampa_pulisci_riga_labirinto(char *s);

/* ricerca dell'esistenza di un percorso */
void F_stampa_percorso(M **matr, Pp **pred,int rP,int cP,int rA,int cA);

/* creazione della finestra di menu' */
void F_crea_finestra_menu(Str P);

/* stampa a video delle informazioni */
void F_stampa_descrizione_lab();

/* genera un intero random */
int F_genera_int_random(int nsize);

/* genera un peso per un arco */
float F_genera_peso(int dim);

/* gestisce la selezione nel menu' */
int F_scelta_menu(WINDOW *win,int selezionato,int partenza,int minimo,int massimo,int x,int y);

/* procedura principale per la gestione di un labirinto */
void F_gestione_labirinto(Str P);

/* menu' di interazione per l'uso del labirinto */
void F_genera_menu_labirinto(Str P);

/* visualizza le celle esplorate */
void F_celle_esplorate(Str P);

/* mostra le celle esplorate */
void F_visualizza_celle_esplorate(Str P);

/* verifica del supporto colori del terminale */
int F_check_color();

/* ripristina lo stato di visita */
void F_pulisci_visite(Str P);

/* inserisci o elimina (P)artenza e/o (A)rrivo */
void F_mostra_elimina_pa(int r,int c, int el);

/* verifica l'esistenza di un percorso */
int F_check_percorso(Str P);

/* richiesta di animazione */
void F_richiesta_animazione(Str P, int rP, int cP, int rA, int cA);

/* effettua l'animazione del personaggio */
void F_animazione_percorso(Str P,int rP, int cP, int rA, int cA);

/*  visualizza il percorso trovato */
void F_mostra_percorso(Str P,int rP,int cP,int rA,int cA);

/* permette la selezione di una cella del labirinto */
InfNodo F_preleva_cella(Str P);

/* verifica che la cella scelta sia corretta */
int F_check_cella_in_labirinto(Str P,MEVENT event);

/* Procedura di test */
void F_stampa_matrice(Str P);


#endif //LASD_FINAL_2_L_UTILITY_H
