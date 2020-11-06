#include "proc.h"
#include "message.h"
#include "kernel-sysapi.h"

/** Priority summary **/
/*
- Plus la priorité est grande (entre 1 et MAXPRIO), plus le processus est prio
- Un proc s'exécute jamais tant qu'il reste un proc actif ou activable avec une
prio plus élevée
- Les prio sont définies par l'utilisateur
*/

LIST_HEAD(head_queue); // Init the queue for Activable process


uint32_t process_count = 0; /* number of process that exist or has existed */
uint32_t process_alive = 0; /* number of process that are alive */

struct Process *endormi_list;
struct Process *endormi_tail = NULL;

struct Process *dead_list = NULL;
struct Process *dead_tail = NULL;

void ctx_sw(int *a, int *b);

/*struct Process *creer_liste_process() {
    struct Process *list;
    list = malloc(sizeof(struct Process));
    return list;
}*/

void insert_mort(struct Process *element)
{
    element->state = MORT;
    if (dead_list == NULL)
    {
        dead_list = element;
        dead_tail = element;
    }
    else
    {
        dead_tail->suiv = element;
        dead_tail = dead_tail->suiv;
    }
    element->suiv = NULL;
}

struct Process *extract_mort()
{
    if (dead_list == NULL)
    {
        return NULL;
    }
    else
    {
        struct Process *tmp = dead_list;

        if (dead_list == dead_tail)
        { // 1 seul elt
            dead_list = NULL;
            dead_tail = NULL;
        }
        else
        {
            dead_list = dead_list->suiv;
        }
        return tmp;
    }
}

struct Process *extract_activable(void)
{
    struct Process *tmp;
    if (queue_empty(&head_queue))
    {
        tmp = NULL;
    }
    else
    {
        tmp = queue_out(&head_queue, Process, my_link);
        tmp->suiv = NULL;
    }
    //printf("Fin extract activable\n");
    //printf("pid = %d\n", tmp->pid);

    return tmp;
}

void insert_activable(struct Process *element)
{
    element->state = ACTIVABLE;
    element->suiv = NULL;
    queue_add(element, &head_queue, Process, my_link, prio);
}

void nbre_endormi()
{
    struct Process *tmp = endormi_list;
    int i = 0;

    while (tmp != NULL && endormi_tail != NULL)
    {
        tmp = tmp->suiv;
        i++;
    }
    //printf("Il y a %d process endormi\n", i);
}

void print_list_endormi()
{
    struct Process *tmp = endormi_list;

    while (tmp != NULL && endormi_tail != NULL)
    {
        printf("[time = %ld] %s : %d | %f\n", current_clock(), tmp->name, tmp->pid, tmp->time_to_wake);
        tmp = tmp->suiv;
    }
}

struct Process *extract_endormi(void)
{
    struct Process *tmp = endormi_list;

    /* Un seul élément dans la liste */
    if (endormi_list == endormi_tail)
    {
        endormi_list = NULL;
        endormi_tail = NULL;
    }
    else
    {
        endormi_list = endormi_list->suiv;
    }

    tmp->suiv = NULL;
    return tmp;
}

void insert_endormi(struct Process *element, double time)
{
    element->time_to_wake = time;
    element->state = ENDORMI;

    if (endormi_tail == NULL)
    {
        element->suiv = NULL;
        endormi_list = element;
        endormi_tail = element;
        return;
    }
    else
    {
        bool already_add = false;
        bool is_head = true;
        struct Process *tmp = endormi_list;
        struct Process *prec = tmp;

        while (tmp != NULL)
        {
            if (tmp->time_to_wake > element->time_to_wake)
            {
                if (is_head)
                {
                    struct Process *old_head = tmp;
                    endormi_list = element;
                    element->suiv = old_head;
                    is_head = false;
                }
                else
                {
                    prec->suiv = element;
                    element->suiv = tmp;
                }
                already_add = true;
                break;
            }
            if (is_head)
            {
                tmp = tmp->suiv;
                is_head = false;
            }
            else
            {
                prec = prec->suiv;
                tmp = tmp->suiv;
            }
        }

        if (!already_add)
        {
            endormi_tail->suiv = element;
            endormi_tail = endormi_tail->suiv;
        }
        endormi_tail->suiv = NULL;
    }
}

