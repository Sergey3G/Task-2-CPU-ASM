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
Errors processor_pushr(Processor* cpu);
Errors processor_popr(Processor* cpu);
Errors processor_pushm(Processor* cpu);
Errors processor_popm(Processor* cpu);
Errors processor_jmp(Processor* cpu);
Errors processor_jb(Processor* cpu);
Errors processor_jbe(Processor* cpu);
Errors processor_je(Processor* cpu);
Errors processor_ja(Processor* cpu);
Errors processor_jae(Processor* cpu);
Errors processor_jne(Processor* cpu);
Errors processor_call(Processor* cpu);
Errors processor_ret(Processor* cpu);

#endif
