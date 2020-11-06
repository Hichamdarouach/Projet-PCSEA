#include "cpu.h"
#include "stdio.h"
#include "proc.h"
#include "kernel_it_49.h"
#include "interrupt.h"

extern void traitant_IT_49(void);
int syscall_number = 0;
int param1 = 0;
int *param2 = (void*)0;

void init_it_superuser()
{
    // Attach function to the interrupt
    init_traitant_IT(49, &traitant_IT_49);
}

void tic_PIT_SUPERUSER(void){

   /* __asm__ __volatile__ ("\t movl %%eax,%0" : "=r"(syscall_number));
    __asm__ __volatile__ ("\t movl %%ecx,%0" : "=r"(param1));
    __asm__ __volatile__ ("\t movl %%edx,%0" : "=r"(param2));*/

    __asm__ (
        "\t mov %%ecx, %0\n"
        "\t movl %%edx, %1\n"
        "\t movl %%esi, %2\n"
        : "=r"(syscall_number), "=r"(param1), "=r"(param2) // variables en sortie
        :  // entrées
        : 
    );

    *param2 = 500;
    waitpid(param1, param2);

}