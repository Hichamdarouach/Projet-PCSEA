#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <stdbool.h>
#include <stdint.h>

#include "display.h"

#define QUARTZ 0x1234DD
#define CLOCKFREQ 100
#define SCHEDFREQ 50

void init_clock();
void clock_freq();
void tic_PIT_CLOCK(void);

void affiche_clock();
void change_clock();

void clock_settings(unsigned long *quartz, unsigned long *ticks);
unsigned long current_clock();
void wait_clock(unsigned long clock);
void tic_PIT(void);
void init_frequence();

#endif