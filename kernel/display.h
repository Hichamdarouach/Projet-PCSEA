#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include <stdint.h>

uint16_t *ptr_mem(uint32_t lig, uint32_t col);
void ecrit_car(uint32_t lig, uint32_t col, char c, uint32_t ct, 
               uint32_t cf, uint32_t cl);
void efface_ecran(void);
void place_curseur(uint32_t lig, uint32_t col);
void traite_car(char c);
int next_tab(void);
void defilement(void);
void console_putbytes(char *chaine, int32_t taille);

uint32_t getL();
uint32_t getC();

#endif