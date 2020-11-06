#include "fifo_process.h"

Fifo_process init_list()
{
    return NULL;
}

bool is_empty(Fifo_process fifo)
{
    return fifo == NULL;
}

Fifo_process init_element(Process *process)
{
    Fifo_process element = (Fifo_process)mem_alloc(sizeof(struct For_fifo_process));

    element->process = process;
    element->next = NULL;

    return element;
}

Fifo_process add_process(Fifo_process fifo, Process *process)
{
    Fifo_process new = init_element(process);
    if (is_empty(fifo))
    { // Case fifo is empty
        new->next = new;
        return (new);
    }

    // Fifo contains at least 1 element
    new->next = fifo->next;
    fifo->next = new;

    return new;
}

// Fifo need to contain at least one element
Fifo_process extract_process(Fifo_process fifo, Process **process_extracted)
{
    if (is_empty(fifo))
    {
        return (NULL);
    }

    Fifo_process temp = fifo->next;
    *process_extracted = temp->process; //Addr of process (because process if a pointer)

    if (fifo->next == fifo)
    //In case of one element chained
    {
        fifo = NULL;
    }
    else
    {
        fifo->next = fifo->next->next;
    }
    destroy_element(temp);

    return fifo;
}

bool contains(Fifo_process fifo, int pid)
{
    Fifo_process tmp = fifo;

    while (!is_empty(tmp))
    {
        if (tmp->process->pid == pid)
        {
            return (true);
        }
        tmp = tmp->next;
    }

    return (false);
}

int getSize(Fifo_process fifo)
{
    Fifo_process temp = fifo;
    int size = 0;

    while (!is_empty(temp))
    {
        size++;
        temp = temp->next;
    }

    return (size);
}

void destroy_element(Fifo_process element)
{
    mem_free((void *)element, sizeof(struct For_fifo_process));
}
