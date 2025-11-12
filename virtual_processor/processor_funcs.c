#include <stdio.h>

#include "processor_funcs.h"
#include "ariphmetics.h"

Errors processor_push(Processor* cpu)
{
    cpu->instruction_pointer++;
    Errors err = stack_push(cpu->stack, cpu->bytecode[cpu->instruction_pointer]);
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
    if (err != NO_ERRORS)
    {
        return err;
    }
    cpu->instruction_pointer++;
    return verify_processor(cpu);
}
