#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/processor.h"
#include "../include/processor_funcs.h"
#include "../include/my_assert.h"

static Command commands[] = {{PUSH, &processor_push},
                      {ADD, &processor_add},
                      {SUB, &processor_sub},
                      {MUL, &processor_mul},
                      {DIV, &processor_div},
                      {SQRT, &processor_sqrt},
                      {OUT, &processor_out},
                      {HLT, &processor_hlt},
                      {PUSHR, &processor_pushr},
                      {POPR, &processor_popr},
                      {PUSHM, &processor_pushm},
                      {POPM, &processor_popm},
                      {JMP, &processor_jmp},
                      {JB, &processor_jb},
                      {JBE, &processor_jbe},
                      {JE, &processor_jbe},
                      {JA, &processor_ja},
                      {JAE, &processor_jae},
                      {JNE, &processor_jne},
                      {CALL, &processor_call},
                      {RET, &processor_ret}};

size_t commands_count = sizeof(commands) / sizeof(Command);

int* read_bytecode(const char* filename)
{
    FILE* file = fopen(filename, "rb");
    MY_ASSERT(file, "Error: cannot open file!\n");

    size_t size = 0;
    fread(&size, sizeof(int), 1, file);

    int* bytecode = (int*)calloc(size + 1, sizeof(int));
    MY_ASSERT(bytecode, "Error: memory allocation failed!\n");

    bytecode[0] = size;

    size_t read_size = fread(bytecode + 1, sizeof(int), size, file);
    MY_ASSERT((read_size == size), "Error: cannot read file content!\n");

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

Errors construct_processor(Processor* processor, const char* filename)
{
    MY_ASSERT(processor, "Error: processor is nullptr!\n");
    MY_ASSERT(filename, "Error: filename is nullptr!\n");

    processor->stack = (Stack*)calloc(1, sizeof(Stack));
    MY_ASSERT(processor->stack, "Error: memory allocation failed!\n");
    Errors err = construct_stack(processor->stack, 10);
    MY_ASSERT((err == NO_ERRORS), "Error: construct_stack failed!\n");

    processor->return_stack = (Stack*)calloc(1, sizeof(Stack));
    MY_ASSERT(processor->return_stack, "Error: memory allocation failed!\n");
    err = construct_stack(processor->return_stack, 10);
    MY_ASSERT((err == NO_ERRORS), "Error: construct_stack failed!\n");

    processor->bytecode = read_bytecode(filename);
    MY_ASSERT(processor->bytecode, "Error: read_bytecode failed!\n");

    processor->instruction_pointer = 0;

    processor->registers[AX] = (Register){AX, 0};
    processor->registers[BX] = (Register){BX, 0};
    processor->registers[CX] = (Register){CX, 0};
    processor->registers[DX] = (Register){DX, 0};

    memset(processor->ram, 0, sizeof(processor->ram));

    return NO_ERRORS;
}

void destruct_processor(Processor* processor)
{
    if (processor->stack)
    {
        destruct_stack(processor->stack);
        free(processor->stack);
        processor->stack = NULL;
    }
    if (processor->return_stack)
    {
        destruct_stack(processor->return_stack);
        free(processor->return_stack);
        processor->return_stack = NULL;
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
        printf("%s = %d \n",
               (processor->registers[i].name == AX) ? "AX" :
               (processor->registers[i].name == BX) ? "BX" :
               (processor->registers[i].name == CX) ? "CX" : "DX",
                processor->registers[i].value);
    }

    printf("\n--- Stack state ---\n");
    stack_dump(processor->stack);

    printf("\n--- Bytecode preview ---\n");
    for (size_t i = 0; i < max(processor->bytecode[0] + 1, 10) && processor->bytecode; i++)
    {
        printf("[%02ld] = %d\n", i, processor->bytecode[i]);
    }
}

Errors execute_bytecode(Processor* cpu)
{
    MY_ASSERT(cpu, "Error: cpu is nullptr!\n");
    MY_ASSERT(cpu->bytecode, "Error: cpu->bytecode is nullptr!\n");

    Errors err = verify_processor(cpu);

    if (err != NO_ERRORS)
    {
        return err;
    }

    cpu->instruction_pointer = 1;

    while (cpu->instruction_pointer < cpu->bytecode[0] + 1)
    {
        int current_command_code = cpu->bytecode[cpu->instruction_pointer];
        int command_found = 0;
        for (size_t i = 0; i < commands_count; i++)
        {
            if (cpu->bytecode[cpu->instruction_pointer] == commands[i].code)
            {
                err = commands[i].function(cpu);
                if (err == PROC_HALT)
                {
                    printf("Execution finished by HLT!\n");
                    return NO_ERRORS;
                }

                if (err != NO_ERRORS)
                {
                    processor_dump(cpu);
                    return err;
                }
                command_found = 1;
                break;
            }
        }
        if (!command_found)
        {
            printf("Error: no such command code!\n");
            err = (Errors)(err | PROC_INVALID_BYTECODE);
            processor_dump(cpu);
            return err;
        }
    }

    printf("Execution completed successfully!\n");
    err = verify_processor(cpu);
    return err;
}
