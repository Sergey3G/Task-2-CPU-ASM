#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

#include "../../instructions.h"
#include "my_assert.h"
#include "labels.h"

typedef struct AsmContext
{
    int* bytecode;
    int* ptr;
    size_t pc;
    LabelTable* labels;
    int pass;
} AsmContext;

typedef struct Command Command;

typedef void (*EmitFunc)(AsmContext* ctx, const Command* cmd, const char* line);

typedef struct Command
{
    const char* name;
    Instructions code;
    EmitFunc emit;
} Command;

char* input_to_buffer(const char* input_filename);
size_t count_of_strings(const char* string);
char** create_str_array(char* buffer, size_t str_count);
int my_atoi(const char* str);
const Command* find_cmd(const char* name);
int* compile_to_bytecode(char** str_array, size_t count);
void print_bytecode(int* bytecode, char* instruction, size_t count, size_t i);
int parse_register(const char* str);

#endif
