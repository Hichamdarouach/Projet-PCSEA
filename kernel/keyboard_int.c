#include "cpu.h"
#include "stdio.h"

#include "keyboard_int.h"
#include "interrupt.h"

extern void traitant_IT_33(void);

void init_keyboard()
{
    // Open the IRQ1
    open_IRQ(1, false);
    // Attach function to the interrupt
    init_traitant_IT(33, &traitant_IT_33);
}

void tic_PIT_KEYBOARD(void){
    
    outb(0x20, 0x20);
    char val = inb(0x60);
    printf("%c", val);

}