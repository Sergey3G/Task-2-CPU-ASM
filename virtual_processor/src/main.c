#include "../include/processor.h"

int main()
{
	Errors err = NO_ERRORS;

	Processor processor = {};
	construct_processor(&processor, "byte_code.bin");

	execute_bytecode(&processor);

	processor_dump(&processor);
	destruct_processor(&processor);
	return 0;
}
