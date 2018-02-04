#ifndef LASD_FINAL_2_L_DIVIDE_H
#define LASD_FINAL_2_L_DIVIDE_H

#include "L_Struttura.h"

/* inizializzazione prima della procedura Divide */
void F_init_divide(Str P);

/*  eliminazione di tutti i muri */
void F_elimina_tutti_muri(Str P);

/* generatore a suddivisione ricorsiva */
void F_divide(Str P,int r, int c,int h, int w);

/* divisione della stanza in orizzontale */
void F_divide_horizontally(Str P,int r,int c,int h, int w);

/* divisione della stanza in verticale */
void F_divide_vertically(Str P, int r, int c, int h,int w);

/* scelta del valore di generazione del muro */
int F_random_even(int vound);

/*  scelta del valore di generazione della porta */
int F_random_odd(int bound);


#endif //LASD_FINAL_2_L_DIVIDE_H
