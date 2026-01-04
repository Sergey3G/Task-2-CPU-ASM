#include <stdio.h>

#include "../include/processor_funcs.h"
#include "../include/stack.h"
#include "../include/my_assert.h"

Errors processor_push(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");

    Errors err = NO_ERRORS;

    cpu->instruction_pointer++;
    err = push_stack(cpu->stack, cpu->bytecode[cpu->instruction_pointer]);
    if (err != NO_ERRORS) return err;
    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_add(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");
    Errors err = stack_binary_op(cpu->stack, ADD);
    if (err != NO_ERRORS) return err;
    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_sub(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");
    Errors err = stack_binary_op(cpu->stack, SUB);
    if (err != NO_ERRORS) return err;
    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_mul(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");
    Errors err = stack_binary_op(cpu->stack, MUL);
    if (err != NO_ERRORS) return err;
    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_div(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");
    Errors err = stack_div(cpu->stack);
    if (err != NO_ERRORS) return err;
    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_sqrt(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");
    Errors err = stack_sqrt(cpu->stack);
    if (err != NO_ERRORS) return err;
    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_out(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");
    Errors err = stack_out(cpu->stack);
    if (err != NO_ERRORS) return err;
    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_hlt(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");
    printf("HLT instruction recieved. Halting processor execution!\n");
    return PROC_HALT;
}

Errors processor_pushr(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");
    cpu->instruction_pointer++;
    data_t value = cpu->registers[cpu->bytecode[cpu->instruction_pointer]].value;
    Errors err = push_stack(cpu->stack, value);
    if (err != NO_ERRORS) return err;
    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_popr(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");
    cpu->instruction_pointer++;
    Errors err = NO_ERRORS;
    data_t value = pop_stack(cpu->stack, &err);
    cpu->registers[cpu->bytecode[cpu->instruction_pointer]].value = value;
    if (err != NO_ERRORS) return err;
    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_pushm(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");
    cpu->instruction_pointer++;
    data_t value = cpu->ram[cpu->bytecode[cpu->instruction_pointer]];
    Errors err = push_stack(cpu->stack, value);
    if (err != NO_ERRORS) return err;
    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_popm(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");
    cpu->instruction_pointer++;
    Errors err = NO_ERRORS;
    data_t value = pop_stack(cpu->stack, &err);
    cpu->ram[cpu->bytecode[cpu->instruction_pointer]] = value;
    if (err != NO_ERRORS) return err;
    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_jmp(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");
    cpu->instruction_pointer++;
    int target = cpu->bytecode[cpu->instruction_pointer];
    if (target < 1 || target >= cpu->bytecode[0] + 1) return PROC_INVALID_BYTECODE;
    cpu->instruction_pointer = target;
    return NO_ERRORS;
}

Errors processor_jb(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");

    Errors err = NO_ERRORS;
    cpu->instruction_pointer++;
    int target = cpu->bytecode[cpu->instruction_pointer];
    if (target < 1 || target > cpu->bytecode[0]) return PROC_INVALID_BYTECODE;

    data_t a = pop_stack(cpu->stack, &err);
    data_t b = pop_stack(cpu->stack, &err);
    if (err != NO_ERRORS) return err;

    if (b < a)
    {
        cpu->instruction_pointer = target;
        return NO_ERRORS;
    }

    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_jbe(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");

    Errors err = NO_ERRORS;
    cpu->instruction_pointer++;
    int target = cpu->bytecode[cpu->instruction_pointer];
    if (target < 1 || target > cpu->bytecode[0]) return PROC_INVALID_BYTECODE;

    data_t a = pop_stack(cpu->stack, &err);
    data_t b = pop_stack(cpu->stack, &err);
    if (err != NO_ERRORS) return err;

    if (b <= a)
    {
        cpu->instruction_pointer = target;
        return NO_ERRORS;
    }

    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_je(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");

    Errors err = NO_ERRORS;
    cpu->instruction_pointer++;
    int target = cpu->bytecode[cpu->instruction_pointer];
    if (target < 1 || target > cpu->bytecode[0]) return PROC_INVALID_BYTECODE;

    data_t a = pop_stack(cpu->stack, &err);
    data_t b = pop_stack(cpu->stack, &err);
    if (err != NO_ERRORS) return err;

    if (b == a)
    {
        cpu->instruction_pointer = target;
        return NO_ERRORS;
    }

    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_ja(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");

    Errors err = NO_ERRORS;
    cpu->instruction_pointer++;
    int target = cpu->bytecode[cpu->instruction_pointer];
    if (target < 1 || target > cpu->bytecode[0]) return PROC_INVALID_BYTECODE;

    data_t a = pop_stack(cpu->stack, &err);
    data_t b = pop_stack(cpu->stack, &err);
    if (err != NO_ERRORS) return err;

    if (b > a)
    {
        cpu->instruction_pointer = target;
        return NO_ERRORS;
    }

    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_jae(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");

    Errors err = NO_ERRORS;
    cpu->instruction_pointer++;
    int target = cpu->bytecode[cpu->instruction_pointer];
    if (target < 1 || target > cpu->bytecode[0]) return PROC_INVALID_BYTECODE;

    data_t a = pop_stack(cpu->stack, &err);
    data_t b = pop_stack(cpu->stack, &err);
    if (err != NO_ERRORS) return err;

    if (b >= a)
    {
        cpu->instruction_pointer = target;
        return NO_ERRORS;
    }

    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_jne(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");

    Errors err = NO_ERRORS;
    cpu->instruction_pointer++;
    int target = cpu->bytecode[cpu->instruction_pointer];
    if (target < 1 || target > cpu->bytecode[0]) return PROC_INVALID_BYTECODE;

    data_t a = pop_stack(cpu->stack, &err);
    data_t b = pop_stack(cpu->stack, &err);
    if (err != NO_ERRORS) return err;

    if (b != a)
    {
        cpu->instruction_pointer = target;
        return NO_ERRORS;
    }

    cpu->instruction_pointer++;
    return NO_ERRORS;
}

Errors processor_call(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");
    cpu->instruction_pointer++;
    Errors err = NO_ERRORS;
    int target = cpu->bytecode[cpu->instruction_pointer];
    if (target < 1 || target > cpu->bytecode[0]) return PROC_INVALID_BYTECODE;
    int return_ip = cpu->instruction_pointer + 1;

    err = push_stack(cpu->return_stack, return_ip);
    if (err != NO_ERRORS) return err;

    cpu->instruction_pointer = target;
    return NO_ERRORS;
}

Errors processor_ret(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->stack, "Error: cpu->stack is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");
    Errors err = NO_ERRORS;
    int return_ip = pop_stack(cpu->return_stack, &err);
    if (err != NO_ERRORS) return err;
    cpu->instruction_pointer = return_ip;
    return NO_ERRORS;
}


