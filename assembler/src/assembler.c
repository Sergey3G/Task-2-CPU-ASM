#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#include "../include/assembler.h"
#include "../include/labels.h"

int parse_register(const char* str)
{
    if (!strcmp(str, "AX")) return 0;
    if (!strcmp(str, "BX")) return 1;
    if (!strcmp(str, "CX")) return 2;
    if (!strcmp(str, "DX")) return 3;
    return -1;
}

static void emit_simple(AsmContext* ctx, const Command* cmd, const char* line)
{
    if (ctx->pass == 2)
    {
        *ctx->ptr++ = cmd->code;
        (*ctx->bytecode)++;
    }

    ctx->pc++;
}

static void emit_imm(AsmContext* ctx, const Command* cmd, const char* line)
{
    int value = 0;
    sscanf(line + strlen(cmd->name), "%d", &value);

    if (ctx->pass == 2)
    {
        *ctx->ptr++ = cmd->code;
        *ctx->ptr++ = value;
        (*ctx->bytecode) += 2;
    }

    ctx->pc += 2;
}

static void emit_reg(AsmContext* ctx, const Command* cmd, const char* line)
{
    char buffer[8] = {0};
    sscanf(line + strlen(cmd->name), "%7s", buffer);

    int reg = parse_register(buffer);
    if (reg < 0)
    {
        printf("Unknown register: %s\n", buffer);
        return;
    }

    if (ctx->pass == 2)
    {
        *ctx->ptr++ = cmd->code;
        *ctx->ptr++ = reg;
        (*ctx->bytecode) += 2;
    }

    ctx->pc += 2;
}

static void emit_jump(AsmContext* ctx, const Command* cmd, const char* line)
{
    char buffer[32] = {0};
    sscanf(line + strlen(cmd->name), "%31s", buffer);

    if (buffer[0] != ':')
    {
        printf("Expected label, got: %s\n", buffer);
        return;
    }

    const char* label_name = buffer + 1;

    if (ctx->pass == 2)
    {
        const Label* label = label_find(ctx->labels, label_name);
        if (!label)
        {
            printf("Undefined label: %s\n", label_name);
            return;
        }

        *ctx->ptr++ = cmd->code;
        *ctx->ptr++ = (int)label->address;
        (*ctx->bytecode) += 2;
    }

    ctx->pc += 2;
}

static int try_parse_label(AsmContext* ctx, const char* line)
{
    if (line[0] != ':')
        return 0;

    labels_add(ctx->labels, line + 1, ctx->pc);
    return 1;
}

static Command commands[] = {{"HLT", HLT, emit_simple},
                             {"PUSH", PUSH, emit_imm},
                             {"ADD", ADD, emit_simple},
                             {"SUB", SUB, emit_simple},
                             {"MUL", MUL, emit_simple},
                             {"DIV", DIV, emit_simple},
                             {"SQRT", SQRT, emit_simple},
                             {"OUT", OUT, emit_simple},
                             {"PUSHR", PUSHR, emit_reg},
                             {"POPR", POPR, emit_reg},
                             {"PUSHM", PUSHM, emit_imm},
                             {"POPM", POPM, emit_imm},
                             {"JMP", JMP, emit_jump},
                             {"JB", JB, emit_jump},
                             {"JBE", JBE, emit_jump},
                             {"JE", JE, emit_jump},
                             {"JA", JA, emit_jump},
                             {"JAE", JAE, emit_jump},
                             {"JNE", JNE, emit_jump},
                             {"CALL", CALL, emit_jump},
                             {"RET", RET, emit_simple}};

char* input_to_buffer(const char* input_filename)
{
    struct stat st;
    MY_ASSERT(!(stat(input_filename, &st)), "Error: stat failed!\n");

    size_t file_size = (size_t)st.st_size;
    FILE* input_file = fopen(input_filename, "r");
    MY_ASSERT(input_file, "Error: cannot open file!\n");

    char* buffer = (char*)calloc(file_size + 1, sizeof(char));
    MY_ASSERT(buffer, "Error: memory allocation failed!\n");

    size_t read_size = fread(buffer, sizeof(char), file_size, input_file);
    fclose(input_file);
    MY_ASSERT((read_size == file_size), "Error: cannot read file content!\n");

    buffer[file_size] = '\0';
    return buffer;
}

size_t count_of_strings(const char* string)
{
    if (!string)
    MY_ASSERT(string, "Error: string is nullptr!\n");

    size_t count = 0;
    const char* check = string;

    while ((check = strchr(check, '\n')) != NULL)
    {
        count++;
        check++;
    }

    return count + 1;
}

char** create_str_array(char* buffer, size_t str_count)
{
    char** array = (char**)calloc(str_count, sizeof(char*));
    MY_ASSERT(array, "Error: memory allocation failed!\n")

    char** current = array;
    *current++ = buffer;
    char* check = strchr(buffer, '\n');

    while (check != NULL && current < array + str_count)
    {
        *check = '\0';
        *current = check + 1;
        current++;
        check = strchr(check + 1, '\n');
    }

    return array;
}

int my_atoi(const char* str)
{
	int result = 0, sign = 1;

    while (*str == ' ' || *str == '\t')
    {
        str++;
    }

	if (*str == '+')
	{
		str++;
	}
	else if (*str == '-')
	{
		sign = -1;
		str++;
	}

	while (*str >= '0' && *str <= '9')
	{
		result = (result * 10) + (*str - '0');
		str++;
	}

	return sign * result;
}

const Command* find_cmd(const char* name)
{
    for (size_t i = 0; i < sizeof(commands) / sizeof(Command); i++)
    {
        if (!strcmp(commands[i].name, name))
            return &commands[i];
    }
    return NULL;
}

int* compile_to_bytecode(char** str_array, size_t count)
{
    MY_ASSERT(str_array, "Error: str_array is nullptr!\n");

    int* bytecode = (int*)calloc(count * 2 + 1, sizeof(int));
    MY_ASSERT(bytecode, "Error: memory allocation failed!\n");

    LabelTable labels = {0};
    labels_init(&labels);

    AsmContext ctx = {.bytecode = bytecode, .labels = &labels};

    for (ctx.pass = 1; ctx.pass <= 2; ctx.pass++)
    {
        ctx.pc = 1;
        ctx.ptr = bytecode + 1;

        for (size_t i = 0; i < count; i++)
        {
            const char* line = str_array[i];

            if (*line == '\0') continue;

            if (try_parse_label(&ctx, line)) continue;

            char buffer[32] = {0};
            sscanf(line, "%31s", buffer);

            const Command* cmd = find_cmd(buffer);
            if (!cmd)
            {
                printf("Unknown instruction: %s\n", buffer);
                continue;
            }

            cmd->emit(&ctx, cmd, line);
        }
    }

    labels_free(&labels);
    return bytecode;
}

void print_bytecode(int* bytecode, char* instruction, size_t count, size_t i)
{
    printf("Read instruction number %ld: %s\n", i + 1, instruction);
    printf("-----------------------------\n");
    for (size_t j = 0; j < 2 * count; j++)
    {
        printf("bytecode[%ld] = %d\n", j, bytecode[j]);
    }
    printf("-----------------------------\n");
    return;
}


