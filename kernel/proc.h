#ifndef __PROC_H__
#define __PROC_H__

#define STACK_SIZE_INTEGER 4096
#define PROCESS_TABLE_SIZE 30
#define MAX_PRIO 256

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "mem.h"
#include "cpu.h"
#include "clock.h"
#include "./../shared/queue.h"

// #include "message.h"

enum process_state
{
    ELU,
    ACTIVABLE,
    ENDORMI,
    MORT,
    ZOMBIE,
    SEMAPHORE
};

struct Process
{
    int pid;
    char name[15];
    enum process_state state;
    int regs[5];
    int exec_stack[STACK_SIZE_INTEGER];
    double time_to_wake;
    uint32_t prio;
    struct Process *suiv;
    struct Process *premier_fils;
    struct Process *dernier_fils;
    struct Process *pere;
    struct Process *frere_droit;
    int *retval; // valeur de retour du processus actuel
    int *retval_fils; // valeur transmise au père par un fils
    int pid_ret_fils;

    link my_link;
};

typedef struct Process Process;

struct Process *creer_liste_process();

struct Process *extract_activable(void);

struct Process *actif;

void insert_activable(struct Process *element);

//struct Process *extract_endormi(void) ;
void insert_endormi(struct Process *element, double time);

int idle(void *a);
int proc1(void *a);
int proc2(void *a);
int proc3(void *a);
void proc4(void);
void proc5(void);
void proc6(void);
void proc7(void);

void init_process(void);
void ordonnance(void);
int32_t getpid(void);
char *mon_nom(void);
int32_t cree_processus(void (*code)(void), char *nom);
uint32_t nbr_secondes();
void dors(uint32_t nbr_secs);
void wake_process();
void fin_processus();
void kill_process();

int kill(int pid);
int getprio(int pid);
bool is_pid_valid(int pid);
int chprio(int pid, int newprio);
int waitpid(int pid, int *retvalp);
void exit(int retval);
int pid_dun_fils_termine(int param);
void suppression_fils_zombie(struct Process* fils_a_supprimer);

void sleep_until(double time);
int start(int (*pt_func)(void *), unsigned long ssize, int prio, const char *name, void *arg);

void display_queue(int sensNormal);
struct Process process[PROCESS_TABLE_SIZE];
#endif
