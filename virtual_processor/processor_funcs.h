#ifndef PROC_FUNCS_H
#define PROC_FUNCS_H

#include "stack_funcs.h"
#include "processor.h"

Errors processor_push(Processor* cpu);
Errors processor_add(Processor* cpu);
Errors processor_sub(Processor* cpu);
Errors processor_mul(Processor* cpu);
Errors processor_div(Processor* cpu);
Errors processor_sqrt(Processor* cpu);
Errors processor_out(Processor* cpu);
Errors processor_hlt(Processor* cpu);

#endif
