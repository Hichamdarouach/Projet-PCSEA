#ifndef _KERNEL_SYSAPI_H_
#define _KERNEL_SYSAPI_H_
#include "debug.h"
#include "it.h"

// Prototype des appels systeme de la spec
int chprio(int pid, int newprio);
void cons_write(const char *str, unsigned long size);

void cons_echo(int on);
void exit(int retval);
int getpid(void);
int getprio(int pid);
int kill(int pid);

int start(int (*pt_func)(void *), unsigned long ssize, int prio, const char *name, void *arg);
int waitpid(int pid, int *retvalp);

/* Available from our standard library */
#ifndef __SIZE_TYPE__
#error __SIZE_TYPE__ not defined
#endif

typedef __SIZE_TYPE__ size_t;

int strcmp(const char *str1, const char *str2);
char *strncpy(char *dst, const char *src, unsigned n);
void *memset(void *dst, int c, size_t n);

#endif /* _KERNEL_SYSAPI_H_ */