/* Retourne 0 si le processus était dans la liste et a été supprimé
 * Retourne -1 si le pid n'est pas dans la liste
*/
int sleep_del(int pid)
{
    if (endormi_list == NULL)
    {
        return -1;
    }

    struct Process *tmp;

    if (endormi_list->pid == pid)
    {
        if (endormi_list == endormi_tail)
            endormi_tail = NULL;

        tmp = endormi_list;
        endormi_list = endormi_list->suiv;
        tmp->suiv = NULL;
        insert_mort(tmp);

        return 0;
    }

    tmp = endormi_list->suiv;
    struct Process *prec = endormi_list;

    while (tmp != NULL)
    {
        if (tmp->pid == pid)
        {
            if (tmp->pid == endormi_tail->pid)
                endormi_tail = prec;

            prec->suiv = tmp->suiv;
            tmp->suiv = NULL;
            insert_mort(tmp);
        }
        prec = tmp;
        tmp = tmp->suiv;
    }

    return -1;
}

int idle(void *a)
{
    a = (int *)a;
    for (;;)
    {
        //printf("Dans Idle\n");
        sti();
        hlt();
        cli();
    }
    return 0;
} //On acquitte l'interruption

int proc1(void *a)
{
    int val;
    int valRet;
    valRet = preceive(0, &val);
    printf("PROC 1 Val : %d; Ret : %d\n", val, valRet);
    valRet = preceive(0, &val);
    printf("PROC 1 Val : %d; Ret : %d\n", val, valRet);
    valRet = preceive(0, &val);
    printf("PROC 1 Val : %d; Ret : %d\n", val, valRet);
    valRet = preceive(0, &val);
    printf("PROC 1 Val : %d; Ret : %d\n", val, valRet);
    valRet = preceive(0, &val);
    printf("PROC 1 Val : %d; Ret : %d\n", val, valRet);
    valRet = preceive(0, &val);
    printf("PROC 1 Val : %d; Ret : %d\n", val, valRet);
    psend(0, 5);

    a = (int *)a;
    // for (int32_t i = 0; i < 56; i++)
    // {
    //     printf("[temps = %lu] processus %s pid = %i\n", current_clock(), mon_nom(), getpid());
    //     //dors(4);
    //     wait_clock(50);
    // }
    return 0;
}

int proc2(void *a)
{
    int valRet;
    valRet = psend(0, 3);
    printf("PROC 2, VALRET : %d\n", valRet);
    a = (int *)a;
    int b = 0;
    for (int i = 0; i < 200000000; ++i)
    {
        b = b + i;
    }
    for (int32_t i = 0; i < 104; i++)
    {
        printf("[temps = %lu] processus %s pid = %i\n", current_clock(), mon_nom(), getpid());
        wait_clock(50);
    }
    return 0;
}

int proc3(void *a)
{
    a = (int *)a;
    for (int32_t i = 0; i < 235; i++)
    {
        printf("[temps = %lu] processus %s pid = %i\n", current_clock(), mon_nom(), getpid());
        wait_clock(50);
    }
    return 0;
}

void proc4(void)
{
    for (int32_t i = 0; i < 3; i++)
    {
        printf("[temps = %lu] processus %s pid = %i\n", current_clock(), mon_nom(), getpid());
        wait_clock(50);
    }
}

void proc5(void)
{
    for (int32_t i = 0; i < 7; i++)
    {
        printf("[temps = %lu] processus %s pid = %i\n", current_clock(), mon_nom(), getpid());
        wait_clock(300);
    }
}

void proc6(void)
{
    for (int32_t i = 0; i < 5; i++)
    {
        printf("[temps = %lu] processus %s pid = %i\n", current_clock(), mon_nom(), getpid());
        wait_clock(150);
    }
}

