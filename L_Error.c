#include <stdio.h>
#include <stdlib.h>
#include "L_Error.h"
#include "L_Utility.h"


/*
 *  Descrizione:   procedura di gestione errori.
 *  Dettagli:      //
 *
 *  Parametri in:  e->intero indicante l'errore.
 *  Parametri out: //
 *
*/
void F_error(int e)
{
    /* terminazione della modalita' curses */
    F_termina_curses();
    switch (e)
    {
        case 1:
            puts("\nError: errore di collegamento celle in: L_Prim - F_dig_to!\n");
            break;
        case 2:
            puts("\nError: errore di selezione di cella random in: L_Prim - F_seleziona_cella_random\n");
            break;
        case 3:
            puts("\nError: errore di ricerca di celle unvisited in: L_Prim - F_controllo_unvisited\n");
            break;
        case 4:
            puts("\nError: errore di selezione nel menu' principale in: L_Utility - F_stampa_menu_principale\n");
            break;
        case 5:
            puts("\nError: errore di selezione nel menu' di generazione del labirinto in: L_Utility - F_stampa_menu_labirinto");
            break;
        case 6:
            puts("\nError: errore di ricerca delle celle adiacenti in: L_Dijstra - F_dijkstra\n");
            break;
        case 7:
            puts("\nError: errore di valore di stima in: L_Heap - F_decrease_key_albero\n");
            break;
        case 8:
            puts("\nError: errore di inserimento carattere nel labirinto in: L_Utility - F_mostra_elimina_pa\n");
            break;
        case 9:
            puts("\nError: errore randomizzazione valore numerico del n° di muri da eliminare in: L_Maze - F_elimina_vicoli_ciechi\n");
            break;
        case 10:
            puts("\nError: errore di selezione della direzione random da scegliere in: L_Recursive - F_recursive_maze\n");
            break;
        case 11:
            puts("\nError: memoria non allocata per l'inserimento di un elemento nella cosa in: L_Coda - F_enqueue\n");
            break;
        case 12:
            puts("\nError: indice di muro per la sua eliminazione errato in: L_Maze - F_elimina_muri\n");
            break;
        case 13:
            puts("\nError: errore allocazione spazio destinato ad un elemento dello stack in L_Stack - F_push\n");
            break;
        default:
            puts("\nError: errore generico!\n");
            break;
    }
    exit(-1);
}