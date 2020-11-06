#include "message.h"

void initialisation_fifo_tab()
{
    for (int i = 0; i < NBQUEUE; i++)
    {
        fifo_tab[i].used = false;
    }
}

int pcreate(int count)
{
    bool location = false;
    int FID = -1;

    if (count < 1) {
        return -1;
    }
    
    for (int i = 0; i < NBQUEUE && !location; i++)
    {
        // Search for a place
        if (!fifo_tab[i].used)
        {
            location = true;
            FID = i;
        }
    }

    if (!location)
    { // No place available
        return (-1);
    }

    //Initialisation
    fifo_tab[FID].fid = FID;

    fifo_tab[FID].down = 0;
    fifo_tab[FID].size = count;

    fifo_tab[FID].consum = init_list();
    fifo_tab[FID].prod = init_list();

    fifo_tab[FID].used = true;
    fifo_tab[FID].nbElement = 0;

    fifo_tab[FID].tab_messages = (int *)mem_alloc(sizeof(int) * fifo_tab[FID].size);

    return FID;
}

int pdelete(int fid)
{
    if (fid < 0 || fid >= NBQUEUE || fifo_tab[fid].used == false)
    {
        // Invalid parameter
        return -1;
    }

    fifo_tab[fid].used = false;
    mem_free((void *)fifo_tab[fid].tab_messages, sizeof(int) * fifo_tab[fid].size);

    Process *tmp;
    while (!is_empty(fifo_tab[fid].consum))
    {
        fifo_tab[fid].consum = extract_process(fifo_tab[fid].consum, &tmp);
        insert_activable(tmp);
        add_process(shouldReceiveNegativeOne, tmp);
    }
    while (!is_empty(fifo_tab[fid].prod))
    {
        fifo_tab[fid].consum = extract_process(fifo_tab[fid].prod, &tmp);
        insert_activable(tmp);
        add_process(shouldReceiveNegativeOne, tmp);
    }

    return 0;
}

int psend(int fid, int message)
{
    if (fid < 0 || fid >= NBQUEUE || fifo_tab[fid].used == false)
    {
        // Check that the call is correct
        return -1;
    }

    Fifo_messages *f = &(fifo_tab[fid]);
    Process *tmp;
    tmp = actif;

    if (f->nbElement == f->size)
    {
        // FIFO full
        // Get the process to wait

        f->consum = add_process(f->consum, tmp);
        tmp->state = ENDORMI;
        ordonnance();
    }

    if (contains(shouldReceiveNegativeOne, tmp->pid))
    {
        return (-1);
    }

    // Add message to the tab
    f->tab_messages[(f->down + f->nbElement) % f->size] = message;
    f->nbElement += 1;

    if (!is_empty(f->prod))
    {
        // Wake a process which is waiting
        f->prod = extract_process(f->prod, &tmp);
        insert_activable(tmp);
    }

    return 0;
}

int preceive(int fid, int *message)
{
    if (fid < 0 || fid >= NBQUEUE || fifo_tab[fid].used == false)
    {
        // Check that the call is correct
        return -1;
    }

    Fifo_messages *f = &(fifo_tab[fid]);
    Process *tmp;
    tmp = actif;

    if (f->nbElement == 0)
    {
        // FIFO empty
        // Get the process to wait

        f->prod = add_process(f->consum, tmp);
        tmp->state = ENDORMI;
        ordonnance();
    }

    if (contains(shouldReceiveNegativeOne, tmp->pid))
    {
        return (-1);
    }

    // Remove message from the tab

    *message = f->tab_messages[f->down];
    f->nbElement -= 1;
    f->down++;
    if (f->down == f->size)
    {
        f->down = 0;
    }

    if (!is_empty(f->consum))
    {
        // Wake a process which is waiting
        f->consum = extract_process(f->consum, &tmp);
        insert_activable(tmp);
    }

    return 0;
}

int preset(int fid)
{
    if (fid < 0 || fid >= NBQUEUE || fifo_tab[fid].used == false)
    {
        // Check that the call is correct
        return -1;
    }

    Fifo_messages *f = &(fifo_tab[fid]);

    Process *tmp;
    f->down = 0;
    f->nbElement = 0;

    while (!is_empty(fifo_tab[fid].consum))
    {
        fifo_tab[fid].consum = extract_process(fifo_tab[fid].consum, &tmp);
        insert_activable(tmp);
        add_process(shouldReceiveNegativeOne, tmp);
    }
    while (!is_empty(fifo_tab[fid].prod))
    {
        fifo_tab[fid].consum = extract_process(fifo_tab[fid].prod, &tmp);
        insert_activable(tmp);
        add_process(shouldReceiveNegativeOne, tmp);
    }

    return (0);
}

int pcount(int fid, int *count)
{
    if (fid < 0 || fid >= NBQUEUE)
    {
        return -1;
    }
    Fifo_messages f = fifo_tab[fid];

    int consum = getSize(f.consum);
    int data = getSize(f.prod);

    data += f.nbElement;

    //Value of count
    if (consum != 0)
    {
        *count = -consum;
    }
    else if (data != 0)
    {
        *count = data;
    }

    return (0);
}