void proc7(void)
{
    for (int32_t i = 0; i < 8; i++)
    {
        printf("[temps = %lu] processus %s pid = %i\n", current_clock(), mon_nom(), getpid());
        wait_clock(20);
    }
}

void init_process(void)
{
    start(idle, 512, 1, "idle", NULL);
    /*start(proc1, 512, 100, "proc1", NULL);
    start(proc2, 512, 150, "proc2", NULL);
    start(proc3, 512, 200, "proc3", NULL);*/
    /*start(proc4, 512, 200, "proc4", NULL);
    start(proc5, 512, 200, "proc5", NULL);
    start(proc6, 512, 200, "proc6", NULL);
    start(proc7, 512, 200, "proc", NULL);*/

    // On lance idle au début;
    actif = &process[0];
}

void ordonnance(void)
{
    //display_queue(1);
    //printf("Ordonnance, prio proc : %d", getprio(getpid()));
    wake_process();
    //display_queue(1);

    uint32_t old_pid = actif->pid;
    struct Process *new_actif;
    //if (actif->pid ==0 )
    //printf("Idle dans ordonnance\n");
    // pas extraire idle alors que le processus courant est plus prioritaire
    if (!queue_empty(&head_queue) && ((queue_top(&head_queue, Process, my_link))->prio >= actif->prio || actif->state == ENDORMI))
        new_actif = extract_activable();
    else
        new_actif = actif;

    if (new_actif == NULL)
    {
        //printf("Pas d'activable\n");
        return;
        //ctx_sw(process[actif->pid]->regs, process[actif->pid]->regs);
    }
    else
    {
        uint32_t next_pid = new_actif->pid;

        if (actif->state != ENDORMI && actif != new_actif)
            insert_activable(actif);
        //printf("actif : %d | new actif : %d\n", actif->pid, new_actif->pid);
        actif = new_actif;
        //printf("%p    %p", process[old_pid].regs, process[next_pid].regs);
        ctx_sw(process[old_pid].regs, process[next_pid].regs);
    }
}

int32_t getpid(void)
{
    return actif->pid;
}

char *mon_nom(void)
{
    return actif->name;
}

/*uint32_t nbr_secondes() {
    return sec_since_launch;
}*/

int start(int (*pt_func)(void *), unsigned long ssize, int prio,
          const char *name, void *arg)
{

    if (ssize >= 0xffffffff)
        return -1; 

    if (prio >= MAX_PRIO && prio < 0)
        return -1;

    if (process_alive == PROCESS_TABLE_SIZE)
        return -1;

    int index;
    struct Process *new_p = extract_mort();
    if (new_p == NULL)
    {
        index = process_alive;
    }
    else
    {
        index = new_p->pid;
    }

    process[index].pid = index;
    strcpy(process[index].name, name);
    process[index].prio = prio;

    //process[index].exec_stack = mem_alloc(STACK_SIZE_INTEGER * sizeof(int));
    process[index].regs[1] = (int)(process[index].exec_stack + (STACK_SIZE_INTEGER - 3));
    process[index].exec_stack[STACK_SIZE_INTEGER - 3] = (int)pt_func;
    process[index].exec_stack[STACK_SIZE_INTEGER - 2] = (int)fin_processus;
    process[index].exec_stack[STACK_SIZE_INTEGER - 1] = (int)arg;

    process[index].pere = actif;
    process[index].dernier_fils = NULL;
    process[index].frere_droit = NULL;
    process[index].premier_fils = NULL;
    process[index].suiv = NULL;
    process[index].retval = NULL;
    process[index].pid_ret_fils = -1;

    if (actif != NULL)
    {
        if (actif->premier_fils == NULL)
        {
            actif->premier_fils = &process[index];
            actif->dernier_fils = &process[index];
        }
        else
        {
            actif->dernier_fils->frere_droit = &process[index];
            actif->dernier_fils = actif->dernier_fils->frere_droit;
        }
    }

    if (index > 0)
        insert_activable(&process[index]);

    int32_t pid = process[index].pid;
    process_alive++;

    return pid;
}

