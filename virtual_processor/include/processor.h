#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "stack.h"
#include "../../instructions.h"
#include "stack_funcs.h"
#include "../../errors.h"

#define max(a, b) ((a) > (b) ? (a) : (b))

typedef enum Registers
{
    AX = 0,
    BX = 1,
    CX = 2,
    DX = 3
} Registers;

typedef struct Register
{
    Registers name;
    data_t value;
} Register;

typedef struct Processor
{
    Stack* stack;
    int* bytecode;
    size_t instruction_pointer;
    Stack* return_stack;
    Register registers[4];
    data_t ram[14400];
} Processor;

typedef Errors (*CommandFunctionPtr)(Processor*);

typedef struct Command
{
    Instructions code;
    CommandFunctionPtr function;
} Command;

int* read_bytecode(const char* filename);
void print_bytecode(int* bytecode, size_t size);
Errors construct_processor(Processor* processor, const char* filename);
void destruct_processor(Processor* processor);
Errors verify_processor(const Processor* processor);
void processor_dump(const Processor* processor);
Errors execute_bytecode(Processor* cpu);

#endif
