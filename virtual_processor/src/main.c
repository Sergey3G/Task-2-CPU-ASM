#include "../include/processor.h"

int main()
{
	Processor processor = {};
	construct_processor(&processor, "byte_code.bin");
	execute_bytecode(&processor);
	destruct_processor(&processor);
	return 0;
}