bool is_pid_valid(int pid)
{
    if (pid < 0 || pid > PROCESS_TABLE_SIZE - 1)
        return false;

    if (actif->pid == pid)
    {
        return true;
    }
    //else if dans une liste autre que mort{
    // alors return true;
    //}
    //else { soit dans mort soit pas allouer
    // alors on return false;
    //}

    return false; // a supprimer
}

int kill(int pid)
{
    if (pid <= 0 || pid > PROCESS_TABLE_SIZE - 1)
        return -1;

    if (actif->pid == pid)
    {
        actif->retval = 0;
        fin_processus();
    }
    else
    {
        struct Process *tmp;

        // Recherche dans les activables
        queue_for_each(tmp, &head_queue, Process, my_link)
        {
            if (tmp->pid == pid)
            {
                tmp->state = ZOMBIE;
                queue_del(tmp, my_link);
                return 0;
            }
        }

        // Recherche dans les endormis
        if (sleep_del(pid) == 0)
            return 0;

        // PID non actif
        return -1;
    }

    return 0; // a supprimer
}

int getprio(int pid)
{
    if (pid < 0 || pid > PROCESS_TABLE_SIZE - 1)
        return -1;

    if (actif->pid == pid)
        return actif->prio;
    else
    {
        struct Process *tmp;

        // Recherche dans les activables
        queue_for_each(tmp, &head_queue, Process, my_link)
        {
            if (tmp->pid == pid)
                return tmp->prio;
        }

        // Recherche dans les endormis
        tmp = endormi_list;
        while (tmp != NULL)
        {
            if (tmp->pid == pid)
                return tmp->prio;
            tmp = tmp->suiv;
        }

        // PID non actif
        return -1;
    }
}

int chprio(int pid, int newprio)
{
    int oldprio = -1;

    if (pid < 0 || pid > PROCESS_TABLE_SIZE - 1 || newprio < 1 || newprio > MAX_PRIO)
    {

        return oldprio;
    }

    if (actif->pid == pid)
    {
        oldprio = actif->prio;
        actif->prio = newprio;
    }
    else
    {
        struct Process *tmp;

        // Recherche dans les activables
        queue_for_each(tmp, &head_queue, Process, my_link)
        {
            if (tmp->pid == pid)
            {
                queue_del(tmp, my_link);
                oldprio = tmp->prio;
                tmp->prio = newprio;
                queue_add(tmp, &head_queue, Process, my_link, prio);
            }
        }

        // Recherche dans les endormis
        tmp = endormi_list;
        while (tmp != NULL)
        {
            if (tmp->pid == pid){
                //return tmp->prio;
                oldprio = tmp->prio;
                tmp->prio = newprio;
            }
            tmp = tmp->suiv;
        }

    }
    ordonnance();
    return oldprio;
}

int waitpid(int pid, int *retvalp)
{

    bool trouve_fils = false;
    int pid_fils_termine = -1;

    // Parcours de la liste des fils, vérification que le pid "pid" est bien dans la liste des fils
    struct Process *tmp = actif->premier_fils;

    if (pid < 0 && actif->premier_fils != NULL){
        // si on passe un paramètre négatif et que l'actif a au moins 1 fils, on attends n'importe quel fils
        trouve_fils = true;
    }else{
        while (tmp != NULL){
            if (tmp->pid == pid && tmp->state == MORT)
                break;
            if (tmp->pid == pid){
                trouve_fils = true;
                break;
            }
            if (tmp == tmp->frere_droit)
                break;
            tmp = tmp->frere_droit;
        }
        /*if (!trouve_fils){
            while (tmp != NULL){
                if (tmp->pid == pid){
                    trouve_fils = true;
                    break;
                }
                tmp = tmp->premier_fils;
            }
        }*/
    }

    // le pid passé en paramètre a été trouvé, il s'agit effectivement d'un fils
    if (trouve_fils){
      if (pid < 0){
          // attente que n'importe quel des fils soit terminé
          while ((pid_fils_termine = pid_dun_fils_termine(-1)) < 0){}
      }else if (pid > 0){
          // attente que le fils de pid "pid" soit terminé
          while ((pid_fils_termine = pid_dun_fils_termine(pid)) != pid ){}
      }

      if (actif->retval_fils != NULL){
          *retvalp = *(actif->retval_fils);
          actif->retval_fils = NULL;
      }

        if (actif->retval_fils != NULL)
        {
            retvalp = actif->retval_fils;
            actif->retval_fils = NULL;
        }

        return pid_fils_termine;
    }
    else
    {
        (void)retvalp;
        return -1;
    }
}

