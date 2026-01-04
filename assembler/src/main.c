#include <stdio.h>
#include <stdlib.h>

#include "../include/assembler.h"

int main(const int argc, const char* const argv[])
{
    if (argc < 2)
    {
        printf("You didn't enter input file name!\n");
        return 1;
    }

    const char* filename = argv[1];

    char* buffer = input_to_buffer(filename);
    if (!buffer)
    {
        return 1;
    }

    size_t strings_count = count_of_strings(buffer);
    char** str_array = create_str_array(buffer, strings_count);
    if (!str_array)
    {
        free(buffer);
        return 1;
    }

    int* bytecode = compile_to_bytecode(str_array, strings_count);
    if (!bytecode)
    {
        free(str_array);
        return 1;
    }

    FILE* output_file = fopen("../virtual_processor/byte_code.bin", "wb");
    if (!output_file)
    {
        printf("Error: cannot open file byte_code.bin!");
        return 1;
    }

    size_t wrote_count = fwrite(bytecode, sizeof(int), (size_t)bytecode[0] + 1, output_file);
    if (wrote_count != (size_t)bytecode[0] + 1)
    {
        printf("Error: cannot correctly write into file!");
        return 1;
    }

    fclose(output_file);
    free(buffer);
    free(str_array);
    free(bytecode);

    return 0;
}
