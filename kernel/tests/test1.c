/*******************************************************************************
 * Test 1
 *
 * Demarrage de processus avec passage de parametre
 * Terminaison normale avec valeur de retour
 * Attente de terminaison (cas fils avant pere et cas pere avant fils)
 ******************************************************************************/

#include "kernel-sysapi.h"

#define DUMMY_VAL 78

int dummy1(void *arg)
{
        printf("1");
        assert((int)arg == DUMMY_VAL);
        exit(3);
        printf("ERREUR");
        return 3;
}

int dummy2(void *arg)
{
        printf(" 5");
        assert((int)arg == DUMMY_VAL + 1);
        exit(4);
        return 4;
}

// int test1(void *arg)
int test1()
{
        int pid1;
        int r;
        int rval;

        // (void)arg;

        pid1 = start(dummy1, 4000, 192, "dummy1", (void *) DUMMY_VAL);
        assert(pid1 > 0);

        //permet d'avoir une interruption sur dummy1
        test_it();

        printf(" 2");
        r = waitpid(pid1, &rval);

        // permet d'avoir une interruption sur waitpid
        //test_it();

        assert(r == pid1);
        assert(rval == 3);
        printf(" 3");
        pid1 = start(dummy2, 4000, 100, "dummy2", (void *) (DUMMY_VAL + 1));

        // permet d'avoir une interruption sur dummy2

        assert(pid1 > 0);
        test_it();

        printf(" 4");
        r = waitpid(pid1, &rval);

        assert(r == pid1);
        assert(rval == 4);
        printf(" 6.\n");
        return 0;
}
