#include "cpu.h"
#include <string.h>
#include <stdbool.h>
#include "segment.h"
#include <stdint.h>
#include "display.h"
#include <stdio.h>
#include "segment.h"
#include "clock.h"
#include "interrupt.h"
#include "proc.h"

extern void traitant_IT_32(void);

unsigned long nb_tick = 0;

uint32_t sec = 0;
uint32_t min = 0;
uint32_t hour = 0;

void affiche_clock()
{
    char time[9];
    sprintf(time, "%2d:%2d:%2d", hour, min, sec);
    for (unsigned int i = 0; i < 9; i++)
    {
        ecrit_car(0, 71 + i, *(time + i), 15, 0, 0);
    }
}

void wait_clock(unsigned long clock)
{
    sleep_until(clock/CLOCKFREQ);
}

void tic_PIT_CLOCK(void)
{
    //On acquitte l'interruption
    outb(0x20, 0x20);
    nb_tick++;
    if (nb_tick % CLOCKFREQ == 0)
    {
        sec++;
        change_clock();
    }
    ordonnance();
}

void change_clock()
{
    uint32_t new_minute = sec / 60;
    sec = sec - new_minute * 60;
    min += new_minute;
    uint32_t new_hour = min / 60;
    min = min - new_hour * 60;
    hour += new_hour;
    if (hour > 99)
    {
        hour = 99;
        min = 59;
        sec = 59;
    }
    affiche_clock();
}

void clock_freq()
{
    uint16_t reglage = QUARTZ / CLOCKFREQ;
    //Envoie de 0x34 sur le port 0x43
    outb(0x34, 0x43);
    //Etape 2 : Envoie des 8 bits de poids faible
    outb((reglage) % 256, 0x40);
    //Etape 3 : Envoie des 8 bits de poids forts
    outb(reglage >> 8, 0x40);
}

void init_clock()
{
    affiche_clock();
    // init the freq of the clock
    clock_freq();
    // Open the IRQ0
    open_IRQ(0, false);
    // Attach function to the interrupt
    init_traitant_IT(32, &traitant_IT_32);
}

unsigned long current_clock()
{
    return nb_tick;
}

