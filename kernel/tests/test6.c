/*******************************************************************************
 * Test 6
 *
 * Waitpid multiple.
 * Creation de processus avec differentes tailles de piles.
 *******************************************************************************/

#include "kernel-sysapi.h"

int proc6_part1()
{
        __asm__(
                ".text\n"
                ".globl proc6_1\n"
                "proc6_1:\n"
                "movl $3,%eax\n"
                "ret\n"
                ".previous\n");
        return 1;
}

int proc6_part2()
{
        __asm__(
                ".text\n"
                ".globl proc6_2\n"
                "proc6_2:\n"
                "movl 4(%esp),%eax\n"
                "pushl %eax\n"
                "popl %eax\n"
                "ret\n"
                ".previous\n");
        return 1;
}

int proc6_part3()
{
        __asm__(
                ".text\n"
                ".globl proc6_3\n"
                "proc6_3:\n"
                "movl 4(%esp),%eax\n"
                "pushl %eax\n"
                "popl %eax\n"
                "ret\n"
                ".previous\n");
        return 1;
}

int test6(void *arg)
{
        int pid1, pid2, pid3;
        int ret;

        (void)arg;

        assert(getprio(getpid()) == 128);
        chprio(getprio(getpid()), 1);
        pid1 = start(proc6_part1, 0, 64, "proc6_1", 0);
        assert(pid1 > 0);
        pid2 = start(proc6_part2, 4, 66, "proc6_2", (void*)4);
        assert(pid2 > 0);
        pid3 = start(proc6_part3, 0xffffffff, 65, "proc6_3", (void*)5);
        assert(pid3 < 0);
        pid3 = start(proc6_part3, 8, 65, "proc6_3", (void*)5);
        assert(pid3 > 0);
        printf("waitpid_1 ");
        assert(waitpid(-1, &ret) == pid2); // bug dans la gestion du -1 sur waitpid (quand on fait : waitpid(pid2, &ret) == pid2, le code se lance et crash)
        printf("waitpid_2 ");
        assert(ret == 4);
        printf("waitpid_3 ");
        assert(waitpid(-1, &ret) == pid3);
        assert(ret == 5);
        printf("waitpid_4 ");
        assert(waitpid(-1, &ret) == pid1);
        assert(ret == 3);
        printf("waitpid_5 ");
        assert(waitpid(pid1, 0) < 0);
        assert(waitpid(-1, 0) < 0);
        assert(waitpid(getpid(), 0) < 0);
        printf("ok.\n");
        return 0;
}