// param : -1 pour attendre tout les fils, une valeur >= à 0 pour attendre un fils précis
int pid_dun_fils_termine(int param){
    struct Process *fils = actif->premier_fils;
    int val = 1;
    while (val){
        test_it();
        if (fils->state == ZOMBIE){
            //zombie : 1,2,3
            // si on prendre n'importe quel fils ou qu'on sélectionne un fils de pid précis

            if(!strcmp(fils->name,"waiter")){
                printf("  ");
            }

            if (param == -1 || fils->pid == param){
                int pid_fils = fils->pid;

                actif->retval_fils = fils->retval;
                //suppression_fils_zombie(fils);
                insert_mort(fils);
                return pid_fils;
            }else{
                fils = fils->frere_droit;
                if (fils == NULL)
                    fils = actif->premier_fils;
            }
        }else{
            fils = fils->frere_droit;
            if (fils == NULL)
                fils = actif->premier_fils;
        }
        if (param == fils->pid && fils->prio < actif->prio){
            chprio(param, getprio(param)+1);
        }
    }
    return -1;
}

void suppression_fils_zombie(struct Process* fils_a_supprimer){

    struct Process *fils_precedent_parcouru = NULL;
    struct Process *fils_actuellement_parcouru = actif->premier_fils;

    while (fils_actuellement_parcouru != NULL && actif->state != ZOMBIE){
        // si le fils du pere actuellement parcouru est l'actif
        if (fils_actuellement_parcouru == fils_a_supprimer){

            // on supprime pas la tete de la liste
            if (fils_precedent_parcouru != NULL){
                if (fils_actuellement_parcouru != NULL){
                    // suppression en milieu de liste
                    //a->b->c : a->c
                    fils_precedent_parcouru->frere_droit = fils_actuellement_parcouru->frere_droit;
                }else{
                    // suppression en fin de liste (dernier élément)
                    // le frere droit du fils precedent parcouru devient NULL
                    fils_precedent_parcouru->frere_droit = NULL;

                    // c->d->(a supprimer) = c->d
                }
            }else{
                // dans ce cas, c'est la suppression de la tete de liste qui doit etre faite
                // parcours_fils_pere est censé toujours être égal à la tete de liste dans ce cas

                // fils_actuellement_parcouru = fils_actuellement_parcouru->frere_droit;

                actif->premier_fils = NULL;

                // (a supprimer)->b->c => b->c
                break;
            }
        }
        fils_precedent_parcouru = fils_actuellement_parcouru;
        fils_actuellement_parcouru = fils_actuellement_parcouru->frere_droit;
    }
}

void exit(int retval) {
    /*if(actif->pere->state == SEMAPHORE){
        actif->pere->pid_ret_fils = getpid();
        actif->pere->retval_fils = &retval;
    }*/


    actif->retval = &retval;

    fin_processus();
    while (1)
    {
    }
}

void dors(uint32_t nbr_secs)
{
    insert_endormi(actif, nbr_secs);
    ordonnance();
}

void sleep_until(double time)
{
    insert_endormi(actif, time);
    ordonnance();
}

void wake_process()
{
    struct Process *tmp = endormi_list;

    while (tmp != NULL && tmp->time_to_wake <= current_clock() && endormi_tail != NULL)
    {
        insert_activable(extract_endormi());
        tmp = tmp->suiv;
    }
}

