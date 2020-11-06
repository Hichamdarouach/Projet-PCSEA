#ifndef __FIFO_PROCESS_H__
#define __FIFO_PROCESS_H__

#include "proc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct For_fifo_process
{
    Process *process;
    struct For_fifo_process *next;
};

//Pointer on the tail. Must be actualise with return value of functions called
typedef struct For_fifo_process *Fifo_process;

//Point on tail (last element) and circular chain
Fifo_process init_list();
bool is_empty(Fifo_process);
Fifo_process init_element(Process *);
Fifo_process add_process(Fifo_process, Process *);
Fifo_process extract_process(Fifo_process, Process **);

bool contains(Fifo_process fifo, int pid);
int getSize(Fifo_process fifo);

void destroy_element(Fifo_process);

#endif