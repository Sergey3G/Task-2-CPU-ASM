#include <stdio.h>

#include "processor_funcs.h"
#include "stack.h"

Errors processor_push(Processor* cpu)
{
    cpu->instruction_pointer++;
    printf("Now I will push %d to stack!\n", cpu->bytecode[cpu->instruction_pointer]);
    Errors err = push_stack(cpu->stack, cpu->bytecode[cpu->instruction_pointer]);
    stack_dump(cpu->stack);
    if (err != NO_ERRORS)
    {
        return err;
    }
    cpu->instruction_pointer++;
    return verify_processor(cpu);
}

Errors processor_add(Processor* cpu)
{
    Errors err = stack_binary_op(cpu->stack, ADD);
    stack_dump(cpu->stack);
    if (err != NO_ERRORS)
    {
        return err;
    }
    cpu->instruction_pointer++;
    return verify_processor(cpu);
}

Errors processor_sub(Processor* cpu)
{
    Errors err = stack_binary_op(cpu->stack, SUB);
    stack_dump(cpu->stack);
    if (err != NO_ERRORS)
    {
        return err;
    }
    cpu->instruction_pointer++;
    return verify_processor(cpu);
}

Errors processor_mul(Processor* cpu)
{
    Errors err = stack_binary_op(cpu->stack, MUL);
    stack_dump(cpu->stack);
    if (err != NO_ERRORS)
    {
        return err;
    }
    cpu->instruction_pointer++;
    return verify_processor(cpu);
}

Errors processor_div(Processor* cpu)
{
    Errors err = stack_div(cpu->stack);
    stack_dump(cpu->stack);
    if (err != NO_ERRORS)
    {
        return err;
    }
    cpu->instruction_pointer++;
    return verify_processor(cpu);
}

Errors processor_sqrt(Processor* cpu)
{
    Errors err = stack_sqrt(cpu->stack);
    stack_dump(cpu->stack);
    if (err != NO_ERRORS)
    {
        return err;
    }
    cpu->instruction_pointer++;
    return verify_processor(cpu);
}

Errors processor_out(Processor* cpu)
{
    Errors err = stack_out(cpu->stack);
    stack_dump(cpu->stack);
    if (err != NO_ERRORS)
    {
        return err;
    }
    cpu->instruction_pointer++;
    return verify_processor(cpu);
}

Errors processor_hlt(Processor* cpu)
{
    stack_dump(cpu->stack);
    printf("HLT instruction recieved. Halting processor execution!\n");
    return PROC_HALT;
}
