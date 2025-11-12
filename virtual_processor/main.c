#include <stdio.h>
#include <stdlib.h>

#include "stack_funcs.h"
#include "processor.h"

int main()
{
	int* bytecode = read_bytecode("byte_code.bin");
	if (!bytecode)
	{
		return 1;
	}

	Stack stack = {};
	construct_stack(&stack, 10);

	Processor processor = {};
	construct_processor(&processor, &stack, bytecode);

	processor_dump(&processor);
	destruct_stack(&stack);
	destruct_processor(&processor);
	return 0;
}
