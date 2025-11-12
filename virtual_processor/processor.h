#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "stack.h"
#include "../instructions.h"
#include "ariphmetics.h"
#include "../errors.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

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
    int state; // state = 0; => register is empty, state = 1; => register is filled
} Register;

typedef struct Processor
{
    Stack* stack;
    int* bytecode;
    size_t instruction_pointer;
    int* return_stack;
    Register registers[4];
    data_t ram[14400];
} Processor;

typedef struct Command
{
    Instructions code;
    Errors (*function_ptr)(Processor*);
} Command;

int* read_bytecode(char* filename);
void print_bytecode(int* bytecode, size_t size);
void construct_processor(Processor* processor, Stack* stack, int* bytecode);
void destruct_processor(Processor* processor);
Errors verify_processor(const Processor* processor);
void processor_dump(const Processor* processor);
Errors execute_bytecode(Processor* cpu);

#endif
