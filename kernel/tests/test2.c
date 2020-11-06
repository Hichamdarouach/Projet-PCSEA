#include "kernel-sysapi.h"

int procKill(void *args)
{
        printf(" X");
        return (int)args;
}

int procExit(void *args)
{
        printf(" 5");
        exit((int)args);
        assert(0);
        return 0;
}

/*void multiple_test_it(){
        for (int i = 0; i < 1000; ++i)
                test_it();
}*/

// int test2(void *arg)
int test2()
{
        int rval;
        int r;
        int pid1;
        int val = 45;
        //char i = 0;

        // (void)arg;

        printf("1");
        pid1 = start(procKill, 4000, 100, "procKill", (void *)val);
        test_it();
        assert(pid1 > 0);
        printf(" 2");
        r = kill(pid1);
        assert(r == 0);
        printf(" 3");
        r = waitpid(pid1, &rval);
        assert(rval == 0);
        assert(r == pid1);
        printf(" 4");
        pid1 = start(procExit, 4000, 192, "procExit", (void *)val);
        assert(pid1 > 0);

        test_it();

        printf(" 6");
        r = waitpid(pid1, &rval);
        assert(rval == val);
        assert(r == pid1);
        assert(waitpid(getpid(), &rval) < 0);
        printf(" 7.\n");
        return 0;
}

