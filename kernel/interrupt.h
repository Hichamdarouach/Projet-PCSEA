#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__
#include <stdbool.h>
#include "stdint.h"

void open_IRQ(uint32_t num_IRQ, bool masque);
void init_traitant_IT(int32_t num_IT, void (*traitant)(void));

#endif
