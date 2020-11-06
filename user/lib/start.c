#include "start.h"

void user_start(void){
    while(1);
    __asm__ __volatile__ ("int $49");
}