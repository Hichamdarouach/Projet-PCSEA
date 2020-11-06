#include <string.h>
#include "cpu.h"
#include "display.h"

// Size of the display
#define NB_COL 80
#define NB_LIG 25

// Default position of the cursor
#define POSI_L_INIT 1
#define POSI_C_INIT 0

uint32_t curs_lig_pos = POSI_L_INIT; // Pour commencer en dessous des instructions au lancement
uint32_t curs_col_pos = POSI_C_INIT;

/**
 * fonction qui renvoie un pointeur sur la case mémoire correspondant
 * aux coordonnées fournies
 */
uint16_t *ptr_mem(uint32_t lig, uint32_t col)
{
    return (uint16_t *)(0xB8000 + 2 * (lig * NB_COL + col));
}

/**
 * fonction qui écrit le caractère c aux coordonnées spécifiées
 */
void ecrit_car(uint32_t lig, uint32_t col, char c, uint32_t ct,
               uint32_t cf, uint32_t cl)
{
    uint16_t *ptr = ptr_mem(lig, col);

    *ptr = c | (cl << 15) | (cf << 12) | (ct << 8);
    // '|' signifie 'ou'
}

void efface_ecran(void)
{
    int lig, col;

    for (lig = 0; lig < NB_LIG; lig++)
    {
        for (col = 0; col < NB_COL; col++)
        {
            ecrit_car(lig, col, ' ', 15, 0, 0);
        }
    }
}

void place_curseur(uint32_t lig, uint32_t col)
{
    uint16_t pos = col + lig * NB_COL;
    uint16_t low_pos, high_pos;

    low_pos = pos;
    high_pos = pos;

    outb(0x0F, 0x3D4);
    outb(((low_pos << 8) >> 8), 0x3D5); //pos >> 8

    outb(0x0E, 0x3D4);
    outb(high_pos >> 8, 0x3D5); //pos << 8

    curs_lig_pos = lig;
    curs_col_pos = col;
}

int next_tab(void)
{
    if (curs_col_pos < 8)
        return 8;
    else if (curs_col_pos < 16)
        return 16;
    else if (curs_col_pos < 24)
        return 24;
    else if (curs_col_pos < 32)
        return 32;
    else if (curs_col_pos < 40)
        return 40;
    else if (curs_col_pos < 48)
        return 48;
    else if (curs_col_pos < 56)
        return 56;
    else if (curs_col_pos < 64)
        return 64;
    else if (curs_col_pos < 72)
        return 72;
    else if (curs_col_pos <= 79)
        return 79;

    return 0;
}

void traite_car(char c)
{
    if ((c >= 32) && (c <= 126))
    {
        ecrit_car(curs_lig_pos, curs_col_pos, c, 15, 0, 0);
        curs_col_pos++;
        if (curs_col_pos == NB_COL)
        {
            curs_col_pos = POSI_C_INIT;
            if (curs_lig_pos != NB_COL - 1)
                curs_lig_pos++;
            else
                defilement();
        }
    }
    else if (c == 8)
    { // \b
        if (curs_col_pos != 0)
            curs_col_pos--;
    }
    else if (c == 9)
    { // \t
        curs_col_pos = next_tab();
        if (!curs_col_pos)
            curs_lig_pos = POSI_L_INIT;
    }
    else if (c == 10)
    { // \n
        if (curs_lig_pos != NB_LIG - 1)
        {
            curs_lig_pos++;
        }
        else
            defilement();
        curs_col_pos = POSI_C_INIT;
    }
    else if (c == 12)
    { // \f
        efface_ecran();
        curs_lig_pos = POSI_L_INIT;
        curs_col_pos = POSI_C_INIT;
    }
    else if (c == 13)
    { // \r
        curs_col_pos = POSI_C_INIT;
    }

    place_curseur(curs_lig_pos, curs_col_pos);
}

void defilement(void)
{
    memmove(ptr_mem(POSI_L_INIT, 0), ptr_mem(POSI_L_INIT + 1, 0), (NB_LIG - 1) * NB_COL * 2);

    if (curs_lig_pos > POSI_L_INIT)
        place_curseur(curs_lig_pos - 1, curs_col_pos);

    for (int i = 0; i < NB_COL; ++i)
        ecrit_car(24, i, ' ', 15, 0, 0);
}

void console_putbytes(char *chaine, int32_t taille)
{
    int i;
    for (i = 0; i < taille; i++) // Erase the final line
        traite_car(chaine[i]);
}

uint32_t getC()
{
    // Get the position of col
    return (curs_col_pos);
}

uint32_t getL()
{
    // Get the position of li
    return (curs_lig_pos);
}