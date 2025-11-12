#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "processor.h"
#include "processor_funcs.h"

Command commands[] = {{PUSH, &processor_push},
                      {ADD, &processor_add},
                      {SUB, &processor_sub},
                      {MUL, &processor_mul},
                      {DIV, &processor_div},
                      {SQRT, &processor_sqrt},
                      {OUT, &processor_out}};

int* read_bytecode(char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (!file)
    {
        printf("Error: cannot open file %s!\n", filename);
        return NULL;
    }

    size_t size = 0;
    fread(&size, sizeof(int), 1, file);

    int* bytecode = (int*)calloc(size + 1, sizeof(int));
    if (!bytecode)
    {
        printf("Error: memory allocation failed!");
        return NULL;
    }

    bytecode[0] = size;

    size_t read_size = fread(bytecode + 1, sizeof(int), size, file);
    if (read_size != size)
    {
        printf("Error: cannot read file content correctly!");
    }

    fclose(file);
    return bytecode;
}

void print_bytecode(int* bytecode, size_t size)
{
    if (!bytecode)
    {
        printf("Error: bytecode is null!\n");
        return;
    }

    printf("\n========= BYTECODE =========\n");
    for (size_t i = 0; i < size; i++)
    {
        printf("%d\n", bytecode[i]);
    }
    printf("============================\n");
}

void construct_processor(Processor* processor, Stack* stack, int* bytecode)
{
    if (!processor)
    {
        printf("Error: processor is null!");
        return;
    }

    if (!stack)
    {
        printf("Error: stack is null!");
        return;
    }

    if (!bytecode)
    {
        printf("Error: bytecode is null!");
        return;
    }

    processor->stack = stack;
    processor->bytecode = bytecode;
    processor->instruction_pointer = 0;
    processor->registers[AX] = {AX, 0, 0};
    processor->registers[BX] = {BX, 0, 0};
    processor->registers[CX] = {CX, 0, 0};
    processor->registers[DX] = {DX, 0, 0};
    processor->ram[14400];
    memset(processor->ram, 0, sizeof(processor->ram));
}

void destruct_processor(Processor* processor)
{
    if (processor->stack)
    {
        destruct_stack(processor->stack);
        processor->stack = NULL;
    }
    if (processor->bytecode)
    {
        free(processor->bytecode);
        processor->bytecode = NULL;
    }
    processor->instruction_pointer = 0;
}

Errors verify_processor(const Processor* processor)
{
    Errors err = NO_ERRORS;

    if (!processor)
    {
        err = (Errors)(err | PROC_NULL_PTR);
    }

    if (!processor->stack)
    {
        err = (Errors)(err | PROC_INVALID_STACK);
    }

    if (!processor->bytecode)
    {
        err = (Errors)(err | PROC_INVALID_BYTECODE);
    }

    if (processor->stack && verify_stack(processor->stack) != NO_ERRORS)
    {
        err = (Errors)(err | verify_stack(processor->stack));
    }

    return err;
}

void processor_dump(const Processor* processor)
{
    printf("\n================ PROCESSOR DUMP ================\n");

    if (!processor)
    {
        printf("Aborted: processor is null!\n");
        printf("\n================================================\n");
        return;
    }

    Errors err = verify_processor(processor);

    printf("Processor address: %p\n", (void*)processor);
    printf("Stack address: %p\n", (void*)processor->stack);
    printf("Bytecode address: %p\n", (void*)processor->bytecode);
    printf("Instruction pointer: %ld\n", processor->instruction_pointer);
    printf("Error flags: %d (", err);

    if (err != NO_ERRORS)
    {
        printf("Processor corrupted! Attempting stack dump...\n");
        if (processor->stack)
        {
            stack_dump(processor->stack);
        }
        printf("================================================\n\n");
        return;
    }

    printf("\n--- Registers ---\n");
    for (int i = 0; i < 4; i++)
    {
        printf("%s = %d (state = %d)\n",
               (processor->registers[i].name == AX) ? "AX" :
               (processor->registers[i].name == BX) ? "BX" :
               (processor->registers[i].name == CX) ? "CX" : "DX",
                processor->registers[i].value,
                processor->registers[i].state);
    }

    printf("\n--- Stack state ---\n");
    stack_dump(processor->stack);

    printf("\n--- Bytecode preview ---\n");
    for (size_t i = 0; i < min(processor->bytecode[0] + 1, 10) && processor->bytecode; i++)
    {
        printf("[%02d] = %d\n", i, processor->bytecode[i]);
    }
}

Errors execute_bytecode(Processor* cpu)
{
    Errors err = verify_processor(cpu);

    if (err != NO_ERRORS)
    {
        return err;
    }

    while (cpu->instruction_pointer < cpu->bytecode[0] + 1)
    {
        switch (cpu->bytecode[cpu->instruction_pointer])
        {
            case PUSH:
                cpu->instruction_pointer++;
                err = stack_push(cpu->stack, cpu->bytecode[cpu->instruction_pointer]);
                if (err != NO_ERRORS)
                {
                    return err;
                }
                cpu->instruction_pointer++;
            case ADD:
                err = stack_binary_op(cpu->stack, ADD);
                if (err != NO_ERRORS)
                {
                    return err;
                }
            case SUB:
                err = stack_binary_op(cpu->stack, SUB);
                if (err != NO_ERRORS)
                {
                    return err;
                }
            case MUL:
                err = stack_binary_op(cpu->stack, MUL);
                if (err != NO_ERRORS)
                {
                    return err;
                }
            case DIV:
                err = stack_div(cpu->stack);
                if (err != NO_ERRORS)
                {
                    return err;
                }
            // i stopped here
        }
    }
}