void fin_processus()
{

    if (actif->pere == NULL)
        insert_mort(actif);
    else{
        //faire devenir l'actif comme zombie
        // à noter que l'actif est pas insérer dans les morts comme dans les activables
        // il semble qu'il est totalement en dehors de toutes les listes
        actif->state = ZOMBIE;
        actif->pere->pid_ret_fils = getpid();
    }


    // dans le cas ou l'actif devient mort ou zombie.
    // mettre les pointeurs "actif->pere"
    // de tout les fils à NULL

    struct Process *parcours_liste_fils = actif->premier_fils;
    while (parcours_liste_fils != NULL){

        parcours_liste_fils->pere = NULL;

        if (parcours_liste_fils->state == ZOMBIE){

            struct Process *frere_du_proc_supprime;

            frere_du_proc_supprime = parcours_liste_fils->frere_droit;
            insert_mort(parcours_liste_fils);
            parcours_liste_fils = frere_du_proc_supprime;
        }else{
            parcours_liste_fils = parcours_liste_fils->frere_droit;
        }
    }

    // et détruire en même temps les fils zombies
    // (comme c'est un parcours de liste, on peut affecter null à actif->pere et détruire tout les fils zombies)

    // retirer le processus courant (actif) de la liste des fils de son pere
    // (actif->pere->fils)

    struct Process *fils_precedent_parcouru = NULL;
    struct Process *fils_actuellement_parcouru = actif->pere->premier_fils;
    while (fils_actuellement_parcouru != NULL && actif->state != ZOMBIE){
        // si le fils du pere actuellement parcouru est l'actif
        if (fils_actuellement_parcouru == actif){

            // on supprime pas la tete de la liste
            if (fils_precedent_parcouru != NULL){
                if (fils_actuellement_parcouru != NULL){
                    // suppression en milieu de liste
                    //a->b->c : a->c
                    fils_precedent_parcouru->frere_droit = fils_actuellement_parcouru->frere_droit;
                }else{
                    // suppression en fin de liste (dernier élément)
                    // le frere droit du fils precedent parcouru devient NULL
                    fils_precedent_parcouru->frere_droit = NULL;

                    // c->d->(a supprimer) = c->d
                }
            }else{
                // dans ce cas, c'est la suppression de la tete de liste qui doit etre faite
                // parcours_fils_pere est censé toujours être égal à la tete de liste dans ce cas
                fils_actuellement_parcouru = fils_actuellement_parcouru->frere_droit;
                // (a supprimer)->b->c => b->c
            }
        }
        fils_precedent_parcouru = fils_actuellement_parcouru;
        fils_actuellement_parcouru = fils_actuellement_parcouru->frere_droit;
    }

    uint32_t old_pid = actif->pid;
    struct Process *new_actif = extract_activable();
    process_alive--;

    if (new_actif == NULL)
    {
        // passage sur idle
        //printf("%p    %p", process[old_pid].regs, process[0].regs);
        ctx_sw(process[old_pid].regs, process[0].regs);
    }
    else
    {
        actif = new_actif;
        //printf("%p    %p", process[old_pid].regs, process[actif->pid].regs);
        ctx_sw(process[old_pid].regs, process[actif->pid].regs);
    }
}

/*void kill_process() {
    struct Process *tampon;

    while(dead_list != NULL) {
        tampon = dead_list->suiv;
        process[dead_list->pid] = NULL;
        free(dead_list);
        dead_list = tampon;
    }
}*/

void display_queue(int normal)
{
    struct Process *tmpProcess;

    if (normal)
    {
        queue_for_each(tmpProcess, &head_queue, Process, my_link)
        {
            printf("Processus : %s ; PRIO : %d\n", tmpProcess->name, tmpProcess->prio);
        }
    }
    else
    {
        queue_for_each_prev(tmpProcess, &head_queue, Process, my_link)
        {
            printf("Processus : %s ; PRIO : %d\n", tmpProcess->name, tmpProcess->prio);
        }
    }
}
