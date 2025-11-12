#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "../instructions.h"

Command commands[] = {{"HLT", HLT, 0, 0},
                      {"PUSH", PUSH, 1, 0},
                      {"ADD", ADD, 0, 0},
                      {"SUB", SUB, 0, 0},
                      {"MUL", MUL, 0, 0},
                      {"DIV", DIV, 0, 0},
                      {"SQRT", SQRT, 0, 0},
                      {"OUT", OUT, 0, 0},
                      {"PUSHR", PUSHR, 1, 0},
                      {"POPR", POPR, 1, 0},
                      {"PUSHM", PUSHM, 1, 0},
                      {"POPM", POPM, 1, 0},
                      {"JMP", JMP, 1, 1},
                      {"JB", JB, 1, 1},
                      {"JBE", JBE, 1, 1},
                      {"JE", JE, 1, 1},
                      {"JA", JA, 1, 1},
                      {"JAE", JAE, 1, 1},
                      {"JNE", JNE, 1, 1},
                      {"CALL", CALL, 1, 1},
                      {"RET", RET, 1, 1}};

char* input_to_buffer(const char* input_filename)
{
    struct stat st;
    if (stat(input_filename, &st) != 0)
    {
        perror("stat failed!");
        return NULL;
    }

    size_t file_size = (size_t)st.st_size;
    FILE* input_file = fopen(input_filename, "r");
    if (!input_file)
    {
        printf("Error: cannot open file: %s!\n", input_filename);
        return NULL;
    }

    char* buffer = (char*)calloc(file_size + 1, sizeof(char));
    if (!buffer)
    {
        printf("Error: memory allocation failed!");
        fclose(input_file);
        return NULL;
    }

    size_t read_size = fread(buffer, sizeof(char), file_size, input_file);
    fclose(input_file);
    if (read_size != file_size)
    {
        printf("Error: cannot read file content!\n");
        free(buffer);
        return NULL;
    }
    buffer[file_size] = '\0';
    return buffer;
}

size_t count_of_strings(const char* string)
{
    if (!string)
    {
        printf("Error: string is nullptr!");
        return 0;
    }
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

    if (!array)
    {
        printf("Error: memory allocation failed!");
        return NULL;
    }

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



int* compile_to_bytecode(char** str_array, size_t count)
{
    int* bytecode = (int*)calloc(count * 2 + 1, sizeof(int));
    if (!bytecode)
    {
        printf("Error: memory allocation failed!");
        return NULL;
    }

    int* changing_bytecode = bytecode;

    *changing_bytecode = 0;
    changing_bytecode++;

    // Command
        // for (n_command)
            // if (strcmp(data, Commands[i].name) == 0)
    for (size_t i = 0; i < count; i++)
    {
        if (strncmp(str_array[i], "PUSH", 4) == 0)
        {
            char* start_instruction = str_array[i];
            *changing_bytecode = PUSH;
            start_instruction += 4;
            changing_bytecode++;

            *changing_bytecode = my_atoi(start_instruction);
            changing_bytecode++;
            print_bytecode(bytecode, str_array[i], count, i);
            *bytecode += 2;
        }
        else if (strcmp(str_array[i], "ADD") == 0)
        {
            *changing_bytecode = ADD;
            changing_bytecode++;
            print_bytecode(bytecode, str_array[i], count, i);
            *bytecode += 1;
        }
        else if (strcmp(str_array[i], "SUB") == 0)
        {
            *changing_bytecode = SUB;
            changing_bytecode++;
            print_bytecode(bytecode, str_array[i], count, i);
            *bytecode += 1;
        }
        else if (strcmp(str_array[i], "MUL") == 0)
        {
            *changing_bytecode = MUL;
            changing_bytecode++;
            print_bytecode(bytecode, str_array[i], count, i);
            *bytecode += 1;
        }
        else if (strcmp(str_array[i], "DIV") == 0)
        {
            *changing_bytecode = DIV;
            changing_bytecode++;
            print_bytecode(bytecode, str_array[i], count, i);
            *bytecode += 1;
        }
        else if (strcmp(str_array[i], "SQRT") == 0)
        {
            *changing_bytecode = SQRT;
            changing_bytecode++;
            print_bytecode(bytecode, str_array[i], count, i);
            *bytecode += 2;
        }
        else if (strcmp(str_array[i], "OUT") == 0)
        {
            *changing_bytecode = OUT;
            changing_bytecode++;
            print_bytecode(bytecode, str_array[i], count, i);
            *bytecode += 1;
        }
        else if (strcmp(str_array[i], "HLT") == 0)
        {
            *changing_bytecode = HLT;
            changing_bytecode++;
            print_bytecode(bytecode, str_array[i], count, i);
            *bytecode += 1;
        }
    }

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

