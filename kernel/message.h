#ifndef __MESSAGE_H__
#define __MESSAGE_H__

//Max number of files
#define NBQUEUE 10

#include "proc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fifo_process.h"

typedef struct Fifo_messages
{
    Fifo_process consum;
    Fifo_process prod;

    int *tab_messages;

    int size;

    int nbElement;
    int down;

    int fid;
    bool used;
} Fifo_messages;

//Should be an Hashmap in futur
Fifo_process shouldReceiveNegativeOne;
Fifo_messages fifo_tab[NBQUEUE];

void initialisation_fifo_tab();
int pcreate(int count);
int pdelete(int fid);
int psend(int fid, int message);
int preceive(int fid, int *message);
int preset(int fid);

// int pcount(int fid, int *count);

#endif