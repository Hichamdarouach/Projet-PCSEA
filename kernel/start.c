#include "debugger.h"
#include "cpu.h"
#include "display.h"
#include "stdio.h"
#include "proc.h"
#include "clock.h"
#include "keyboard_int.h"
#include "message.h"
#include "tests.h"
#include "kernel_it_49.h"
#include "start.h"
#include "../user/lib/sys_calls.h"


/*int waitpid(){
    __asm__ __volatile__ ("mov %%eax, %0"::"a"(35));
	__asm__ __volatile__ ("pushl %eax");
    __asm__ __volatile__ ("mov %%ebx, %0"::"a"(56));
    __asm__ __volatile__ ("mov %%ecx, %0"::"a"(73));

    // appel système (user vers kernel)
    __asm__ __volatile__ ("int $49");


	//__asm__ __volatile__ ("mov %eax, 35; mov %ebx, 56; mov %ecx, 73; int $49");
	// appel a un fichier assembleur
    while(1);
}*/

void kernel_start(void)
{

	init_clock();
	init_keyboard();
	init_it_superuser();

	efface_ecran();

	/*int valeur_passee_a_waitpid_2 = 904;
	int valeur_passee_a_waitpid = 31;
	int valeur_retour_waitpid = 49;

	int valeur_passee_a_kill = 108;

	printf("%p %p %p %p", &valeur_passee_a_waitpid_2, &valeur_passee_a_waitpid, &valeur_retour_waitpid, &valeur_passee_a_kill);

	waitpide(valeur_passee_a_waitpid_2, &valeur_retour_waitpid);

	printf("\nvaleur retour waitpid : %d\n", valeur_retour_waitpid);*/
	initialisation_fifo_tab();
	assert(pcreate(2) == 0);

	//while(1);
	
	init_process();
	printf("\nTest 0 :\n");
	test0();
	//defilement();
	        printf("\nTest 1 :\n");
	test1();
	//defilement();
	        printf("\nTest 2 :\n");
	test2();
	//defilement();        
	printf("\nTest 3 :\n");
	test3();
	//defilement();
	printf("\nTest 4 :\n");
	test4();
	//defilement();
	//test5();
	//defilement();
	test6();

	void *a = (void *)1;
	idle(a);

	while (1)
		hlt();

	return;
}
